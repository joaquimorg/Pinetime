#pragma once

#include <lvgl/lvgl.h>
#include <cstdint>
#include <memory>
#include "Modal.h"
#include "Screen.h"
#include "../Apps.h"
#include "components/datetime/DateTimeController.h"
#include "components/settings/Settings.h"

namespace Pinetime {
  namespace Applications {
    namespace Screens {
      class Tile : public Screen {
        public:
          struct Applications {
            const void * icon;
            const char* name;
            Pinetime::Applications::Apps application;
          };

          explicit Tile(uint8_t screenID, uint8_t numScreens,
              DisplayApp* app, 
              Controllers::DateTime& dateTimeController,
              Controllers::Settings& settingsController, 
              std::array<Applications, 4>& applications);
          ~Tile() override;

          bool Refresh() override;
          bool OnButtonPushed() override;

          void OnObjectEvent(lv_obj_t* obj, lv_event_t event, uint32_t buttonId);

        private:
          lv_obj_t * backgroundLabel;
          lv_obj_t * label_time;
          bool running = true;
          
          uint8_t oldHours = 0;
          uint8_t oldMinutes = 0;

          //std::unique_ptr<Modal> modal;

          lv_obj_t * iconsApps[4];
          Pinetime::Applications::Apps apps[4];

          Controllers::DateTime& dateTimeController;
          Controllers::Settings& settingsController;
      };
    }
  }
}
