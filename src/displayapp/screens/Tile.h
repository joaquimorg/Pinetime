#pragma once

#include <lvgl/lvgl.h>
#include <cstdint>
#include <memory>
#include "Screen.h"
#include "../Apps.h"
#include "components/datetime/DateTimeController.h"
#include "components/settings/Settings.h"
#include "components/datetime/DateTimeController.h"
#include "components/battery/BatteryController.h"

namespace Pinetime {
  namespace Applications {
    namespace Screens {
      class Tile : public Screen {
        public:
          struct Applications {
            const char* icon;
            const char* name;
            Pinetime::Applications::Apps application;
          };

          explicit Tile(uint8_t screenID, uint8_t numScreens,
              DisplayApp* app, 
              Controllers::DateTime& dateTimeController,
              Controllers::Settings& settingsController, 
              Pinetime::Controllers::Battery& batteryController,
              std::array<Applications, 4>& applications);
          ~Tile() override;

          bool Refresh() override;
          void UpdateScreen();
                    
          void OnObjectEvent(lv_obj_t* obj, lv_event_t event);

        private:

          Controllers::DateTime& dateTimeController;
          Controllers::Settings& settingsController;
          Pinetime::Controllers::Battery& batteryController;
          Pinetime::Applications::Apps apps[4];

          lv_task_t* taskUpdate;

          lv_obj_t * label_time;
          lv_obj_t * batteryIcon;
          
          uint8_t oldHours = 0;
          uint8_t oldMinutes = 0;
          uint8_t hours;
          uint8_t minutes;
          int8_t batteryPercent = -1;
          

          lv_obj_t * iconsApps[4];
          lv_obj_t * iconsAppsLabel[4];

          lv_point_t pageIndicatorBasePoints[2];
          lv_point_t pageIndicatorPoints[2];
          lv_obj_t* pageIndicatorBase;
          lv_obj_t* pageIndicator;
         
      };
    }
  }
}
