#pragma once

#include <lvgl/lvgl.h>
#include <cstdint>
#include <memory>
#include "Screen.h"
#include "../Apps.h"
#include "components/datetime/DateTimeController.h"
#include "components/settings/Settings.h"

#define MAXLISTITEMS 5

namespace Pinetime {
  namespace Applications {
    namespace Screens {
      class List : public Screen {
        public:
          struct Applications {
            const char* icon;
            const char* name;
            Pinetime::Applications::Apps application;
          };

          explicit List(uint8_t screenID, uint8_t numScreens,
              DisplayApp* app, 
              Controllers::DateTime& dateTimeController,
              Controllers::Settings& settingsController, 
              std::array<Applications, MAXLISTITEMS>& applications);
          ~List() override;

          bool Refresh() override;
          bool OnButtonPushed() override;

          void OnButtonEvent(lv_obj_t *object, lv_event_t event);

        private:

          Controllers::DateTime& dateTimeController;
          Controllers::Settings& settingsController;
          Pinetime::Applications::Apps apps[MAXLISTITEMS];

          DirtyValue<std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds>> currentDateTime;

          lv_obj_t * label_time;
          bool running = true;
          
          uint8_t oldHours = 0;
          uint8_t oldMinutes = 0;
          uint8_t hours;
          uint8_t minutes;


          lv_obj_t * itemApps[MAXLISTITEMS];

          lv_point_t pageIndicatorBasePoints[2];
          lv_point_t pageIndicatorPoints[2];
          lv_obj_t* pageIndicatorBase;
          lv_obj_t* pageIndicator;
         
      };
    }
  }
}
