#pragma once

#include <cstdint>
#include <FreeRTOS.h>
#include <timers.h>
#include "Screen.h"
#include <lvgl/src/lv_core/lv_style.h>
#include <lvgl/src/lv_core/lv_obj.h>
#include "systemtask/SystemTask.h"
#include "hrs3300/hrs3300.h"


namespace Pinetime {

  namespace Drivers {
    class HRS3300;
  }
  namespace Controllers {
    class Settings;
  }

  namespace Applications {
    namespace Screens {

      class HeartRate : public Screen{
        public:
          HeartRate(
            DisplayApp* app, 
            Drivers::HRS3300 &hrs,
            Controllers::Settings &settingsController,
            System::SystemTask &systemTask);
          ~HeartRate() override;

          bool Refresh() override;
          bool OnButtonPushed() override;
          void EndHRReading();

        private:          
          Pinetime::Drivers::HRS3300& hrs;
          Pinetime::Controllers::Settings& settingsController;
          Pinetime::System::SystemTask& systemTask;

          lv_obj_t* llabel;
          lv_obj_t* lhistory;
          lv_obj_t* lhrs;
          lv_obj_t* chart;

          uint8_t hrv = 0;
          uint8_t heartRateReading = 0;
          uint8_t heartRate = 0;
          uint8_t bpLow = 0;
          uint8_t bpHigh = 0;

          TimerHandle_t hrTimer;

          bool running = true;

          void UpdateGraph();

      };
    }
  }
}
