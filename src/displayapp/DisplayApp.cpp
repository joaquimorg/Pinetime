#include "DisplayApp.h"
#include <libraries/log/nrf_log.h>

#include "components/battery/BatteryController.h"
#include "components/ble/BleController.h"
#include "components/datetime/DateTimeController.h"
#include "components/ble/NotificationManager.h"
#include "components/ble/CallNotificationManager.h"

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
#include "displayapp/screens/QuickSettings.h"
#include "displayapp/screens/Steps.h"
#include "displayapp/screens/FlashLight.h"

#include "displayapp/screens/Settings.h"
#include "displayapp/screens/SettingDisplay.h"
#include "displayapp/screens/SettingTimeFormat.h"
#include "displayapp/screens/SettingWatchFace.h"
#include "displayapp/screens/SettingSteps.h"

#include "drivers/Cst816s.h"
#include "drivers/St7789.h"
#include "drivers/Watchdog.h"
#include "systemtask/SystemTask.h"

using namespace Pinetime::Applications;

DisplayApp::DisplayApp(Drivers::St7789 &lcd, Components::LittleVgl &lvgl, Drivers::Cst816S &touchPanel,
                       Controllers::Battery &batteryController, Controllers::Ble &bleController,
                       Controllers::DateTime &dateTimeController, Drivers::WatchdogView &watchdog,                       
                       Controllers::Settings &settingsController,
                       Drivers::BMA421 &stepCounter,
                       System::SystemTask &systemTask,
                       Pinetime::Controllers::NotificationManager &notificationManager,
                       Pinetime::Controllers::CallNotificationManager &callNotificationManager) :
        lcd{lcd},
        lvgl{lvgl},
        touchPanel{touchPanel},
        batteryController{batteryController},
        bleController{bleController},
        dateTimeController{dateTimeController},
        watchdog{watchdog},        
        settingsController{settingsController},
        stepCounter{stepCounter},
        systemTask{systemTask},
        notificationManager{notificationManager},
        callNotificationManager{callNotificationManager}/*,
        currentScreen{new Screens::Clock(this, dateTimeController, batteryController, bleController, notificationManager, settingsController, stepCounter) }*/
{
  msgQueue = xQueueCreate(queueSize, itemSize);
  LoadApp( Apps::Clock, DisplayApp::FullRefreshDirections::Down );
}

