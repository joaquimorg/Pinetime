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
    class Accelerometer;
  }
  namespace Applications {
    namespace Screens {

      class WatchFaceDigital : public Screen {
        public:
          WatchFaceDigital(DisplayApp* app,
                  Controllers::DateTime& dateTimeController,
                  Controllers::Battery& batteryController,
                  Controllers::Ble& bleController,
                  Controllers::NotificationManager& notificatioManager,
                  Controllers::Settings &settingsController,
                  Controllers::Accelerometer& accelerometer);
          
          ~WatchFaceDigital() override;

          bool Refresh() override;
          void UpdateScreen();

        private:
          uint8_t sHour, sMinute, sSecond;

          uint16_t currentYear = 1970;
          Pinetime::Controllers::DateTime::Months currentMonth = Pinetime::Controllers::DateTime::Months::Unknown;
          Pinetime::Controllers::DateTime::Days currentDayOfWeek = Pinetime::Controllers::DateTime::Days::Unknown;
          uint8_t currentDay = 0;

          lv_style_t stepc_style;

          lv_style_t infoc_style;
          lv_style_t powerc_style;
          lv_style_t heartc_style;
          lv_style_t timec_style;
          lv_style_t datec_style;
          lv_style_t time_style;
          lv_style_t seconds_style;
          lv_style_t seconds_line_style;
          lv_style_t dateyear_style;
          lv_style_t dateday_style;
          lv_style_t datemonth_style;
          lv_style_t date_style;
          
          lv_obj_t* label_time;
          lv_obj_t* label_time_am_pm;
          lv_obj_t* label_time_sec;
          lv_obj_t* label_date;
          lv_obj_t* label_date_month;
          lv_obj_t* label_date_year;
          lv_obj_t* label_date_day;
          lv_obj_t* batteryIcon;
          lv_obj_t* batteryValue;
          lv_obj_t* bleIcon;
          lv_obj_t* batteryPlug;
          lv_obj_t* heartbeatIcon;
          lv_obj_t* heartbeatValue;
          lv_obj_t* stepIcon;
          lv_obj_t* stepValue;
          lv_obj_t* notificationIcon;
          lv_obj_t* seconds_body;

          lv_task_t* taskUpdate;
          
          lv_point_t seconds_point[2];

          Controllers::DateTime& dateTimeController;
          Controllers::Battery& batteryController;
          Controllers::Ble& bleController;
          Controllers::NotificationManager& notificatioManager;
          Controllers::Settings& settingsController;
          Controllers::Accelerometer& accelerometer;
      };
    }
  }
}