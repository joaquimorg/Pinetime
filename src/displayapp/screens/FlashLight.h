#pragma once

#include <cstdint>
#include "Screen.h"
#include <lvgl/lvgl.h>
#include "systemtask/SystemTask.h"
#include "components/brightness/BrightnessController.h"


namespace Pinetime {
 
  namespace Applications {
    namespace Screens {

      class FlashLight : public Screen{
        public:
          FlashLight(DisplayApp* app, System::SystemTask &systemTask, Controllers::BrightnessController& brightness);
          ~FlashLight() override;

          bool Refresh() override;
          
          bool OnTouchEvent(Pinetime::Applications::TouchEvents event) override;

        private:
          Pinetime::System::SystemTask& systemTask;
          Controllers::BrightnessController& brightness;

      };
    }
  }
}
