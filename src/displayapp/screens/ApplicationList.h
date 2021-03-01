#pragma once

#include <memory>

#include "Screen.h"
#include "ScreenList.h"
#include "components/datetime/DateTimeController.h"
#include "components/settings/Settings.h"
#include "components/battery/BatteryController.h"

namespace Pinetime {
  namespace Applications {
    namespace Screens {
      class ApplicationList : public Screen {
        public:
          explicit ApplicationList(DisplayApp* app, 
            Pinetime::Controllers::DateTime& dateTimeController,
            Pinetime::Controllers::Settings &settingsController,
            Pinetime::Controllers::Battery& batteryController
            );
          ~ApplicationList() override;
          bool Refresh() override;
          
          bool OnTouchEvent(TouchEvents event) override;
        private:

          Controllers::DateTime& dateTimeController;
          Controllers::Settings& settingsController;
          Pinetime::Controllers::Battery& batteryController;
          ScreenList<1> screens;

          std::unique_ptr<Screen> CreateScreen1();
          //std::unique_ptr<Screen> CreateScreen2();
          //std::unique_ptr<Screen> CreateScreen3();


      };
    }
  }
}