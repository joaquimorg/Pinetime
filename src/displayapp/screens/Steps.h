#pragma once

#include <cstdint>
#include "Screen.h"
#include <lvgl/src/lv_core/lv_style.h>
#include <lvgl/src/lv_core/lv_obj.h>

namespace Pinetime {

  namespace Controllers {
    class Settings;
    class Accelerometer;
  }

  namespace Applications {
    namespace Screens {

      class Steps : public Screen {
        public:
          Steps(DisplayApp* app, Pinetime::Controllers::Accelerometer& accelerometer, Controllers::Settings &settingsController);
          ~Steps() override;

          bool Refresh() override;
          

        private:          

          Pinetime::Controllers::Accelerometer& accelerometer;
          Controllers::Settings& settingsController;

          DirtyValue<uint32_t> stepCount  {0};

          lv_obj_t * lSteps;
          lv_obj_t * stepsArc;


      };
    }
  }
}
