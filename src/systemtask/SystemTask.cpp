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

#include "main.h"
#include "board_config.h"
#include "BootloaderVersion.h"

using namespace Pinetime::System;

void IdleTimerCallback(TimerHandle_t xTimer) {

  //NRF_LOG_INFO("IdleTimerCallback");
  auto sysTask = static_cast<SystemTask *>(pvTimerGetTimerID(xTimer));
  sysTask->OnIdle();
}

void HardwareTimerCallback(TimerHandle_t xTimer) {
  auto sysTask = static_cast<SystemTask *>(pvTimerGetTimerID(xTimer));
  sysTask->HardwareStatus();
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

                       nimbleController(*this, bleController, dateTimeController, notificationManager, callNotificationManager, batteryController, spiNorFlash),
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
  //ret_code_t errCode;

  watchdog.Setup(7);
  watchdog.Start();
  APP_GPIOTE_INIT(6);

  /*if(!nrf_drv_gpiote_is_init()) {
    errCode = nrf_drv_gpiote_init();                                       
    APP_ERROR_CHECK(errCode);
  }*/

  spi.Init();
  spiNorFlash.Init();
  spiNorFlash.Wakeup();

  nimbleController.Init();
  nimbleController.StartAdvertising();
  lcd.Init();

  twiMaster.Init();
  touchPanel.Init();
  
  batteryController.Init();
  batteryController.Update();

  settingsController.Init();

  accelerometer.Init();

  displayApp.reset(new Applications::DisplayApp(lcd, lvgl, touchPanel, batteryController, bleController,
                                                          dateTimeController, watchdogView, settingsController, accelerometer, *this, notificationManager, callNotificationManager));
  displayApp->Start();

  //displayApp->PushMessage(Applications::DisplayApp::Messages::UpdateBatteryLevel);

  // Button
  nrf_gpio_cfg_sense_input(KEY_ACTION, (nrf_gpio_pin_pull_t)GPIO_PIN_CNF_PULL_Pulldown, (nrf_gpio_pin_sense_t)GPIO_PIN_CNF_SENSE_High);
  nrf_gpio_cfg_output(KEY_ENABLE);
  nrf_gpio_pin_set(KEY_ENABLE);

  nrfx_gpiote_in_config_t pinConfig;
  pinConfig.skip_gpio_setup = true;
  pinConfig.hi_accuracy = false;
  pinConfig.is_watcher = false;
  pinConfig.sense = (nrf_gpiote_polarity_t)NRF_GPIOTE_POLARITY_HITOLO;
  pinConfig.pull = (nrf_gpio_pin_pull_t)GPIO_PIN_CNF_PULL_Pulldown;

  nrfx_gpiote_in_init(KEY_ACTION, &pinConfig, nrfx_gpiote_evt_handler);
  //

  // Touch IRQ
  nrf_gpio_cfg_sense_input(TP_IRQ, (nrf_gpio_pin_pull_t)GPIO_PIN_CNF_PULL_Pullup, (nrf_gpio_pin_sense_t)GPIO_PIN_CNF_SENSE_Low);

  pinConfig.skip_gpio_setup = true;
  pinConfig.hi_accuracy = false;
  pinConfig.is_watcher = false;
  pinConfig.sense = (nrf_gpiote_polarity_t)NRF_GPIOTE_POLARITY_HITOLO;
  pinConfig.pull = (nrf_gpio_pin_pull_t)GPIO_PIN_CNF_PULL_Pullup;

  nrfx_gpiote_in_init(TP_IRQ, &pinConfig, nrfx_gpiote_evt_handler);
  //

  // Step Counter IRQ
  /*nrf_gpio_cfg_sense_input(BMA421_IRQ, (nrf_gpio_pin_pull_t)GPIO_PIN_CNF_PULL_Pullup, (nrf_gpio_pin_sense_t)GPIO_PIN_CNF_SENSE_Low);

  pinConfig.skip_gpio_setup = true;
  pinConfig.hi_accuracy = false;
  pinConfig.is_watcher = false;
  pinConfig.sense = (nrf_gpiote_polarity_t)NRF_GPIOTE_POLARITY_HITOLO;
  pinConfig.pull = (nrf_gpio_pin_pull_t)GPIO_PIN_CNF_PULL_Pullup;

  nrfx_gpiote_in_init(BMA421_IRQ, &pinConfig, nrfx_gpiote_evt_handler);*/
  //

  idleTimer = xTimerCreate ("idleTimer", pdMS_TO_TICKS(settingsController.GetScreenTimeOut()), pdFALSE, this, IdleTimerCallback);
  xTimerStart(idleTimer, 0);

  // Hardware status timer
  hardwareTimer = xTimerCreate ("hardwareTimer", pdMS_TO_TICKS(hardwareTime), pdTRUE, this, HardwareTimerCallback);
  xTimerStart(hardwareTimer, 0);

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
          spi.Wakeup();
          twiMaster.Wakeup();
          //accelerometer.Wakeup();

          nimbleController.StartAdvertising();
          
          spiNorFlash.Wakeup();
          touchPanel.Wakeup();
          lcd.Wakeup();
          //accelerometer.Update();

          isSleeping = false;
          isWakingUp = false;

          displayApp->PushMessage(Applications::DisplayApp::Messages::GoToRunning);
          //displayApp->PushMessage(Applications::DisplayApp::Messages::UpdateBatteryLevel);

          xTimerStart(idleTimer, 0);
          xTimerChangePeriod(hardwareTimer, pdMS_TO_TICKS(hardwareTime), 0);

        break;
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
          vrMotor.Vibrate(25);
          accelerometer.Update();
          break;
        case Messages::OnDisplayTaskSleeping:
          if(BootloaderVersion::IsValid()) {
            // First versions of the bootloader do not expose their version and cannot initialize the SPI NOR FLASH
            // if it's in sleep mode. Avoid bricked device by disabling sleep mode on these versions.
            spiNorFlash.Sleep();
          }
          lcd.Sleep();
          touchPanel.Sleep();
          //accelerometer.Sleep();
          //spi.Sleep();
          //twiMaster.Sleep();
          isSleeping = true;
          isGoingToSleep = false;
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

    monitor.Process();
    uint32_t systick_counter = nrf_rtc_counter_get(portNRF_RTC_REG);
    dateTimeController.UpdateTime(systick_counter);
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
  else {
    if(!isWakingUp) {
      //NRF_LOG_INFO("[systemtask] Button pushed, waking up");
      WakeUp();
    }
  }
}

