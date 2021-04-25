#include "DisplayApp.h"
#include <libraries/log/nrf_log.h>

#include "components/battery/BatteryController.h"
#include "components/ble/BleController.h"
#include "components/datetime/DateTimeController.h"
#include "components/ble/NotificationManager.h"
#include "components/ble/CallNotificationManager.h"

#include "displayapp/screens/ShowMessage.h"
#include "displayapp/screens/ApplicationList.h"
#include "displayapp/screens/Clock.h"
#include "displayapp/screens/Charging.h"
#include "displayapp/screens/LowBatt.h"
#include "displayapp/screens/FirmwareUpdate.h"
#include "displayapp/screens/FirmwareValidation.h"
#include "displayapp/screens/Notifications.h"
#include "displayapp/screens/IncomingCall.h"
#include "displayapp/screens/About.h"
#include "displayapp/screens/Tile.h"
#include "displayapp/screens/Steps.h"
#include "displayapp/screens/FlashLight.h"
#include "displayapp/screens/Weather.h"
#include "displayapp/screens/Motion.h"

#include "displayapp/screens/settings/QuickSettings.h"
#include "displayapp/screens/settings/Settings.h"
#include "displayapp/screens/settings/SettingDisplay.h"
#include "displayapp/screens/settings/SettingTimeFormat.h"
#include "displayapp/screens/settings/SettingWatchFace.h"
#include "displayapp/screens/settings/SettingSteps.h"
#include "displayapp/screens/settings/SettingWakeUp.h"

#include "drivers/Cst816s.h"
#include "drivers/St7789.h"
#include "drivers/Watchdog.h"
#include "systemtask/SystemTask.h"

using namespace Pinetime::Applications;

DisplayApp::DisplayApp(Drivers::St7789 &lcd, Components::LittleVgl &lvgl, Drivers::Cst816S &touchPanel,
                       Controllers::Battery &batteryController, Controllers::Ble &bleController,
                       Drivers::SpiNorFlash& spiNorFlash,
                       Controllers::DateTime &dateTimeController, Drivers::WatchdogView &watchdog,                       
                       Controllers::Settings &settingsController,
                       Controllers::Accelerometer& accelerometer,
                       Controllers::BrightnessController& brightnessController,
                       System::SystemTask &systemTask,
                       Pinetime::Controllers::NotificationManager &notificationManager,
                       Pinetime::Controllers::CallNotificationManager &callNotificationManager) :
        lcd{lcd},
        lvgl{lvgl},
        touchPanel{touchPanel},
        batteryController{batteryController},
        bleController{bleController},
        spiNorFlash{spiNorFlash},
        dateTimeController{dateTimeController},
        watchdog{watchdog},        
        settingsController{settingsController},
        accelerometer{accelerometer},
        brightnessController{brightnessController},
        systemTask{systemTask},
        notificationManager{notificationManager},
        callNotificationManager{callNotificationManager}
{
  msgQueue = xQueueCreate(queueSize, itemSize);
  StartApp( Apps::Clock, DisplayApp::FullRefreshDirections::Down );
}

void DisplayApp::Start() {
  if (pdPASS != xTaskCreate(DisplayApp::Process, "DISP", 800, this, 0, &taskHandle))
    APP_ERROR_HANDLER(NRF_ERROR_NO_MEM);
}

void DisplayApp::Process(void *instance) {
  auto *app = static_cast<DisplayApp *>(instance);
  NRF_LOG_INFO("displayapp task started!");
  app->InitHw();

  // Send a dummy notification to unlock the lvgl display driver for the first iteration
  xTaskNotifyGive(xTaskGetCurrentTaskHandle());

  while (1) {
    app->Refresh();
  }
  
}

void DisplayApp::InitHw() {
  brightnessController.Set(settingsController.GetBrightness());
}

