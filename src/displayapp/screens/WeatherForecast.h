#pragma once

#include <cstdint>
#include "Screen.h"
#include <lvgl/lvgl.h>
#include "components/settings/Settings.h"

namespace Pinetime {

  namespace Applications {
    namespace Screens {

      class WeatherForecast : public Screen{
        public:
          WeatherForecast(DisplayApp* app, Controllers::Settings &settingsController);
          ~WeatherForecast() override;

          bool Refresh() override;
          void UpdateScreen();
         
        private:
          Controllers::Settings &settingsController;
          Controllers::Settings::Weather weather;

          lv_obj_t* symbNav;

          lv_obj_t* maxmin[3];
          lv_obj_t* iconW[3];
          lv_obj_t* iconWDesc[3];
          
          lv_task_t* taskUpdate;

          int8_t animNav = 0;
      };
    }
  }
}
