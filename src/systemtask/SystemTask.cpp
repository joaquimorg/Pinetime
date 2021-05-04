#include "SystemTask.h"
#define min // workaround: nimble's min/max macros conflict with libstdc++
#define max
#include <host/ble_gap.h>
#include <host/ble_gatt.h>
#include <host/ble_hs_adv.h>
#include <host/util/util.h>
#include <nimble/hci_common.h>
#undef max
#undef min
#include <hal/nrf_rtc.h>
#include <libraries/gpiote/app_gpiote.h>
#include <libraries/log/nrf_log.h>
#include <hal/nrf_gpio.h>
#include "main.h"
#include "board_config.h"
#include "BootloaderVersion.h"
#include "Version.h"

#include <memory>

using namespace Pinetime::System;

namespace {
  void IdleTimerCallback(TimerHandle_t xTimer) {
    //NRF_LOG_INFO("IdleTimerCallback");
    auto sysTask = static_cast<SystemTask *>(pvTimerGetTimerID(xTimer));
    sysTask->OnIdle();
  }

  void HardwareTimerCallback(TimerHandle_t xTimer) {
    auto sysTask = static_cast<SystemTask *>(pvTimerGetTimerID(xTimer));
    sysTask->HardwareStatus();
  }

  void WakeUpTimerCallback(TimerHandle_t xTimer) {
    auto sysTask = static_cast<SystemTask *>(pvTimerGetTimerID(xTimer));
    sysTask->WakeUpCheck();
  }
}

SystemTask::SystemTask(Drivers::SpiMaster &spi, Drivers::St7789 &lcd,
                       Drivers::SpiNorFlash& spiNorFlash,
                       Drivers::TwiMaster& twiMaster, Drivers::Cst816S &touchPanel, Controllers::Accelerometer& accelerometer,
                       Components::LittleVgl &lvgl,
                       Controllers::Battery &batteryController, Controllers::Ble &bleController,
                       Controllers::DateTime &dateTimeController,
                       Controllers::Settings &settingsController) :

                       spi{spi}, 
                       lcd{lcd}, 
                       spiNorFlash{spiNorFlash},
                       twiMaster{twiMaster}, 
                       touchPanel{touchPanel}, 
                       accelerometer{accelerometer},
                       lvgl{lvgl}, 
                       batteryController{batteryController},
                       bleController{bleController}, 
                       dateTimeController{dateTimeController}, 
                       settingsController{settingsController},
                       
                       watchdog{}, 
                       watchdogView{watchdog},

                       fs( spiNorFlash, FS_START_ADDRESS, TOTAL_SIZE ),
                       nimbleController(*this, bleController, dateTimeController, notificationManager, callNotificationManager, batteryController, spiNorFlash, settingsController, fs),
                       vrMotor( settingsController ) {
  systemTasksMsgQueue = xQueueCreate(10, 1);
}

void SystemTask::Start() {
  if (pdPASS != xTaskCreate(SystemTask::Process, "MAIN", 350, this, 0, &taskHandle))
    APP_ERROR_HANDLER(NRF_ERROR_NO_MEM);
}

void SystemTask::Process(void *instance) {
  auto *app = static_cast<SystemTask *>(instance);
  NRF_LOG_INFO("systemtask task started!");
  app->Work();
}

