#pragma once

#include <cstdint>
#include "Screen.h"
#include <lvgl/src/lv_core/lv_style.h>
#include <lvgl/src/lv_core/lv_obj.h>
#include "bma421/bma4.h"

namespace Pinetime {

  namespace Drivers {
    class WatchdogView;
    class BMA421;
  }
  namespace Controllers {
    class Battery;
    class BrightnessController;
  }

  namespace Applications {
    namespace Screens {

      class Settings : public Screen{
        public:
          Settings(DisplayApp* app, Controllers::Battery& batteryController);
          ~Settings() override;

          bool Refresh() override;
          bool OnButtonPushed() override;
          void OnButtonEvent(lv_obj_t *object, lv_event_t event);

        private:          

          lv_obj_t* llabel;
          lv_obj_t* buttonPwrOff;

          bool running = true;

          Controllers::Battery& batteryController;

      };
    }
  }
}
