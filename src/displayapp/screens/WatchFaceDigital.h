#pragma once

#include <vector>
#include "Screen.h"
#include <lvgl/lvgl.h>
#include "components/ble/NotificationManager.h"
#include "components/battery/BatteryController.h"
#include "components/ble/BleController.h"
#include "components/datetime/DateTimeController.h"

namespace Pinetime {
  namespace Applications {
    namespace Screens {

      class WatchFaceDigital : public Screen {
        public:
          WatchFaceDigital(DisplayApp* app,
                  Controllers::DateTime& dateTimeController,
                  Controllers::Battery& batteryController,
                  Controllers::Ble& bleController,
                  Controllers::NotificationManager& notificatioManager);
          ~WatchFaceDigital() override;
          bool Refresh() override {return false;}

        private:
          lv_obj_t * label = nullptr;
      };
    }
  }
}