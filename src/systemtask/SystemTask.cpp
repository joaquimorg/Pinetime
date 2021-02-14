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

#include "BootloaderVersion.h"
#include "components/ble/BleController.h"
#include "displayapp/LittleVgl.h"
#include "drivers/BMA421.h"
#include "drivers/Cst816s.h"
#include "drivers/St7789.h"
#include "drivers/InternalFlash.h"
#include "drivers/SpiMaster.h"
#include "drivers/SpiNorFlash.h"
#include "drivers/TwiMaster.h"
#include "main.h"
#include "board_config.h"

using namespace Pinetime::System;

void IdleTimerCallback(TimerHandle_t xTimer) {

  NRF_LOG_INFO("IdleTimerCallback");
  auto sysTask = static_cast<SystemTask *>(pvTimerGetTimerID(xTimer));
  sysTask->OnIdle();
}


SystemTask::SystemTask(Drivers::SpiMaster &spi, Drivers::St7789 &lcd,
                       Pinetime::Drivers::SpiNorFlash& spiNorFlash,
                       Drivers::TwiMaster& twiMaster, Drivers::Cst816S &touchPanel, Drivers::BMA421& stepCounter,
                       Components::LittleVgl &lvgl,
                       Controllers::Battery &batteryController, Controllers::Ble &bleController,
                       Controllers::DateTime &dateTimeController,
                       Controllers::Settings &settingsController) :
                       spi{spi}, lcd{lcd}, spiNorFlash{spiNorFlash},
                       twiMaster{twiMaster}, touchPanel{touchPanel}, stepCounter{stepCounter},
                       lvgl{lvgl}, batteryController{batteryController},
                       bleController{bleController}, dateTimeController{dateTimeController}, settingsController{settingsController},
                       watchdog{}, watchdogView{watchdog},
                       nimbleController(*this, bleController,dateTimeController, notificationManager, batteryController, spiNorFlash) {
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
  ret_code_t errCode;

  watchdog.Setup(7);
  watchdog.Start();
  NRF_LOG_INFO("Last reset reason : %s", Pinetime::Drivers::Watchdog::ResetReasonToString(watchdog.ResetReason()));
  APP_GPIOTE_INIT(3);

  spi.Init();
  spiNorFlash.Init();
  spiNorFlash.Wakeup();

  nimbleController.Init();
  nimbleController.StartAdvertising();
  lcd.Init();

  twiMaster.Init();
  touchPanel.Init();
  
  batteryController.Init();
  settingsController.Init();

  stepCounter.Init();

  displayApp.reset(new Pinetime::Applications::DisplayApp(lcd, lvgl, touchPanel, batteryController, bleController,
                                                          dateTimeController, watchdogView, settingsController, stepCounter, *this, notificationManager));
  displayApp->Start();

  batteryController.Update();
  displayApp->PushMessage(Pinetime::Applications::DisplayApp::Messages::UpdateBatteryLevel);

  if(!nrf_drv_gpiote_is_init())
  {
        errCode = nrf_drv_gpiote_init();                                       
        APP_ERROR_CHECK(errCode);
  }

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
  nrf_gpio_cfg_sense_input(BMA421_IRQ, (nrf_gpio_pin_pull_t)GPIO_PIN_CNF_PULL_Pullup, (nrf_gpio_pin_sense_t)GPIO_PIN_CNF_SENSE_Low);

  pinConfig.skip_gpio_setup = true;
  pinConfig.hi_accuracy = false;
  pinConfig.is_watcher = false;
  pinConfig.sense = (nrf_gpiote_polarity_t)NRF_GPIOTE_POLARITY_HITOLO;
  pinConfig.pull = (nrf_gpio_pin_pull_t)GPIO_PIN_CNF_PULL_Pullup;

  nrfx_gpiote_in_init(BMA421_IRQ, &pinConfig, nrfx_gpiote_evt_handler);
  //

  idleTimer = xTimerCreate ("idleTimer", pdMS_TO_TICKS(idleTime), pdFALSE, this, IdleTimerCallback);
  xTimerStart(idleTimer, 0);
  
  vibration.Init();

  // Suppress endless loop diagnostic
  #pragma clang diagnostic push
  #pragma ide diagnostic ignored "EndlessLoop"
  while(true) {
    uint8_t msg;
    if (xQueueReceive(systemTasksMsgQueue, &msg, isSleeping ? 2500 : 1000)) {
      batteryController.Update();
      
      Messages message = static_cast<Messages >(msg);
      switch(message) {
        case Messages::EnableSleeping:
          doNotGoToSleep = false;
        break;
        case Messages::DisableSleeping:
          doNotGoToSleep = true;
        break;
        case Messages::WakeUp:
          spi.Wakeup();
          twiMaster.Wakeup();
          //stepCounter.Wakeup();

          nimbleController.StartAdvertising();
          xTimerStart(idleTimer, 0);
          spiNorFlash.Wakeup();
          touchPanel.Wakeup();
          lcd.Wakeup();
          stepCounter.Update();

          isSleeping = false;
          isWakingUp = false;

          displayApp->PushMessage(Applications::DisplayApp::Messages::GoToRunning);
          displayApp->PushMessage(Applications::DisplayApp::Messages::UpdateBatteryLevel);

        break;
        case Messages::GoToRunning:
         
          displayApp->PushMessage(Applications::DisplayApp::Messages::GoToRunning);
          displayApp->PushMessage(Applications::DisplayApp::Messages::UpdateBatteryLevel);

          break;
        case Messages::GoToSleep:
          isGoingToSleep = true;
          NRF_LOG_INFO("[systemtask] Going to sleep");
          xTimerStop(idleTimer, 0);
          displayApp->PushMessage(Pinetime::Applications::DisplayApp::Messages::GoToSleep);
          break;
        case Messages::OnNewNotification:
          if(isSleeping && !isWakingUp) {
            WakeUp();
          }
          vibration.Vibrate(35);
          displayApp->PushMessage(Pinetime::Applications::DisplayApp::Messages::NewNotification);

          break;
        case Messages::OnNewCall:
          if(isSleeping && !isWakingUp) WakeUp();
          vibration.Vibrate(35);
          displayApp->PushMessage(Pinetime::Applications::DisplayApp::Messages::NewCall);
          break;
        case Messages::BleConnected:
          ReloadIdleTimer();
          isBleDiscoveryTimerRunning = true;
          bleDiscoveryTimer = 5;
          break;
        case Messages::BleFirmwareUpdateStarted:
          doNotGoToSleep = true;
          if(isSleeping && !isWakingUp) WakeUp();
          displayApp->PushMessage(Pinetime::Applications::DisplayApp::Messages::BleFirmwareUpdateStarted);
          break;
        case Messages::BleFirmwareUpdateFinished:
          doNotGoToSleep = false;
          xTimerStart(idleTimer, 0);
          if(bleController.State() == Pinetime::Controllers::Ble::FirmwareUpdateStates::Validated)
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
          vibration.Vibrate(25);
          stepCounter.Update();
          break;
        case Messages::OnDisplayTaskSleeping:
          if(BootloaderVersion::IsValid()) {
            // First versions of the bootloader do not expose their version and cannot initialize the SPI NOR FLASH
            // if it's in sleep mode. Avoid bricked device by disabling sleep mode on these versions.
            spiNorFlash.Sleep();
          }
          lcd.Sleep();
          touchPanel.Sleep();
          //stepCounter.Sleep();
          spi.Sleep();
          twiMaster.Sleep();
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
    NRF_LOG_INFO("[systemtask] Button pushed");
    PushMessage(Messages::OnButtonEvent);
    displayApp->PushMessage(Pinetime::Applications::DisplayApp::Messages::ButtonPushed);
  }
  else {
    if(!isWakingUp) {
      NRF_LOG_INFO("[systemtask] Button pushed, waking up");
      WakeUp();
      //GoToRunning();
    }
  }
}

void SystemTask::GoToRunning() {
  //isWakingUp = true;
  PushMessage(Messages::GoToRunning);
}

void SystemTask::WakeUp() {
  isWakingUp = true;
  PushMessage(Messages::WakeUp);
}

void SystemTask::OnTouchEvent() {
  if(isGoingToSleep) return ;
  NRF_LOG_INFO("[systemtask] Touch event");
  if(!isSleeping) {
    PushMessage(Messages::OnTouchEvent);
    displayApp->PushMessage(Pinetime::Applications::DisplayApp::Messages::TouchEvent);
  }
}

void SystemTask::OnStepEvent() {
  stepCounter.Update();
  if(isGoingToSleep) return ;
  NRF_LOG_INFO("[systemtask] Step event");
  if(!isSleeping) {    
    //PushMessage(Messages::OnStepEvent);
    displayApp->PushMessage(Pinetime::Applications::DisplayApp::Messages::StepEvent);    
  }
}

void SystemTask::OnChargingEvent() {
  if(isGoingToSleep) return ;
  /*if(isSleeping && !isWakingUp) {
    WakeUp();
  }
  //vibration.Vibrate(35);
  displayApp->PushMessage(Pinetime::Applications::DisplayApp::Messages::ChargingEvent);*/
}

void SystemTask::OnPowerPresentEvent() {  
  if(isGoingToSleep) return ;
  if(isSleeping && !isWakingUp) {
    WakeUp();
  }
  if ( !batteryController.IsCharging() ) {
    //vibration.Vibrate(35);
    displayApp->PushMessage(Pinetime::Applications::DisplayApp::Messages::ChargingEvent);
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
