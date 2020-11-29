#pragma once

#include <memory>

#include "Screen.h"
#include "ScreenList.h"

namespace Pinetime {
  namespace Applications {
    namespace Screens {
      class ApplicationList : public Screen {
        public:
          explicit ApplicationList(DisplayApp* app, 
            Pinetime::Controllers::DateTime& dateTimeController,
            Pinetime::Controllers::Settings &settingsController
            );
          ~ApplicationList() override;
          bool Refresh() override;
          bool OnButtonPushed() override;
          bool OnTouchEvent(TouchEvents event) override;
        private:

          ScreenList<3> screens;

          Controllers::DateTime& dateTimeController;
          Controllers::Settings& settingsController;

          std::unique_ptr<Screen> CreateScreen1();
          std::unique_ptr<Screen> CreateScreen2();
          std::unique_ptr<Screen> CreateScreen3();

          bool running = true;          
      };
    }
  }
}