#pragma once

#include <cstdint>
#include "Screen.h"
#include <lvgl/src/lv_core/lv_style.h>
#include <lvgl/src/lv_core/lv_obj.h>
#include "bma421/bma4.h"

namespace Pinetime {

  namespace Applications {
    namespace Screens {

      class Settings : public Screen{
        public:
          Settings(DisplayApp* app);
          ~Settings() override;

          bool Refresh() override;
          bool OnButtonPushed() override;

        private:          

          lv_obj_t * llabel;

          bool running = true;

      };
    }
  }
}
