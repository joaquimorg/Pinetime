#pragma once

#include <cstdint>
#include "Screen.h"
#include <lvgl/src/lv_core/lv_style.h>
#include <lvgl/src/lv_core/lv_obj.h>
#include "hrs3300/hrs3300.h"


namespace Pinetime {

  namespace Drivers {
    class HRS3300;
  }
  namespace Controllers {
    class Settings;
  }

  namespace Applications {
    namespace Screens {

      class HeartRate : public Screen{
        public:
          HeartRate(
            DisplayApp* app, 
            Drivers::HRS3300 &hrs,
            Controllers::Settings &settingsController);
          ~HeartRate() override;

          bool Refresh() override;
          bool OnButtonPushed() override;

        private:          
          Pinetime::Drivers::HRS3300& hrs;
          Pinetime::Controllers::Settings& settingsController;

          lv_obj_t* llabel;
          lv_obj_t* lhrs;

          hrs3300_results_t heartRate;

          bool running = true;

      };
    }
  }
}