void SystemTask::Work() {

  watchdog.Setup(7);
  watchdog.Start();
  
  APP_GPIOTE_INIT(2);  

  twiMaster.Init();
  accelerometer.Init();

  spi.Init();
  lcd.Init();
  brightnessController.Init();
  //brightnessController.Set(Controllers::BrightnessController::Levels::Low);

  spiNorFlash.Init();
  spiNorFlash.Wakeup();
    
  settingsController.Init();  

  nimbleController.Init();
  nimbleController.StartAdvertising();

  touchPanel.Init();

  batteryController.Init();
  //batteryController.Update();

  fs.Init();

  // Button
  nrf_gpio_cfg_sense_input(KEY_ACTION, NRF_GPIO_PIN_PULLDOWN, NRF_GPIO_PIN_SENSE_HIGH);
  nrf_gpio_cfg_output(KEY_ENABLE);
  nrf_gpio_pin_set(KEY_ENABLE);

  static nrfx_gpiote_in_config_t const pinConfigKEY = {
      .sense = NRF_GPIOTE_POLARITY_LOTOHI,
      .pull = NRF_GPIO_PIN_PULLDOWN,
      .is_watcher = false,
      .hi_accuracy = false,
      .skip_gpio_setup = true,
    };
  
  nrfx_gpiote_in_init(KEY_ACTION, &pinConfigKEY, nrfx_gpiote_evt_handler);
  //

  // Touch IRQ
  nrf_gpio_cfg_sense_input(TP_IRQ, NRF_GPIO_PIN_PULLUP, NRF_GPIO_PIN_SENSE_LOW);

  static nrfx_gpiote_in_config_t const pinConfigTP = {
      .sense = NRF_GPIOTE_POLARITY_HITOLO,
      .pull = NRF_GPIO_PIN_PULLUP,
      .is_watcher = false,
      .hi_accuracy = false,
      .skip_gpio_setup = true,
    };
  
  nrfx_gpiote_in_init(TP_IRQ, &pinConfigTP, nrfx_gpiote_evt_handler);
  //

  // Step Counter IRQ
  /*nrf_gpio_cfg_sense_input(BMA421_IRQ, NRF_GPIO_PIN_PULLUP, NRF_GPIO_PIN_SENSE_LOW);

  static nrfx_gpiote_in_config_t const pinConfigBMA = {
      .sense = NRF_GPIOTE_POLARITY_TOGGLE,
      .pull = NRF_GPIO_PIN_PULLUP,
      .is_watcher = false,
      .hi_accuracy = false,
      .skip_gpio_setup = true,
    };
  
  nrfx_gpiote_in_init(BMA421_IRQ, &pinConfigBMA, nrfx_gpiote_evt_handler);*/
  //

  displayApp =  std::make_unique<Pinetime::Applications::DisplayApp>(
    lcd, lvgl, touchPanel, batteryController, bleController, spiNorFlash, 
    dateTimeController, watchdogView, settingsController, accelerometer, brightnessController,
    *this, notificationManager, callNotificationManager, fs);
    
  displayApp->Start(); 

  idleTimer = xTimerCreate ("idleTimer", pdMS_TO_TICKS(settingsController.GetScreenTimeOut()), pdFALSE, this, IdleTimerCallback);
  xTimerStart(idleTimer, 0);

  // Hardware status timer
  hardwareTimer = xTimerCreate ("hardwareTimer", pdMS_TO_TICKS(hardwareTime), pdTRUE, this, HardwareTimerCallback);
  xTimerStart(hardwareTimer, 0);   

  // WakeUp timer
  wakeUpTimer = xTimerCreate ("wakeUpTimer", pdMS_TO_TICKS(500), pdTRUE, this, WakeUpTimerCallback);
  //xTimerStart(wakeUpTimer, 0);

  vrMotor.Init();
  
  // Suppress endless loop diagnostic
  #pragma clang diagnostic push
  #pragma ide diagnostic ignored "EndlessLoop"
  while(true) {
    uint8_t msg;
    if (xQueueReceive(systemTasksMsgQueue, &msg, isSleeping ? 2500 : 1000)) {
      //batteryController.Update();
      
      Messages message = static_cast<Messages >(msg);
      switch(message) {
        case Messages::EnableSleeping:
          doNotGoToSleep = false;
        break;
        case Messages::DisableSleeping:
          doNotGoToSleep = true;
        break;
        case Messages::UpdateTimeOut:
          xTimerChangePeriod(idleTimer, pdMS_TO_TICKS(settingsController.GetScreenTimeOut()), 0);
        break;
        case Messages::WakeUp:
          //spi.Wakeup();
          //twiMaster.Wakeup();
          //accelerometer.Wakeup();

          nimbleController.StartAdvertising();
          
          spiNorFlash.Wakeup();

          // Double Tap needs the touch screen to be in normal mode
          if ( settingsController.getWakeUpMode() != Pinetime::Controllers::Settings::WakeUpMode::DoubleTap ) {
            //touchPanel.Wakeup();
          }
          lcd.Wakeup();
          //accelerometer.Update();

          isSleeping = false;
          isWakingUp = false;

          displayApp->PushMessage(Applications::DisplayApp::Messages::GoToRunning);
          //displayApp->PushMessage(Applications::DisplayApp::Messages::UpdateBatteryLevel);

          xTimerStart(idleTimer, 0);
          xTimerChangePeriod(hardwareTimer, pdMS_TO_TICKS(hardwareTime), 0);

          if ( settingsController.getWakeUpMode() == Pinetime::Controllers::Settings::WakeUpMode::RaiseWrist ) {
            xTimerStop(wakeUpTimer, 0);
          }

        break;
        case Messages::TouchWakeUp: {
          auto touchInfo = touchPanel.GetTouchInfo();
          if( touchInfo.isTouch and 
              (
                ( touchInfo.gesture == Pinetime::Drivers::Cst816S::Gestures::DoubleTap and 
                  settingsController.getWakeUpMode() == Pinetime::Controllers::Settings::WakeUpMode::DoubleTap 
                ) or
                ( touchInfo.gesture == Pinetime::Drivers::Cst816S::Gestures::SingleTap and 
                  settingsController.getWakeUpMode() == Pinetime::Controllers::Settings::WakeUpMode::SingleTap
                )
              )
            ) {
            WakeUp();
          }
        } break;
        case Messages::GoToSleep:
          isGoingToSleep = true;
          //NRF_LOG_INFO("[systemtask] Going to sleep");
          xTimerStop(idleTimer, 0);
          xTimerChangePeriod(hardwareTimer, pdMS_TO_TICKS(hardwareIdleTime), 0);

          displayApp->PushMessage(Applications::DisplayApp::Messages::GoToSleep);
          break;
        case Messages::OnNewNotification:
          if(isSleeping && !isWakingUp) {
            WakeUp();
          }
          vrMotor.Vibrate(35);
          displayApp->PushMessage(Applications::DisplayApp::Messages::NewNotification);

          break;
        case Messages::OnNewCall:
          if(isSleeping && !isWakingUp) WakeUp();
          vrMotor.Vibrate(35);
          displayApp->PushMessage(Applications::DisplayApp::Messages::NewCall);
          break;
        case Messages::BleConnected:
          ReloadIdleTimer();
          isBleDiscoveryTimerRunning = true;
          bleDiscoveryTimer = 5;
          break;
        case Messages::BleFirmwareUpdateStarted:
          doNotGoToSleep = true;
          if(isSleeping && !isWakingUp) WakeUp();
          displayApp->PushMessage(Applications::DisplayApp::Messages::BleFirmwareUpdateStarted);
          break;
        case Messages::BleFirmwareUpdateFinished:
          doNotGoToSleep = false;
          xTimerStart(idleTimer, 0);
          if(bleController.State() == Controllers::Ble::FirmwareUpdateStates::Validated)
            NVIC_SystemReset();
          break;
        case Messages::OnResourceUpdateStart:
          doNotGoToSleep = true;
          if(isSleeping && !isWakingUp) WakeUp();
          displayApp->PushMessage(Applications::DisplayApp::Messages::ResourceUpdateStart);
          break;

        case Messages::OnResourceUpdateEnd:
          doNotGoToSleep = false;
          xTimerStart(idleTimer, 0);
          //displayApp->PushMessage(Applications::DisplayApp::Messages::ResourceUpdateEnd);
          break;
        case Messages::OnTouchEvent:
          ReloadIdleTimer();
          break;
        case Messages::OnButtonEvent:
          ReloadIdleTimer();
          break;
        case Messages::ReloadIdleTimer:
          ReloadIdleTimer();
          break;
        case Messages::OnStepEvent:
          //vrMotor.Vibrate(25);
          //accelerometer.Update();
          break;
        case Messages::OnDisplayTaskSleeping:
          if(BootloaderVersion::IsValid()) {
            // First versions of the bootloader do not expose their version and cannot initialize the SPI NOR FLASH
            // if it's in sleep mode. Avoid bricked device by disabling sleep mode on these versions.
            spiNorFlash.Sleep();
          }
          lcd.Sleep();

          // Double Tap needs the touch screen to be in normal mode
          if ( settingsController.getWakeUpMode() != Pinetime::Controllers::Settings::WakeUpMode::DoubleTap ) {
            //touchPanel.Sleep();
          }
          //accelerometer.Sleep();
          //spi.Sleep();
          //twiMaster.Sleep();

          if ( settingsController.getWakeUpMode() == Pinetime::Controllers::Settings::WakeUpMode::RaiseWrist ) {
            xTimerStart(wakeUpTimer, 0);
          }

          isSleeping = true;
          isGoingToSleep = false;
          break;
        case Messages::PowerOFF:
          /*brightnessController.Set(Controllers::BrightnessController::Levels::Off);
          //xTimerStop(idleTimer, 0);
          //xTimerStop(hardwareTimer, 0);
          spiNorFlash.Sleep();
          lcd.Sleep();
          spi.Sleep();
          twiMaster.Sleep();
          accelerometer.Sleep();
          //isSleeping = true;
          nrf_gpio_cfg_default(TP_IRQ);
          nrf_gpio_cfg_default(BMA421_IRQ);
          nrf_gpio_cfg_default(KEY_ACTION);
          nrf_gpio_cfg_default(CHARGE_BASE_IRQ);
          nrf_gpio_cfg_default(CHARGE_IRQ);

          // Button
          nrf_gpio_cfg_sense_input(KEY_ACTION, (nrf_gpio_pin_pull_t)GPIO_PIN_CNF_PULL_Pulldown, (nrf_gpio_pin_sense_t)GPIO_PIN_CNF_SENSE_High);
          nrf_gpio_cfg_output(KEY_ENABLE);
          nrf_gpio_pin_set(KEY_ENABLE);
          NRF_POWER->SYSTEMOFF = 1;*/
          break;
        default: break;
      }
    }

    if(isBleDiscoveryTimerRunning) {
      if(bleDiscoveryTimer == 0) {
        isBleDiscoveryTimerRunning = false;
        // Services discovery is deffered from 3 seconds to avoid the conflicts between the host communicating with the
        // tharget and vice-versa. I'm not sure if this is the right way to handle this...
        nimbleController.StartDiscovery();
      } else {
        bleDiscoveryTimer--;
      }
    }

    //monitor.Process();
    //uint32_t systick_counter = nrf_rtc_counter_get(portNRF_RTC_REG);
    //dateTimeController.UpdateTime(systick_counter);

    dateTimeController.UpdateTime(nrf_rtc_counter_get(portNRF_RTC_REG));
    if(!nrf_gpio_pin_read(KEY_ACTION))
      watchdog.Kick();
  }
  // Clear diagnostic suppression
  #pragma clang diagnostic pop
}

