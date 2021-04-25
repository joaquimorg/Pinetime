#pragma once

#include <cstdint>
#include <lvgl/lvgl.h>
#include "Screen.h"

namespace Pinetime {

  namespace Applications {
    namespace Screens {

      class ShowMessage : public Screen{
        public:
          ShowMessage(DisplayApp* app, const char* message, const void * src_img);
          ~ShowMessage() override;

          bool Refresh() override;
         
        private:          

          lv_obj_t* iconAnim;
          lv_obj_t* info;

      };
    }
  }
}
