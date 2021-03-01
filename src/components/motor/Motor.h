#pragma once

#include <cstdint>
#include <FreeRTOS.h>
#include "timers.h"
#include "components/settings/Settings.h"

namespace Pinetime {
  namespace Controllers {

    class Motor {
      public:
        Motor( Controllers::Settings &settingsController );
        void Init();
        void Vibrate(uint8_t durationMs);

        void EndVibration();

      private:

        Controllers::Settings& settingsController;

        TimerHandle_t vrTimer;

    };
  }
}