void SystemTask::OnButtonPushed() {
  if(isGoingToSleep) return;
  if(!isSleeping) {
    //NRF_LOG_INFO("[systemtask] Button pushed");
    PushMessage(Messages::OnButtonEvent);
    displayApp->PushMessage(Applications::DisplayApp::Messages::ButtonPushed);
  }
  else if(!isWakingUp) {
    //NRF_LOG_INFO("[systemtask] Button pushed, waking up");
    WakeUp();
  }
}

void SystemTask::WakeUp() {
  isWakingUp = true;
  PushMessage(Messages::WakeUp);
}

void SystemTask::OnTouchEvent() {
   if(isGoingToSleep) return ;
  if(!isSleeping) {
    PushMessage(Messages::OnTouchEvent);
    displayApp->PushMessage(Pinetime::Applications::DisplayApp::Messages::TouchEvent);
  } else if(!isWakingUp) {
    if( settingsController.getWakeUpMode() == Pinetime::Controllers::Settings::WakeUpMode::None or 
          settingsController.getWakeUpMode() == Pinetime::Controllers::Settings::WakeUpMode::RaiseWrist ) return;
    PushMessage(Messages::TouchWakeUp);
  }
}

void SystemTask::OnStepEvent() {
  if(isGoingToSleep) return ;

  //vrMotor.Vibrate(10);
  
  /*
  //NRF_LOG_INFO("[systemtask] Step event");
  if(!isSleeping) {    
    //PushMessage(Messages::OnStepEvent);
    displayApp->PushMessage(Applications::DisplayApp::Messages::StepEvent);    
  }*/
}

