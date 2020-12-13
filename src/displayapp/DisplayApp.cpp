#include "DisplayApp.h"
#include <libraries/log/nrf_log.h>
#include "components/battery/BatteryController.h"
#include "components/ble/BleController.h"
#include "components/datetime/DateTimeController.h"
#include "components/ble/NotificationManager.h"
#include "displayapp/screens/ApplicationList.h"
#include "displayapp/screens/Brightness.h"
#include "displayapp/screens/Clock.h"
#include "displayapp/screens/FirmwareUpdate.h"
#include "displayapp/screens/FirmwareValidation.h"
#include "displayapp/screens/Gauge.h"
#include "displayapp/screens/InfiniPaint.h"
#include "displayapp/screens/Meter.h"
#include "displayapp/screens/Music.h"
#include "displayapp/screens/Notifications.h"
#include "displayapp/screens/SystemInfo.h"
#include "displayapp/screens/Tile.h"
#include "displayapp/screens/FileManager.h"
#include "displayapp/screens/Settings.h"
#include "displayapp/screens/Steps.h"
#include "displayapp/screens/HeartRate.h"
#include "displayapp/screens/ScreensTemplate.h"
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
                       Drivers::HRS3300 &hrs,
                       System::SystemTask &systemTask,
                       Pinetime::Controllers::NotificationManager &notificationManager) :
        lcd{lcd},
        lvgl{lvgl},
        touchPanel{touchPanel},
        batteryController{batteryController},
        bleController{bleController},
        dateTimeController{dateTimeController},
        watchdog{watchdog},        
        settingsController{settingsController},
        stepCounter{stepCounter},
        hrs{hrs},
        systemTask{systemTask},
        notificationManager{notificationManager},
        currentScreen{new Screens::Clock(this, dateTimeController, batteryController, bleController, notificationManager, settingsController, stepCounter) }        
{
  msgQueue = xQueueCreate(queueSize, itemSize);
  onClockApp = true;
  //modal.reset(new Screens::Modal(this));
}

