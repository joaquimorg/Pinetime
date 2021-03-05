#pragma once

#include <cstdint>
#include <lvgl/lvgl.h>
#include "components/settings/Settings.h"
#include "Screen.h"

namespace Pinetime {

  namespace Applications {
    namespace Screens {

      class SettingTimeFormat : public Screen{
        public:
          SettingTimeFormat(DisplayApp* app, Pinetime::Controllers::Settings &settingsController);
          ~SettingTimeFormat() override;

          bool Refresh() override;
          void UpdateSelected(lv_obj_t *object, lv_event_t event);
         
        private:          

          Controllers::Settings& settingsController;

          lv_obj_t * cbTimeOut[4];
          
      };
    }
  }
}