void SystemTask::WakeUp() {
  isWakingUp = true;
  PushMessage(Messages::WakeUp);
}

void SystemTask::OnTouchEvent() {
  if(isGoingToSleep) return ;
  //NRF_LOG_INFO("[systemtask] Touch event");
  if(!isSleeping) {
    PushMessage(Messages::OnTouchEvent);
    displayApp->PushMessage(Applications::DisplayApp::Messages::TouchEvent);
  }
}

void SystemTask::OnStepEvent() {
  accelerometer.Update();
  if(isGoingToSleep) return ;
  //NRF_LOG_INFO("[systemtask] Step event");
  if(!isSleeping) {    
    //PushMessage(Messages::OnStepEvent);
    displayApp->PushMessage(Applications::DisplayApp::Messages::StepEvent);    
  }
}

void SystemTask::OnChargingEvent() {
  if(isGoingToSleep) return ;
  /*if(isSleeping && !isWakingUp) {
    WakeUp();
  }
  //vrMotor.Vibrate(35);
  displayApp->PushMessage(Applications::DisplayApp::Messages::ChargingEvent);*/
}

void SystemTask::OnPowerPresentEvent() {  
  if(isGoingToSleep) return ;
  if(isSleeping && !isWakingUp) {
    WakeUp();
  }
  if ( !batteryController.IsCharging() ) {
    //vrMotor.Vibrate(35);
    displayApp->PushMessage(Applications::DisplayApp::Messages::ChargingEvent);
  }
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


void SystemTask::HardwareStatus() {

    
  accelerometer.Update();  
  // verify the day to reset de counter
  settingsController.SetHistorySteps( accelerometer, dateTimeController );
  
  // Update Battery status
  batteryController.Update();

  // verify batt status to alert if is to low
  if ( batteryController.PercentRemaining() >= 0 && batteryController.PercentRemaining() < 15 && !batteryController.IsCharging() ) {
    if(isGoingToSleep) return ;
    if(isSleeping && !isWakingUp) {
      WakeUp();
      vrMotor.Vibrate(35);
      displayApp->PushMessage(Applications::DisplayApp::Messages::LowBattEvent);
    }
  }

}