#pragma once

#include <cstdint>
#include <lvgl/lvgl.h>
#include "Screen.h"

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
          void UpdateScreen();

        private:          

          Pinetime::Controllers::Accelerometer& accelerometer;
          Controllers::Settings& settingsController;

          DirtyValue<uint32_t> stepCount  {0};

          lv_obj_t * lSteps;
          lv_obj_t * stepsArc;
          lv_obj_t * steps_icon;

          lv_task_t* taskUpdate;
          uint8_t currentImage = 1;

      };
    }
  }
}
