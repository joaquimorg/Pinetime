#pragma once

#include <cstdint>
#include <chrono>

#include "Screen.h"
#include "ScreenList.h"
#include <bits/unique_ptr.h>
#include "../DisplayApp.h"

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
          bool OnTouchEvent(TouchEvents event) override;

        private:

          Controllers::DateTime& dateTimeController;
          Controllers::Battery& batteryController;
          Controllers::Ble& bleController;
          Controllers::NotificationManager& notificatioManager;

          ScreenList<1> screens;
          std::unique_ptr<Screen> WatchFaceDigitalScreen();

          bool running = true;

      };
    }
  }
}