void DisplayApp::Refresh() {
  TickType_t queueTimeout;
  switch (state) {
    case States::Idle:
      IdleState();
      queueTimeout = portMAX_DELAY;
      break;
    case States::Running:
      RunningState();
      queueTimeout = 20;
      break;
    default:
      queueTimeout = portMAX_DELAY;
      break;
  }

  Messages msg;
  if (xQueueReceive(msgQueue, &msg, queueTimeout)) {
    switch (msg) {
      case Messages::GoToSleep:
        brightnessController.Backup();
        while(brightnessController.Level() != Controllers::BrightnessController::Levels::Off) {
          brightnessController.Lower();
          vTaskDelay(100);
        }
      
        systemTask.PushMessage(System::SystemTask::Messages::OnDisplayTaskSleeping);
        state = States::Idle;        

      break;

      case Messages::GoToRunning:
        if (state == States::Running) break;
        if(!currentScreen) {
          StartApp( Apps::Clock, DisplayApp::FullRefreshDirections::None );
        } else {
          if ( currentApp == Apps::Launcher || currentApp == Apps::QuickSettings || currentApp == Apps::Settings ) {
            StartApp( Apps::Clock, DisplayApp::FullRefreshDirections::None );
          }
        }
        
        brightnessController.Restore();
        state = States::Running;
        
      break;

      case Messages::UpdateTimeOut:
        systemTask.PushMessage(System::SystemTask::Messages::UpdateTimeOut);
      break;

      case Messages::NewCall:
        if ( currentApp != Apps::IncomingCall ) {
          StartApp( Apps::IncomingCall, DisplayApp::FullRefreshDirections::Down );
        }
      break;

      case Messages::NewNotification: 
        StartApp( Apps::Notifications, DisplayApp::FullRefreshDirections::Down );
      break;

      case Messages::TouchEvent: {
        if (state != States::Running) break;
        auto gesture = OnTouchEvent();
        if(!currentScreen->OnTouchEvent(gesture)) {
          if ( currentApp == Apps::Clock ) {
            switch (gesture) {
              case TouchEvents::SwipeUp:
                StartApp( Apps::Launcher, DisplayApp::FullRefreshDirections::Up );
                break;
              case TouchEvents::SwipeDown:
                StartApp( Apps::Notifications, DisplayApp::FullRefreshDirections::Down );
                break;
              case TouchEvents::SwipeRight:
                StartApp( Apps::QuickSettings, DisplayApp::FullRefreshDirections::RightAnim );
                break;
              case TouchEvents::DoubleTap:
                systemTask.PushMessage(System::SystemTask::Messages::GoToSleep);
                break;
              default:
                break;
            }
          } else {
            if ( returnTouchEvent == gesture ) {
              StartApp( returnToApp, returnDirection );
            }
          }
        }
      }
      break;
      
      case Messages::ButtonPushed:
        if( currentApp == Apps::Clock ) {
          systemTask.PushMessage(System::SystemTask::Messages::GoToSleep);
        } else {
          if ( !currentScreen->OnButtonPushed() ) {
            StartApp( returnToApp, returnDirection );
          }
        }
      break;

      case Messages::BleFirmwareUpdateStarted:
        StartApp( Apps::FirmwareUpdate, DisplayApp::FullRefreshDirections::Down );
      break;

      case Messages::ResourceUpdateStart:
        StartApp( Apps::ResourceUpdate, DisplayApp::FullRefreshDirections::Down );
      break;

      case Messages::StepEvent:
      break;
      
      case Messages::UpdateBleConnection:
      break;
      
      case Messages::UpdateBatteryLevel:
        batteryController.Update();
      break;

      case Messages::ChargingEvent :
        if( currentApp != Apps::Charging ) {
          StartApp( Apps::Charging, DisplayApp::FullRefreshDirections::Down );
        }
      break;

      case Messages::LowBattEvent :        
        if( currentApp != Apps::LowBatt ) {
          brightnessController.Set(Controllers::BrightnessController::Levels::Low);
          StartApp( Apps::LowBatt, DisplayApp::FullRefreshDirections::Down );
        }
      break;

    }
  }

  if(state != States::Idle && touchMode == TouchModes::Polling) {
    auto info = touchPanel.GetTouchInfo();
    if(info.action == 2) {// 2 = contact
      if(!currentScreen->OnTouchEvent(info.x, info.y)) {
        lvgl.SetNewTapEvent(info.x, info.y);
      }
    }
  }
}

void DisplayApp::RunningState() {

  if(!currentScreen->Refresh()) {
    StartApp( returnToApp, returnDirection );
  }

  lv_task_handler();
}

void DisplayApp::returnApp(Apps app, DisplayApp::FullRefreshDirections direction, TouchEvents touchEvent) {
  returnToApp = app;
  returnDirection = direction;
  returnTouchEvent = touchEvent;
}

