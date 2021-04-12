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
#include "WatchFacePong.h"
#include "WatchFaceCustom.h"

using namespace Pinetime::Applications::Screens;

Clock::Clock(DisplayApp* app,
        Controllers::DateTime& dateTimeController,
        Controllers::Battery& batteryController,
        Controllers::Ble& bleController,
        Controllers::NotificationManager& notificatioManager,
        Controllers::Settings &settingsController,
        Controllers::Accelerometer& accelerometer) : Screen(app),
        dateTimeController{dateTimeController}, batteryController{batteryController},
        bleController{bleController}, notificatioManager{notificatioManager},
        settingsController{settingsController},
        accelerometer{accelerometer},
        screens{app, 
          settingsController.GetClockFace(),
          {
                [this]() -> std::unique_ptr<Screen> { return WatchFaceDigitalScreen(); },
                [this]() -> std::unique_ptr<Screen> { return WatchFaceAnalogScreen1(); },
                [this]() -> std::unique_ptr<Screen> { return WatchFaceAnalogScreen2(); },
                [this]() -> std::unique_ptr<Screen> { return WatchFaceMinimalScreen(); },
                [this]() -> std::unique_ptr<Screen> { return WatchFacePongScreen(); },
                [this]() -> std::unique_ptr<Screen> { return WatchFaceCustomScreen1(); },
                [this]() -> std::unique_ptr<Screen> { return WatchFaceCustomScreen2(); },
                [this]() -> std::unique_ptr<Screen> { return WatchFaceCustomScreen3(); },
          },
          Screens::ScreenListModes::LongPress          
        } {

          settingsController.SetAppMenu(0);

        }

Clock::~Clock() {
  lv_obj_clean(lv_scr_act());
}


bool Clock::Refresh() {  
  screens.Refresh();  
  return running;
}

bool Clock::OnTouchEvent(Pinetime::Applications::TouchEvents event) {
  return screens.OnTouchEvent(event);
}

std::unique_ptr<Screen> Clock::WatchFaceDigitalScreen() {  
  return std::make_unique<Screens::WatchFaceDigital>(app, dateTimeController, batteryController, bleController, notificatioManager, settingsController, accelerometer);
}

std::unique_ptr<Screen> Clock::WatchFaceAnalogScreen1() {  
  return std::make_unique<Screens::WatchFaceAnalog>(0, app, dateTimeController, batteryController, bleController, notificatioManager, settingsController);
}

std::unique_ptr<Screen> Clock::WatchFaceAnalogScreen2() {  
  return std::make_unique<Screens::WatchFaceAnalog>(1, app, dateTimeController, batteryController, bleController, notificatioManager, settingsController);
}

std::unique_ptr<Screen> Clock::WatchFaceMinimalScreen() {  
  return std::make_unique<Screens::WatchFaceMinimal>(app, dateTimeController, batteryController, bleController, notificatioManager, settingsController);
}

std::unique_ptr<Screen> Clock::WatchFacePongScreen() {  
  return std::make_unique<Screens::WatchFacePong>(app, dateTimeController, batteryController, bleController, notificatioManager, settingsController);
}

std::unique_ptr<Screen> Clock::WatchFaceCustomScreen1() {  
  return std::make_unique<Screens::WatchFaceCustom>(0, app, dateTimeController, batteryController, bleController, notificatioManager, settingsController);
}

std::unique_ptr<Screen> Clock::WatchFaceCustomScreen2() {  
  return std::make_unique<Screens::WatchFaceCustom>(1, app, dateTimeController, batteryController, bleController, notificatioManager, settingsController);
}

std::unique_ptr<Screen> Clock::WatchFaceCustomScreen3() {  
  return std::make_unique<Screens::WatchFaceCustom>(2, app, dateTimeController, batteryController, bleController, notificatioManager, settingsController);
}