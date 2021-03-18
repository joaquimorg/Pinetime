#pragma once

#include <cstdint>
#include <lvgl/lvgl.h>
#include "Screen.h"

namespace Pinetime {

  namespace Applications {
    namespace Screens {

      class ResourceUpdate : public Screen{
        public:
          ResourceUpdate(DisplayApp* app);
          ~ResourceUpdate() override;

          bool Refresh() override;
         
        private:          

          

      };
    }
  }
}
