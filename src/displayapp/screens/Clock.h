#pragma once

#include <lvgl/src/lv_core/lv_obj.h>
#include <chrono>
#include <cstdint>
#include <memory>
#include "Screen.h"
#include "ScreenList.h"
#include "components/datetime/DateTimeController.h"

namespace Pinetime {

  namespace Controllers {
    class Settings;
    class Battery;
    class Ble;
    class NotificationManager;
    class Accelerometer;
  }

  namespace Applications {
    namespace Screens {
      class Clock : public Screen {
        public:
          Clock(DisplayApp* app,
                  Controllers::DateTime& dateTimeController,
                  Controllers::Battery& batteryController,
                  Controllers::Ble& bleController,
                  Controllers::NotificationManager& notificatioManager,
                  Controllers::Settings &settingsController,
                  Controllers::Accelerometer& accelerometer);
          ~Clock() override;

          bool Refresh() override;
          
          bool OnTouchEvent(TouchEvents event) override;

        private:

          Controllers::DateTime& dateTimeController;
          Controllers::Battery& batteryController;
          Controllers::Ble& bleController;
          Controllers::NotificationManager& notificatioManager;
          Controllers::Settings& settingsController;
          Controllers::Accelerometer& accelerometer;

          ScreenList<9> screens;
          std::unique_ptr<Screen> WatchFaceDigitalScreen();
          std::unique_ptr<Screen> WatchFaceAnalogScreen1();
          std::unique_ptr<Screen> WatchFaceAnalogScreen2();
          std::unique_ptr<Screen> WatchFaceMinimalScreen();
          std::unique_ptr<Screen> WatchFacePongScreen();
          std::unique_ptr<Screen> WatchFaceCustomScreen1();
          std::unique_ptr<Screen> WatchFaceCustomScreen2();
          std::unique_ptr<Screen> WatchFaceCustomScreen3();
          std::unique_ptr<Screen> WatchFaceWeather();


      };
    }
  }
}
