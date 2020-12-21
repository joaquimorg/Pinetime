#pragma once

#include <cstdint>
#include "Screen.h"
#include <lvgl/src/lv_core/lv_style.h>
#include <lvgl/src/lv_core/lv_obj.h>
#include "drivers/BMA421.h"

namespace Pinetime {

  namespace Drivers {
    class BMA421;
  }

  namespace Controllers {
    class Settings;
  }

  namespace Applications {
    namespace Screens {

      class Steps : public Screen{
        public:
          Steps(DisplayApp* app, Pinetime::Drivers::BMA421& stepCounter, Controllers::Settings &settingsController);
          ~Steps() override;

          bool Refresh() override;
          bool OnButtonPushed() override;

        private:          

          Pinetime::Drivers::BMA421& stepCounter;
          Controllers::Settings& settingsController;

          DirtyValue<uint32_t> stepCount  {0};

          lv_obj_t * lSteps;
          lv_obj_t * stepsArc;

          bool running = true;

      };
    }
  }
}
