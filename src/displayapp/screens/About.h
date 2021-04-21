#pragma once

#include <memory>
#include "Screen.h"
#include "ScreenList.h"

namespace Pinetime {

  namespace Applications {
    
    namespace Screens {
      class About : public Screen {
        public:
          explicit About(DisplayApp* app,
                              Pinetime::Controllers::DateTime& dateTimeController,
                              Pinetime::Controllers::Battery& batteryController,
                              Pinetime::Controllers::BrightnessController& brightnessController,
                              Pinetime::Controllers::Ble& bleController,
                              Pinetime::Drivers::WatchdogView& watchdog,
                              Pinetime::Controllers::Accelerometer& accelerometer,
                              Pinetime::Controllers::Settings& settingsController);
          ~About() override;
          bool Refresh() override;
          
          bool OnTouchEvent(TouchEvents event) override;
        private:
          bool running = true;
          
          lv_obj_t* container1;

          Pinetime::Controllers::DateTime& dateTimeController;
          Pinetime::Controllers::Battery& batteryController;
          Pinetime::Controllers::BrightnessController& brightnessController;
          Pinetime::Controllers::Ble& bleController;
          Pinetime::Drivers::WatchdogView& watchdog;
          Pinetime::Controllers::Accelerometer& accelerometer;
          Pinetime::Controllers::Settings& settingsController;

          ScreenList<5> screens;
          std::unique_ptr<Screen> CreateScreen1();
          std::unique_ptr<Screen> CreateScreen2();
          std::unique_ptr<Screen> CreateScreen3();
          std::unique_ptr<Screen> CreateScreen4();
          std::unique_ptr<Screen> CreateScreen5();

          void CreateContainer();
      };
    }
  }
}