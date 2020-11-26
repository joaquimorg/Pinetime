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
          FileManager(DisplayApp* app, Pinetime::Drivers::FileSystem& fs);
          ~FileManager() override;

          bool Refresh() override;
          bool OnButtonPushed() override;

        private:
          Pinetime::Drivers::FileSystem& fs;

          lv_obj_t * list;
          bool running = true;

      };
    }
  }
}