void SystemTask::OnChargingEvent() {
  if(isGoingToSleep) return ;
  if(isSleeping && !isWakingUp) {
    WakeUp();
  }
  //vrMotor.Vibrate(35);
  displayApp->PushMessage(Applications::DisplayApp::Messages::ChargingEvent);
}

void SystemTask::OnPowerPresentEvent() {  
  if(isGoingToSleep) return ;
  /*if(isSleeping && !isWakingUp) {
    WakeUp();
  }
  if ( !batteryController.IsCharging() ) {
    //vrMotor.Vibrate(35);
    displayApp->PushMessage(Applications::DisplayApp::Messages::ChargingEvent);
  }*/
}

void SystemTask::PushMessage(SystemTask::Messages msg) {
  if(msg == Messages::GoToSleep) {
    isGoingToSleep = true;
  }
  BaseType_t xHigherPriorityTaskWoken;
  xHigherPriorityTaskWoken = pdFALSE;
  xQueueSendFromISR(systemTasksMsgQueue, &msg, &xHigherPriorityTaskWoken);
  if (xHigherPriorityTaskWoken) {
    /* Actual macro used here is port specific. */
    // TODO: should I do something here?
  }
}

void SystemTask::OnIdle() {
  if(doNotGoToSleep) return;
  NRF_LOG_INFO("Idle timeout -> Going to sleep")
  PushMessage(Messages::GoToSleep);
}

