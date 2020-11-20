#pragma once

#include <cstdint>
#include <chrono>

#include "Screen.h"
#include "ScreenList.h"
#include <bits/unique_ptr.h>
#include <libs/lvgl/src/lv_core/lv_style.h>
#include <libs/lvgl/src/lv_core/lv_obj.h>
#include "components/ble/NotificationManager.h"
#include "components/battery/BatteryController.h"
#include "components/ble/BleController.h"

namespace Pinetime {
  namespace Applications {
    namespace Screens {
      class Clock : public Screen{
        public:
          Clock(DisplayApp* app,
                  Controllers::DateTime& dateTimeController,
                  Controllers::Battery& batteryController,
                  Controllers::Ble& bleController,
                  Controllers::NotificationManager& notificatioManager);
          ~Clock() override;

          bool Refresh() override;
          bool OnButtonPushed() override;

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
          DirtyValue<uint8_t> heartbeat  {0};
          DirtyValue<bool> notificationState {false};

          lv_style_t ble_style;
          
          lv_obj_t* label_time;
          lv_obj_t* label_time_sec;
          lv_obj_t* label_date;
          lv_obj_t* label_date_day;
          lv_obj_t* backgroundLabel;
          lv_obj_t* batteryIcon;
          lv_obj_t* bleIcon;
          lv_obj_t* batteryPlug;
          lv_obj_t* heartbeatIcon;
          lv_obj_t* heartbeatValue;
          lv_obj_t* heartbeatBpm;
          lv_obj_t* stepIcon;
          lv_obj_t* stepValue;
          lv_obj_t* notificationIcon;

          Controllers::DateTime& dateTimeController;
          Controllers::Battery& batteryController;
          Controllers::Ble& bleController;
          Controllers::NotificationManager& notificatioManager;

          ScreenList<1> screens;
          std::unique_ptr<Screen> WatchFaceDigital();

          bool running = true;

      };
    }
  }
}
