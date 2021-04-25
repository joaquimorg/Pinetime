#pragma once

#include <cstdint>
#include <memory>
#include "Screen.h"
#include "ScreenList.h"

namespace Pinetime {

  namespace Applications {
    namespace Screens {

      class Weather : public Screen{
        public:
          Weather(DisplayApp* app, Controllers::Settings &settingsController);
          ~Weather() override;

          bool Refresh() override;
          bool OnTouchEvent(TouchEvents event) override;
         
        private:          
          Controllers::Settings &settingsController;

          bool running = true;
          ScreenList<2> screens;
          std::unique_ptr<Screen> CreateScreen1();
          std::unique_ptr<Screen> CreateScreen2();

      };
    }
  }
}