void DisplayApp::StartApp(Apps app, DisplayApp::FullRefreshDirections direction) {
    
  currentScreen.reset(nullptr);
  
  SetFullRefresh( direction );

  switch(app) {
      case Apps::Launcher: 
        currentScreen = std::make_unique<Screens::ApplicationList>(this, dateTimeController, settingsController, batteryController); 
        returnApp(Apps::Clock, FullRefreshDirections::Down, TouchEvents::SwipeDown);
        break;
      case Apps::None:
      case Apps::Clock: 
        currentScreen = std::make_unique<Screens::Clock>(this, dateTimeController, batteryController, bleController, notificationManager, settingsController, accelerometer); 
        break;
      case Apps::Notifications:
        currentScreen = std::make_unique<Screens::Notifications>(this, notificationManager);
        returnApp(Apps::Clock, FullRefreshDirections::Up, TouchEvents::SwipeUp);
        break;
      case Apps::IncomingCall: 
        currentScreen = std::make_unique<Screens::IncomingCall>(this, callNotificationManager, systemTask.nimble().alertService(), systemTask); 
        returnApp(Apps::Clock, FullRefreshDirections::Down, TouchEvents::SwipeDown);
        break;
      case Apps::FlashLight: 
        currentScreen = std::make_unique<Screens::FlashLight>(this, systemTask, brightnessController);
        returnApp(Apps::Clock, FullRefreshDirections::Down, TouchEvents::SwipeDown);
        break;
      case Apps::QuickSettings: 
        currentScreen = std::make_unique<Screens::QuickSettings>(this, batteryController, dateTimeController, brightnessController, settingsController); 
        returnApp(Apps::Clock, FullRefreshDirections::LeftAnim, TouchEvents::SwipeLeft);
        break;

      // -----------------------------------------------------------------------------------------------------------------------------------------
      case Apps::Settings: 
        currentScreen = std::make_unique<Screens::Settings>(this, settingsController);
        returnApp(Apps::QuickSettings, FullRefreshDirections::Down, TouchEvents::SwipeDown);
        break;

      case Apps::SettingDisplay: 
        currentScreen = std::make_unique<Screens::SettingDisplay>(this, settingsController);
        returnApp(Apps::Settings, FullRefreshDirections::Down, TouchEvents::SwipeDown);
        break;

      case Apps::SettingTimeFormat: 
        currentScreen = std::make_unique<Screens::SettingTimeFormat>(this, settingsController);
        returnApp(Apps::Settings, FullRefreshDirections::Down, TouchEvents::SwipeDown);
        break;

      case Apps::SettingWatchFace: 
        currentScreen = std::make_unique<Screens::SettingWatchFace>(this, settingsController);
        returnApp(Apps::Settings, FullRefreshDirections::Down, TouchEvents::SwipeDown);
        break;
        
      case Apps::SettingWakeUp: 
        currentScreen = std::make_unique<Screens::SettingWakeUp>(this, settingsController);
        returnApp(Apps::Settings, FullRefreshDirections::Down, TouchEvents::SwipeDown);
        break;

      case Apps::SettingSteps: 
        currentScreen = std::make_unique<Screens::SettingSteps>(this, settingsController);
        returnApp(Apps::Settings, FullRefreshDirections::Down, TouchEvents::SwipeDown);
        break;

      case Apps::About: 
        currentScreen = std::make_unique<Screens::About>(this, dateTimeController, batteryController, brightnessController, bleController, watchdog, accelerometer, settingsController); 
        returnApp(Apps::Settings, FullRefreshDirections::Down, TouchEvents::SwipeDown);
        break;
      // -----------------------------------------------------------------------------------------------------------------------------------------

      case Apps::FirmwareUpdate:
        currentScreen = std::make_unique<Screens::FirmwareUpdate>(this, bleController, systemTask);
        returnApp(Apps::Clock, FullRefreshDirections::Down, TouchEvents::SwipeDown);
        //returnApp(currentApp == app ? Apps::Clock : currentApp, FullRefreshDirections::Down, TouchEvents::SwipeDown);
        break;
      case Apps::FirmwareValidation: 
        currentScreen = std::make_unique<Screens::FirmwareValidation>(this, validator); 
        returnApp(Apps::Settings, FullRefreshDirections::Down, TouchEvents::SwipeDown);
        break;

      case Apps::ResourceUpdate:
        currentScreen = std::make_unique<Screens::FirmwareUpdate>(this, bleController, systemTask);
        returnApp(Apps::Clock, FullRefreshDirections::Down, TouchEvents::SwipeDown);
        //returnApp(currentApp == app ? Apps::Clock : currentApp, FullRefreshDirections::Down, TouchEvents::SwipeDown);
        break;

      // -----------------------------------------------------------------------------------------------------------------------------------------
      
      case Apps::Charging:
        currentScreen = std::make_unique<Screens::Charging>(this, batteryController);
        //returnApp(Apps::Clock, FullRefreshDirections::Down, TouchEvents::SwipeDown);
        returnApp(currentApp == app or currentApp == Apps::LowBatt ? Apps::Clock : currentApp, FullRefreshDirections::Down, TouchEvents::SwipeDown);
        break;
      case Apps::LowBatt:
        currentScreen = std::make_unique<Screens::LowBatt>(this, batteryController);
        //returnApp(Apps::Clock, FullRefreshDirections::Down, TouchEvents::SwipeDown);
        returnApp(currentApp == app ? Apps::Clock : currentApp, FullRefreshDirections::Down, TouchEvents::SwipeDown);
        break;
      
      // -----------------------------------------------------------------------------------------------------------------------------------------

      case Apps::Steps: 
        currentScreen = std::make_unique<Screens::Steps>(this, accelerometer, settingsController);
        returnApp(Apps::Launcher, FullRefreshDirections::Down, TouchEvents::SwipeDown);
        break;
      case Apps::Weather:
        if (settingsController.GetWeather().hasData) {
          currentScreen = std::make_unique<Screens::Weather>(this, settingsController);
        } else {
          currentScreen = std::make_unique<Screens::ShowMessage>(this, "Wait for\nweather information.", "F:/meteo_sun.bin");
        }
        returnApp(Apps::Launcher, FullRefreshDirections::Down, TouchEvents::SwipeDown);
        break;
      case Apps::Motion: 
        currentScreen = std::make_unique<Screens::Motion>(this, accelerometer);
        returnApp(Apps::Launcher, FullRefreshDirections::Down, TouchEvents::SwipeDown);
        break;
    }
        
    currentApp = app;
}

