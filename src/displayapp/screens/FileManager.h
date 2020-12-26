#pragma once

#include <cstdint>
#include "Screen.h"
#include <lvgl/lvgl.h>
#include "drivers/FileSystem.h"

namespace Pinetime {
  namespace Drivers {
    //class FileSystem;
  }
  namespace Applications {
    namespace Screens {

      class FileManager : public Screen{
        public:
          FileManager(DisplayApp* app);
          ~FileManager() override;

          bool Refresh() override;
          bool OnButtonPushed() override;
          bool OnTouchEvent(TouchEvents event) override;

        private:
          //Pinetime::Drivers::FileSystem fs;

          lv_style_t style_halo;
          bool running = true;

      };
    }
  }
}
