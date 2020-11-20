#pragma once

#include <memory>

#include "Screen.h"
#include "ScreenList.h"

namespace Pinetime {
  namespace Applications {
    namespace Screens {
      class ApplicationList : public Screen {
        public:
          explicit ApplicationList(DisplayApp* app, Pinetime::Controllers::DateTime& dateTimeController);
          ~ApplicationList() override;
          bool Refresh() override;
          bool OnButtonPushed() override;
          bool OnTouchEvent(TouchEvents event) override;
        private:
          bool running = true;
          Controllers::DateTime& dateTimeController;

          ScreenList<2> screens;
          std::unique_ptr<Screen> CreateScreen1();
          std::unique_ptr<Screen> CreateScreen2();
          //std::unique_ptr<Screen> CreateScreen3();
      };
    }
  }
}