void DisplayApp::Start() {
  if (pdPASS != xTaskCreate(DisplayApp::Process, "displayapp", 512, this, 0, &taskHandle))
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

uint32_t acc = 0;
uint32_t count = 0;
bool toggle = true;

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
        //lcd.DisplayOff();        
        systemTask.PushMessage(System::SystemTask::Messages::OnDisplayTaskSleeping);
        state = States::Idle;
        onClockApp = false;
        //currentScreen.reset(nullptr);
      break;

      case Messages::GoToRunning:
        //if (state == States::Running) break;
        //if(!onClockApp) {
          onClockApp = true;
          //lcd.DisplayOn();
          currentScreen.reset(nullptr);
          currentScreen.reset(new Screens::Clock(this, dateTimeController, batteryController, bleController, notificationManager, settingsController, stepCounter));        
        //}
        //if (state != States::Running) {
          brightnessController.Restore();
          state = States::Running;
        //}

      break;

      case Messages::NewCall:
      break;

      case Messages::NewNotification: 
        
        if (state == States::Running) {
          onClockApp = false;
          lvgl.SetFullRefresh(Components::LittleVgl::FullRefreshDirections::Down);
          currentScreen.reset(nullptr);
          currentScreen.reset(new Screens::Notifications(this, notificationManager, Screens::Notifications::Modes::Preview));
        } else {
          PushMessage(Messages::GoToRunning);
          //PushMessage(Messages::NewNotification);
          //PushMessage(Messages::NewNotification);
          //onClockApp = false;
          //currentScreen.reset(nullptr);
          //currentScreen.reset(new Screens::Notifications(this, notificationManager, Screens::Notifications::Modes::Preview));
          //brightnessController.Restore();
          //state = States::Running;
        }
                
        //currentScreen.reset(new Screens::Clock(this, dateTimeController, batteryController, bleController, notificationManager, settingsController, stepCounter));        
        /*
        if (state != States::Running) {
          brightnessController.Restore();
          state = States::Running;
        }*/
        
      break;

      case Messages::TouchEvent: {
        if (state != States::Running) break;
        auto gesture = OnTouchEvent();
        if(!currentScreen->OnTouchEvent(gesture)) {
          switch (gesture) {
            case TouchEvents::SwipeUp:
              if(onClockApp) {
                onClockApp = false;
                //currentScreen->OnButtonPushed();
                lvgl.SetFullRefresh(Components::LittleVgl::FullRefreshDirections::Up);
                currentScreen.reset(nullptr);
                currentScreen.reset(new Screens::ApplicationList(this, dateTimeController, settingsController));
              } else {
                lvgl.SetFullRefresh(Components::LittleVgl::FullRefreshDirections::Up);
                currentScreen->OnButtonPushed();
              }
              break;
            /*case TouchEvents::SwipeDown:
              currentScreen->OnButtonPushed();
              lvgl.SetFullRefresh(Components::LittleVgl::FullRefreshDirections::Down);
              break;*/
            case TouchEvents::SwipeDown:
              if(!onClockApp) {
                /*lvgl.SetFullRefresh(Components::LittleVgl::FullRefreshDirections::Down);
                currentScreen.reset(nullptr);
                currentScreen.reset(new Screens::Clock(this, dateTimeController, batteryController, bleController, notificationManager, settingsController, stepCounter));
                onClockApp = true;*/
                lvgl.SetFullRefresh(Components::LittleVgl::FullRefreshDirections::Down);
                currentScreen->OnButtonPushed();
              } else {
                onClockApp = false;
                lvgl.SetFullRefresh(Components::LittleVgl::FullRefreshDirections::Down);
                currentScreen.reset(nullptr);
                currentScreen.reset(new Screens::Notifications(this, notificationManager, Screens::Notifications::Modes::Clock));
              }
              break;
            default:
              break;
          }
        }
      }
      break;
      
      case Messages::ButtonPushed:
        if(onClockApp)
            systemTask.PushMessage(System::SystemTask::Messages::GoToSleep);
          else {
            auto buttonUsedByApp = currentScreen->OnButtonPushed();
            if (!buttonUsedByApp) {
              systemTask.PushMessage(System::SystemTask::Messages::GoToSleep);
            } else {
              lvgl.SetFullRefresh(Components::LittleVgl::FullRefreshDirections::Up);
          }
        }
      break;

      case Messages::BleFirmwareUpdateStarted:
        lvgl.SetFullRefresh(Components::LittleVgl::FullRefreshDirections::Down);
        currentScreen.reset(nullptr);
        currentScreen.reset(new Screens::FirmwareUpdate(this, bleController));
        onClockApp = false;
      break;

      case Messages::StepEvent:
      break;
      
      case Messages::UpdateDateTime:
      break;
      
      case Messages::UpdateBleConnection:
      break;
      
      case Messages::UpdateBatteryLevel:
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
//  clockScreen.SetCurrentDateTime(dateTimeController.CurrentDateTime());

  if(!currentScreen->Refresh()) {
    currentScreen.reset(nullptr);
    lvgl.SetFullRefresh(Components::LittleVgl::FullRefreshDirections::Up);
    onClockApp = false;
    switch(nextApp) {
      case Apps::None:
      case Apps::Launcher: currentScreen.reset(new Screens::ApplicationList(this, dateTimeController, settingsController)); break;
      case Apps::Clock:
        currentScreen.reset(new Screens::Clock(this, dateTimeController, batteryController, bleController, notificationManager, settingsController, stepCounter));
        onClockApp = true;
        break;
      case Apps::SysInfo: currentScreen.reset(new Screens::SystemInfo(this, dateTimeController, batteryController, brightnessController, bleController, watchdog, stepCounter)); break;
      case Apps::Paint: currentScreen.reset(new Screens::InfiniPaint(this, lvgl)); break;
      case Apps::Brightness : currentScreen.reset(new Screens::Brightness(this, brightnessController)); break;
      case Apps::Music : currentScreen.reset(new Screens::Music(this, systemTask.nimble().music())); break;
      case Apps::FirmwareValidation: currentScreen.reset(new Screens::FirmwareValidation(this, validator)); break;
      case Apps::Notifications: currentScreen.reset(new Screens::Notifications(this, notificationManager, Screens::Notifications::Modes::Normal)); break;
      case Apps::FileManager: currentScreen.reset(new Screens::FileManager(this)); break;
      case Apps::Settings: currentScreen.reset(new Screens::Settings(this, batteryController)); break;
      case Apps::Steps: currentScreen.reset(new Screens::Steps(this, stepCounter)); break;
      case Apps::HeartRate: currentScreen.reset(new Screens::HeartRate(this, hrs, settingsController, systemTask)); break;

      // To Do :-)
      case Apps::Weather: currentScreen.reset(new Screens::ScreensTemplate(this, "Weather")); break;
      case Apps::Iot: currentScreen.reset(new Screens::ScreensTemplate(this, "Iot")); break;
      case Apps::MobileApp: currentScreen.reset(new Screens::ScreensTemplate(this, "Mobile App")); break;
      case Apps::Charging: currentScreen.reset(new Screens::ScreensTemplate(this, "Charging")); break;
      case Apps::StopWatch: currentScreen.reset(new Screens::ScreensTemplate(this, "Stop Watch")); break;
    }
    nextApp = Apps::None;
  }
  lv_task_handler();
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

void DisplayApp::StartApp(Apps app) {
  nextApp = app;
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
