#pragma once

#include <cstdint>
#include "Screen.h"
#include "ScreenList.h"

namespace Pinetime {


  namespace Applications {
    namespace Screens {

      class Settings : public Screen{
        public:
          Settings(DisplayApp* app, 
          Controllers::Battery& batteryController,
          Pinetime::Controllers::DateTime& dateTimeController,
          Pinetime::Controllers::Settings &settingsController);
          ~Settings() override;

          bool Refresh() override;
          bool OnButtonPushed() override;
          void OnButtonEvent(lv_obj_t *object, lv_event_t event);
          bool OnTouchEvent(Pinetime::Applications::TouchEvents event) override;

        private:          

          Controllers::Battery& batteryController;
          Controllers::DateTime& dateTimeController;
          Controllers::Settings& settingsController;
          
          ScreenList<1> screens;

          std::unique_ptr<Screen> CreateScreen1();
          //std::unique_ptr<Screen> CreateScreen2();

          bool running = true;

      };
    }
  }
}
