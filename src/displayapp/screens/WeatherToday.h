#pragma once

#include <cstdint>
#include <lvgl/lvgl.h>
#include "Screen.h"
#include "components/settings/Settings.h"

namespace Pinetime {

  namespace Applications {
    namespace Screens {

      class WeatherToday : public Screen{
        public:
          
          static char const *Icons[];
          static char const *WeatherDescription[];

          WeatherToday(DisplayApp* app, Controllers::Settings &settingsController);
          ~WeatherToday() override;

          bool OnTouchEvent(TouchEvents event) override;
          bool Refresh() override;
          void UpdateScreen();
         
        private:
          Controllers::Settings &settingsController;

          lv_obj_t* iconAnimW;
          lv_obj_t* currentTemp;
          lv_obj_t* symbTemp;
          lv_obj_t* symbNav;
          lv_obj_t* iconWDesc;
          lv_obj_t* location;
          lv_obj_t* dateInfo;
          lv_obj_t* maxmin;
          lv_task_t* taskUpdate;

          int8_t anim = 0;
          int8_t animNav = 0;
          int8_t animDelta = 2;
          int8_t animNavDelta = 1;

          Controllers::Settings::Weather weather;
          
          void DisplayWeather();

          void UpdateWeather();

      };
    }
  }
}
