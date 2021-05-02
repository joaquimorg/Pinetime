#pragma once

#include <chrono>
#include <cstdint>
#include <memory>
#include <lvgl/lvgl.h>
#include "Screen.h"
#include "ScreenList.h"
#include "components/datetime/DateTimeController.h"
#include "components/battery/BatteryController.h"
#include "components/ble/BleController.h"
#include "components/ble/NotificationManager.h"

namespace Pinetime {
  namespace Controllers {
    class Settings;
    class Battery;
    class Ble;
    class NotificationManager;
  }
  namespace Applications {
    namespace Screens {

      class WatchFaceWeather : public Screen {
        public:
          WatchFaceWeather(DisplayApp* app,
                  Controllers::DateTime& dateTimeController,
                  Controllers::Battery& batteryController,
                  Controllers::Settings &settingsController);

          ~WatchFaceWeather() override;

          bool Refresh() override;
          void UpdateScreen();

        private:

          static char const *Numbers[];
          static char const *NumbersS[];
          static char const *NumbersSS[];
          static char const *WeekDay[];

          uint8_t sHour, sMinute, sSecond;
          uint8_t hour, minute, second, weatherIcon, sWeatherIcon;

          Pinetime::Controllers::DateTime::Days currentDayOfWeek = Pinetime::Controllers::DateTime::Days::Unknown;
          uint8_t currentDay = 0;

          uint8_t day;
          Pinetime::Controllers::DateTime::Days dayOfWeek;

          lv_task_t* taskUpdate;

          lv_obj_t* image[10];
          //lv_obj_t* batteryIcon;
          //lv_obj_t* label_day;

          Controllers::DateTime& dateTimeController;
          Controllers::Battery& batteryController;
          Controllers::Settings& settingsController;

      };
    }
  }
}