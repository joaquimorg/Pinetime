#pragma once

#include "Screen.h"
#include <lvgl/src/lv_core/lv_obj.h>
#include "systemtask/SystemTask.h"

namespace Pinetime {
  namespace Controllers {
    class Ble;
  }
  namespace Applications {
    namespace Screens {

      class FirmwareUpdate : public Screen{
        public:
          FirmwareUpdate(DisplayApp* app, Pinetime::Controllers::Ble& bleController, System::SystemTask& systemTask);
          ~FirmwareUpdate() override;

          bool Refresh() override;
          

        private:
          enum class States { Idle, Running, Validated, Error };
          Pinetime::Controllers::Ble& bleController;
          Pinetime::System::SystemTask& systemTask;
          
          lv_obj_t* bar1;
          lv_obj_t* percentLabel;
          lv_obj_t* titleLabel;

          States state;

          bool DisplayProgression() const;

          void UpdateValidated();

          void UpdateError();
      };
    }
  }
}
