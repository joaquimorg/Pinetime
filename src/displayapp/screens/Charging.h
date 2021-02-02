#pragma once

#include <cstdint>
#include <FreeRTOS.h>
#include <timers.h>
#include "Screen.h"
#include <lvgl/lvgl.h>


namespace Pinetime {
  namespace Controllers {
    class Battery;
  }

  namespace Applications {
    namespace Screens {

      class Charging : public Screen{
        public:
          Charging(DisplayApp* app, 
            Pinetime::Controllers::Battery& batteryController);
          ~Charging() override;

          bool Refresh() override;
          bool OnButtonPushed() override;

          void Reading();

        private:          

          Pinetime::Controllers::Battery& batteryController;

          lv_obj_t* llabel;
          lv_obj_t* charging_bar;

          uint8_t animation = 0;
          uint8_t batteryPercent = 0;
          float batteryVoltage = 0.0f;
          uint8_t readc = 0, old_readc = 0;;

          bool running = true;
          TimerHandle_t chTimer;

      };
    }
  }
}
