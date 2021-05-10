#pragma once

#include <cstdint>
#include <lvgl/lvgl.h>
#include "components/settings/Settings.h"
#include "displayapp/screens/Screen.h"
#include "displayapp/screens/ScreenList.h"

namespace Pinetime {

  namespace Applications {
    namespace Screens {

      class SettingWatchFace : public Screen{
        public:
          SettingWatchFace(DisplayApp* app, Pinetime::Controllers::Settings &settingsController);
          ~SettingWatchFace() override;

          bool Refresh() override;
          bool OnTouchEvent(TouchEvents event) override;
          void UpdateSelected(lv_obj_t *object, lv_event_t event);
         
        private:          
          
          static constexpr uint8_t wfTotal = 9;

          Controllers::Settings& settingsController;
          ScreenList<wfTotal> screens;
          uint8_t selectedFace = 0;
          std::unique_ptr<Screen> CreateScreen(const char * imageName, uint8_t num);
          
      };
    }
  }
}