void DisplayApp::IdleState() {

}

void DisplayApp::PushMessage(DisplayApp::Messages msg) {
  BaseType_t xHigherPriorityTaskWoken;
  xHigherPriorityTaskWoken = pdFALSE;
  xQueueSendFromISR(msgQueue, &msg, &xHigherPriorityTaskWoken);
  if (xHigherPriorityTaskWoken) {
    /* Actual macro used here is port specific. */
    // TODO : should I do something here?
  }
}

TouchEvents DisplayApp::OnTouchEvent() {
  auto info = touchPanel.GetTouchInfo();
  if(info.isTouch) {
    switch(info.gesture) {
      case Pinetime::Drivers::Cst816S::Gestures::SingleTap:        
        if(touchMode == TouchModes::Gestures)
          lvgl.SetNewTapEvent(info.x, info.y);
        return TouchEvents::Tap;
      case Pinetime::Drivers::Cst816S::Gestures::LongPress:
        return TouchEvents::LongTap;
      case Pinetime::Drivers::Cst816S::Gestures::DoubleTap:
        return TouchEvents::DoubleTap;
      case Pinetime::Drivers::Cst816S::Gestures::SlideRight:
        return TouchEvents::SwipeRight;
      case Pinetime::Drivers::Cst816S::Gestures::SlideLeft:
        return TouchEvents::SwipeLeft;
      case Pinetime::Drivers::Cst816S::Gestures::SlideDown:
        return TouchEvents::SwipeDown;
      case Pinetime::Drivers::Cst816S::Gestures::SlideUp:
        return TouchEvents::SwipeUp;
      case Pinetime::Drivers::Cst816S::Gestures::None:
      default:
        return TouchEvents::None;
    }
  }
  return TouchEvents::None;
}

void DisplayApp::SetFullRefresh(DisplayApp::FullRefreshDirections direction) {
  switch(direction){
    case DisplayApp::FullRefreshDirections::Down:
      lvgl.SetFullRefresh(Components::LittleVgl::FullRefreshDirections::Down);
      break;
    case DisplayApp::FullRefreshDirections::Up:
      lvgl.SetFullRefresh(Components::LittleVgl::FullRefreshDirections::Up);
      break;
    case DisplayApp::FullRefreshDirections::Left:
      lvgl.SetFullRefresh(Components::LittleVgl::FullRefreshDirections::Left);
      break;
    case DisplayApp::FullRefreshDirections::Right:
      lvgl.SetFullRefresh(Components::LittleVgl::FullRefreshDirections::Right);
      break;
    case DisplayApp::FullRefreshDirections::LeftAnim:
      lvgl.SetFullRefresh(Components::LittleVgl::FullRefreshDirections::LeftAnim);
      break;
    case DisplayApp::FullRefreshDirections::RightAnim:
      lvgl.SetFullRefresh(Components::LittleVgl::FullRefreshDirections::RightAnim);
      break;
    default: break;
  }

}

void DisplayApp::SetTouchMode(DisplayApp::TouchModes mode) {
  touchMode = mode;
}