void DisplayApp::Start() {
  if (pdPASS != xTaskCreate(DisplayApp::Process, "displayapp", 640, this, 0, &taskHandle))
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
  brightnessController.Init();
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
          LoadApp( Apps::Clock, DisplayApp::FullRefreshDirections::None );
        } else {
          if ( currentApp == Apps::Launcher || currentApp == Apps::QuickSettings || currentApp == Apps::Settings ) {
            LoadApp( Apps::Clock, DisplayApp::FullRefreshDirections::None );
          }
        }
        
        brightnessController.Restore();
        state = States::Running;
        
      break;

      case Messages::UpdateTimeOut:
        systemTask.PushMessage(System::SystemTask::Messages::UpdateTimeOut);
      break;

      case Messages::NewCall:
        if ( callNotificationManager.IsInCall() ) {
          LoadApp( Apps::IncomingCall, DisplayApp::FullRefreshDirections::Down );
        } else {
          LoadApp( Apps::Clock, DisplayApp::FullRefreshDirections::Up );
        }
      break;

      case Messages::NewNotification: 
        LoadApp( Apps::Notifications, DisplayApp::FullRefreshDirections::Down );
      break;

      case Messages::TouchEvent: {
        break;
        if (state != States::Running) break;
        auto gesture = OnTouchEvent();
        if(!currentScreen->OnTouchEvent(gesture)) {
         /*switch (gesture) {
            case TouchEvents::SwipeUp:
              if(currentApp == Apps::Clock) {
                LoadApp( Apps::Launcher, DisplayApp::FullRefreshDirections::Up );
              } else {
                LoadApp( returnToApp, DisplayApp::FullRefreshDirections::Up );
              }
              break;

            case TouchEvents::SwipeDown:
              if( currentApp == Apps::Clock ) {
                LoadApp( Apps::Notifications, DisplayApp::FullRefreshDirections::Down );
              } else {
                LoadApp( returnToApp, DisplayApp::FullRefreshDirections::Down );
              }
              break;
            case TouchEvents::SwipeRight:
              if(currentApp == Apps::Clock) {
                LoadApp( Apps::QuickSettings, DisplayApp::FullRefreshDirections::None );
              }
              break;
            case TouchEvents::SwipeRight:
              if(currentApp == Apps::Clock) {
                LoadApp( Apps::QuickSettings, DisplayApp::FullRefreshDirections::None );
              }
              break;

            default:
              break;
          }*/
        }
      }
      break;
      
      case Messages::ButtonPushed:
        if( currentApp == Apps::Clock ) {
          //systemTask.PushMessage(System::SystemTask::Messages::GoToSleep);
          
          settingsController.SetClockFace(settingsController.GetClockFace() + 1);
          if ( settingsController.GetClockFace() > 3 ) {
            settingsController.SetClockFace(0);
            showapp++;
            if ( showapp > 17 ) showapp = 1;
            LoadApp( (Apps)showapp, DisplayApp::FullRefreshDirections::Up );
          } else {
            LoadApp( Apps::Clock, DisplayApp::FullRefreshDirections::Up );
          }
        } else {
          if ( !currentScreen->OnButtonPushed() ) {
            LoadApp( returnToApp, returnDirection );
          }
        }
      break;

      case Messages::BleFirmwareUpdateStarted:
        LoadApp( Apps::FirmwareUpdate, DisplayApp::FullRefreshDirections::Down );
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
          LoadApp( Apps::Charging, DisplayApp::FullRefreshDirections::Down );
        }
      break;

      case Messages::LowBattEvent :        
        if( currentApp != Apps::LowBatt ) {
          SetBrightness(Controllers::BrightnessController::Levels::Low);
          LoadApp( Apps::LowBatt, DisplayApp::FullRefreshDirections::Down );
        }
      break;

    }
  }

  /*if(state != States::Idle && touchMode == TouchModes::Polling) {
    auto info = touchPanel.GetTouchInfo();
    if(info.action == 2) {// 2 = contact
      if(!currentScreen->OnTouchEvent(info.x, info.y)) {
        lvgl.SetNewTapEvent(info.x, info.y);
      }
    }
  }*/
}

void DisplayApp::RunningState() {

  if(!currentScreen->Refresh()) {

    LoadApp( nextApp, nextDirection );
    //LoadApp( returnToApp, returnDirection );
    
    //nextApp = Apps::None;
    //nextDirection = DisplayApp::FullRefreshDirections::None;
  }

  lv_task_handler();
}

void DisplayApp::IdleState() {

}

void DisplayApp::PushMessage(DisplayApp::Messages msg) {
  BaseType_t xHigherPriorityTaskWoken;
  xHigherPriorityTaskWoken = pdFALSE;
  xQueueSendFromISR(msgQueue, &msg, &xHigherPriorityTaskWoken);
  portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
  //if (xHigherPriorityTaskWoken) {
    /* Actual macro used here is port specific. */
    // TODO : should I do something here?
  //}
}

TouchEvents DisplayApp::OnTouchEvent() {
  #ifndef P8CLONE
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
  #else
  //lvgl.SetFullRefresh(Components::LittleVgl::FullRefreshDirections::Down);
  //lvgl.SetNewTapEvent(120, 120);
  return TouchEvents::LongPress;  
  #endif
}

void DisplayApp::returnApp(Apps app, DisplayApp::FullRefreshDirections direction) {
  returnToApp = app;
  returnDirection = direction;
}

void DisplayApp::StartApp(Apps app, DisplayApp::FullRefreshDirections direction) {
  nextApp = app;
  nextDirection = direction;
}

