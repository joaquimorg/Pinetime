#pragma once

#include <lvgl/src/lv_core/lv_obj.h>
#include <chrono>
#include <cstdint>
#include <memory>
#include "Screen.h"
#include "ScreenList.h"
#include "components/datetime/DateTimeController.h"
#include "components/battery/BatteryController.h"
#include "components/ble/BleController.h"
#include "components/ble/NotificationManager.h"

namespace Pinetime {
  namespace Drivers {
    class BMA421;
  }
  namespace Controllers {
    class Settings;
    class Battery;
    class Ble;
    class NotificationManager;
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
                  Drivers::BMA421& stepCounter);
          
          ~WatchFaceDigital() override;

          bool Refresh() override;
          void UpdateScreen();
          
        private:
          uint8_t sHour, sMinute, sSecond;

          uint16_t currentYear = 1970;
          Pinetime::Controllers::DateTime::Months currentMonth = Pinetime::Controllers::DateTime::Months::Unknown;
          Pinetime::Controllers::DateTime::Days currentDayOfWeek = Pinetime::Controllers::DateTime::Days::Unknown;
          uint8_t currentDay = 0;

          DirtyValue<float> batteryPercentRemaining  {0};
          DirtyValue<bool> bleState {false};
          DirtyValue<std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds>> currentDateTime;
          DirtyValue<uint32_t> stepCount  {0};
          //DirtyValue<uint8_t> heartbeat  {0};
          DirtyValue<bool> notificationState {false};

          lv_style_t ble_style;

          lv_style_t step_style;
          lv_style_t stepc_style;

          lv_style_t infoc_style;
          lv_style_t powerc_style;
          lv_style_t heartc_style;
          lv_style_t timec_style;
          lv_style_t datec_style;
          //lv_style_t div_line_style;
          lv_style_t batt_style;
          lv_style_t not_style;
          lv_style_t time_style;
          lv_style_t seconds_style;
          lv_style_t seconds_line_style;
          lv_style_t dateyear_style;
          lv_style_t dateday_style;
          lv_style_t datemonth_style;
          lv_style_t date_style;
          lv_style_t heart_style;
          
          lv_obj_t* label_time;
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

          lv_point_t seconds_point[2];

          lv_task_t* taskUpdate;

          Controllers::DateTime& dateTimeController;
          Controllers::Battery& batteryController;
          Controllers::Ble& bleController;
          Controllers::NotificationManager& notificatioManager;
          Controllers::Settings& settingsController;
          Pinetime::Drivers::BMA421& stepCounter;
      };
    }
  }
}