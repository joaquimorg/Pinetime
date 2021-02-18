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

      class LowBatt : public Screen{
        public:
          LowBatt(DisplayApp* app, 
            Pinetime::Controllers::Battery& batteryController);
          ~LowBatt() override;

          bool Refresh() override;
          bool OnButtonPushed() override;

          void UpdateScreen();

        private:          

          Pinetime::Controllers::Battery& batteryController;

          lv_obj_t* voltage;
          lv_obj_t* percent;
          lv_task_t* taskUpdate;

          int8_t batteryPercent = -1;
          float batteryVoltage = 0.0f;

          bool running = true;

      };
    }
  }
}
