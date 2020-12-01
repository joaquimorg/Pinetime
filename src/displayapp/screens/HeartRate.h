#pragma once

#include <cstdint>
#include "Screen.h"
#include <lvgl/src/lv_core/lv_style.h>
#include <lvgl/src/lv_core/lv_obj.h>


namespace Pinetime {

  namespace Drivers {
    class HRS3300;
  }

  namespace Applications {
    namespace Screens {

      class HeartRate : public Screen{
        public:
          HeartRate(DisplayApp* app, Drivers::HRS3300& hrs);
          ~HeartRate() override;

          bool Refresh() override;
          bool OnButtonPushed() override;

        private:          
          Pinetime::Drivers::HRS3300& hrs;

          lv_obj_t* llabel;
          lv_obj_t* lhrs;

          bool running = true;

      };
    }
  }
}
