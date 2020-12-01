#pragma once

#include <cstdint>
#include "Screen.h"
#include <lvgl/src/lv_core/lv_style.h>
#include <lvgl/src/lv_core/lv_obj.h>


namespace Pinetime {

  namespace Applications {
    namespace Screens {

      class ScreensTemplate : public Screen{
        public:
          ScreensTemplate(DisplayApp* app, const char* name);
          ~ScreensTemplate() override;

          bool Refresh() override;
          bool OnButtonPushed() override;

        private:          

          lv_obj_t* llabel;

          bool running = true;

      };
    }
  }
}
