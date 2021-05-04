#pragma once

#include <cstdint>
#include "Screen.h"
#include <lvgl/lvgl.h>
#include <littlefs/lfs.h>
#include "components/fs/FS.h"

namespace Pinetime {

  namespace Applications {
    namespace Screens {

      class FileExplorer : public Screen{
        public:
          FileExplorer(DisplayApp* app, Controllers::FS &fs);
          ~FileExplorer() override;

          bool Refresh() override;
          bool OnTouchEvent(Pinetime::Applications::TouchEvents event) override;
         
        private:          
          Controllers::FS &fs;

          lfs_dir_t mLfs_dir;

      };
    }
  }
}
