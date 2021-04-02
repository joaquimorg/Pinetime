#pragma once

#include <cstdint>
#include <lvgl/lvgl.h>
#include "Screen.h"

namespace Pinetime {

  namespace Applications {
    namespace Screens {

      class Weather : public Screen{
        public:
          Weather(DisplayApp* app);
          ~Weather() override;

          bool Refresh() override;
         
        private:
          
          lv_obj_t* llabel;

      };
    }
  }
}