void SystemTask::ReloadIdleTimer() const {
  if(isSleeping || isGoingToSleep) return;
  xTimerReset(idleTimer, 0);
}


void SystemTask::WakeUpCheck() {
  if(isGoingToSleep) return ;
  if(isSleeping and !isWakingUp) {
    if( settingsController.getWakeUpMode() == Pinetime::Controllers::Settings::WakeUpMode::RaiseWrist ) {
      if ( accelerometer.WristRotate() ) {
        WakeUp();
      }
    }
  }
}

void SystemTask::HardwareStatus() {

  accelerometer.Update();  
  // verify the day to reset de counter
  settingsController.SetHistorySteps( accelerometer, dateTimeController );  
  // Update Battery status
  batteryController.Update();

  if(isGoingToSleep) return ;
  if(isSleeping and !isWakingUp) {    

    if ( !bleController.IsConnected() ) {
      nimbleController.StartAdvertising();
    }
    
    // verify batt status to alert if is to low
    if ( batteryController.PercentRemaining() >= 0 and batteryController.PercentRemaining() < 10 and !batteryController.IsCharging() ) {          
        WakeUp();
        brightnessController.Set(Controllers::BrightnessController::Levels::Low);
        vrMotor.Vibrate(35);
        displayApp->PushMessage(Applications::DisplayApp::Messages::LowBattEvent);

        // if to low switch off
        if ( batteryController.PercentRemaining() < 5 ) {
          PushMessage(Messages::PowerOFF);
        }
    }

    // verify if batt is charged
    if ( !batteryController.IsCharging() and batteryController.IsPowerPresent() ) {
        WakeUp();
        displayApp->PushMessage(Applications::DisplayApp::Messages::ChargingEvent);
    }
    
  }

}