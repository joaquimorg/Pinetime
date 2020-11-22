#include "Clock.h"

#include <date/date.h>
#include <lvgl/lvgl.h>
#include <cstdio>
#include "BatteryIcon.h"
#include "BleIcon.h"
#include "NotificationIcon.h"
#include "Symbols.h"
#include "components/battery/BatteryController.h"
#include "components/ble/BleController.h"
#include "components/ble/NotificationManager.h"
#include "../DisplayApp.h"
#include "WatchFaceDigital.h"
#include "WatchFaceAnalog.h"
#include "WatchFaceMinimal.h"

using namespace Pinetime::Applications::Screens;

Clock::Clock(DisplayApp* app,
        Controllers::DateTime& dateTimeController,
        Controllers::Battery& batteryController,
        Controllers::Ble& bleController,
        Controllers::NotificationManager& notificatioManager) : Screen(app),
                                           dateTimeController{dateTimeController}, batteryController{batteryController},
                                           bleController{bleController}, notificatioManager{notificatioManager},
        screens{app, {
                [this]() -> std::unique_ptr<Screen> { return WatchFaceDigitalScreen(); },
                [this]() -> std::unique_ptr<Screen> { return WatchFaceAnalogScreen(); },
                [this]() -> std::unique_ptr<Screen> { return WatchFaceMinimalScreen(); }
          },
          Screens::ScreenListModes::LongPress,
          1
        } {}

Clock::~Clock() {
  lv_obj_clean(lv_scr_act());
}


bool Clock::Refresh() {  
  screens.Refresh();  
  return running;
}

bool Clock::OnButtonPushed() {
  running = false;
  return false;
}

bool Clock::OnTouchEvent(Pinetime::Applications::TouchEvents event) {
  return screens.OnTouchEvent(event);
}

std::unique_ptr<Screen> Clock::WatchFaceDigitalScreen() {  
  return std::unique_ptr<Screen>(new Screens::WatchFaceDigital(app, dateTimeController, batteryController, bleController, notificatioManager));
}

std::unique_ptr<Screen> Clock::WatchFaceAnalogScreen() {  
  return std::unique_ptr<Screen>(new Screens::WatchFaceAnalog(app, dateTimeController, batteryController, bleController, notificatioManager));
}

std::unique_ptr<Screen> Clock::WatchFaceMinimalScreen() {  
  return std::unique_ptr<Screen>(new Screens::WatchFaceMinimal(app, dateTimeController, batteryController, bleController, notificatioManager));
}