void DisplayApp::LoadApp(Apps app, DisplayApp::FullRefreshDirections direction) {
    
  currentScreen.reset(nullptr);
  
  SetFullRefresh( direction );

  returnApp(currentApp, currentDirection);

  switch(app) {
      case Apps::None:
      case Apps::Launcher: 
        currentScreen.reset(new Screens::ApplicationList(this, dateTimeController, settingsController, batteryController)); 
        returnApp(Apps::Clock, FullRefreshDirections::Down);
        break;
      case Apps::Clock: 
        currentScreen.reset(new Screens::Clock(this, dateTimeController, batteryController, bleController, notificationManager, settingsController, stepCounter)); 
        break;
      case Apps::Notifications:
        currentScreen.reset(new Screens::Notifications(this, notificationManager));
        returnApp(Apps::Clock, FullRefreshDirections::Up);
        break;
      case Apps::IncomingCall: 
        currentScreen.reset(new Screens::IncomingCall(this, callNotificationManager, systemTask.nimble().alertService())); 
        returnApp(Apps::Clock, FullRefreshDirections::Down);
        break;
      case Apps::Steps: 
        currentScreen.reset(new Screens::Steps(this, stepCounter, settingsController));
        returnApp(Apps::Launcher, FullRefreshDirections::Down); 
        break;
      case Apps::FlashLight: 
        currentScreen.reset(new Screens::FlashLight(this, systemTask, brightnessController)); 
        returnApp(Apps::Clock, FullRefreshDirections::Down);
        break;
      case Apps::QuickSettings: 
        currentScreen.reset(new Screens::QuickSettings(this, batteryController, dateTimeController, brightnessController, settingsController)); 
        returnApp(Apps::Clock, FullRefreshDirections::None);
        break;

      // -----------------------------------------------------------------------------------------------------------------------------------------
      case Apps::Settings: 
        currentScreen.reset(new Screens::Settings(this, batteryController, dateTimeController, settingsController));
        returnApp(Apps::QuickSettings, FullRefreshDirections::Down);
        break;

      case Apps::SettingDisplay: 
        currentScreen.reset(new Screens::SettingDisplay(this, settingsController));
        returnApp(Apps::Settings, FullRefreshDirections::Down);
        break;

      case Apps::SettingTimeFormat: 
        currentScreen.reset(new Screens::SettingTimeFormat(this, settingsController));
        returnApp(Apps::Settings, FullRefreshDirections::Down);
        break;

      case Apps::SettingWatchFace: 
        currentScreen.reset(new Screens::SettingWatchFace(this, settingsController));
        returnApp(Apps::Settings, FullRefreshDirections::Down);
        break;

      case Apps::SettingSteps: 
        currentScreen.reset(new Screens::SettingSteps(this, settingsController));
        returnApp(Apps::Settings, FullRefreshDirections::Down);
        break;
      case Apps::About: 
        currentScreen.reset(new Screens::About(this, dateTimeController, batteryController, brightnessController, bleController, watchdog, stepCounter)); 
        returnApp(Apps::Settings, FullRefreshDirections::Down);
        break;
      case Apps::FirmwareUpdate:
        currentScreen.reset(new Screens::FirmwareUpdate(this, bleController));
        //returnApp(currentApp, currentDirection);
        break;
      case Apps::FirmwareValidation: 
        currentScreen.reset(new Screens::FirmwareValidation(this, validator)); 
        returnApp(Apps::Settings, FullRefreshDirections::Down);
        break;

      // -----------------------------------------------------------------------------------------------------------------------------------------
      
      case Apps::Charging:
        currentScreen.reset(new Screens::Charging(this, batteryController));
        //returnApp(currentApp, currentDirection);
        break;
      case Apps::LowBatt:
        currentScreen.reset(new Screens::LowBatt(this, batteryController));
        returnApp(Apps::Clock, FullRefreshDirections::None);
        //returnApp(currentApp, currentDirection);
        break;
      
    }
    
    currentApp = app;
    currentDirection = direction;
}

void DisplayApp::SetFullRefresh(DisplayApp::FullRefreshDirections direction) {
  switch(direction){
    case DisplayApp::FullRefreshDirections::Down:
      lvgl.SetFullRefresh(Components::LittleVgl::FullRefreshDirections::Down);
      break;
    case DisplayApp::FullRefreshDirections::Up:
      lvgl.SetFullRefresh(Components::LittleVgl::FullRefreshDirections::Up);
      break;
    default: break;
  }

}

void DisplayApp::SetTouchMode(DisplayApp::TouchModes mode) {
  touchMode = mode;
}

void DisplayApp::SetBrightness(Controllers::BrightnessController::Levels level) {
  brightnessController.Set(level);
}

