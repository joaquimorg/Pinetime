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
  namespace Controllers {
    class Settings;
    class Battery;
    class Ble;
    class NotificationManager;
  }
  namespace Applications {
    namespace Screens {

      class WatchFacePong : public Screen {
        public:
          WatchFacePong(DisplayApp* app,
                  Controllers::DateTime& dateTimeController,
                  Controllers::Battery& batteryController,
                  Controllers::Ble& bleController,
                  Controllers::NotificationManager& notificatioManager,
                  Controllers::Settings &settingsController);
          
          ~WatchFacePong() override;

          bool Refresh() override;
          void pong_play();

          void UpdateScreen();

        private:
  
          uint8_t sHour, sMinute, sSecond;

          Pinetime::Controllers::DateTime::Months currentMonth = Pinetime::Controllers::DateTime::Months::Unknown;
          Pinetime::Controllers::DateTime::Days currentDayOfWeek = Pinetime::Controllers::DateTime::Days::Unknown;
          uint8_t currentDay = 0;

          DirtyValue<float> batteryPercentRemaining  {0};
          DirtyValue<bool> bleState {false};
          DirtyValue<std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds>> currentDateTime;
          DirtyValue<uint32_t> stepCount  {0};
          DirtyValue<uint8_t> heartbeat  {0};
          DirtyValue<bool> notificationState {false};
          
          lv_obj_t* time_h;
          lv_obj_t* time_m;
          lv_obj_t* ball;
          lv_obj_t* player1;
          lv_obj_t* player2;
          
          lv_obj_t* notificationIcon;

          lv_task_t* taskUpdate;
          lv_task_t* taskPlay;

          Controllers::DateTime& dateTimeController;
          Controllers::Battery& batteryController;
          Controllers::Ble& bleController;
          Controllers::NotificationManager& notificatioManager;
          Controllers::Settings& settingsController;

          uint16_t ballx = 120;
          uint16_t bally = 120;
          uint8_t dirx = 1;
          uint8_t diry = 1;
          int16_t angball = 1;

          uint16_t player1y = 120;
          uint16_t player2y = 120;

          uint8_t point = 0;
          uint16_t delaynew = 0;

          lv_point_t ball_points[2];
          lv_point_t palyer1_points[2];
          lv_point_t palyer2_points[2];

          void draw_player1();
          void draw_player2();
          void draw_ball();
          void ball_angle();

      };
    }
  }
}