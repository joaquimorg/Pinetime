#include "Settings.h"
#include <lvgl/lvgl.h>
#include <array>
#include "Symbols.h"
#include "List.h"
#include "Tile.h"
#include "displayapp/Apps.h"
#include "../DisplayApp.h"

using namespace Pinetime::Applications::Screens;

Settings::Settings(
  Pinetime::Applications::DisplayApp *app, 
  Controllers::Battery& batteryController, 
  Pinetime::Controllers::DateTime& dateTimeController,
  Pinetime::Controllers::Settings &settingsController) :
    Screen(app), 
    batteryController{batteryController},
    dateTimeController{dateTimeController},
    settingsController{settingsController},
    screens{app, 
      settingsController.GetSettingsMenu(),
      {
        [this]() -> std::unique_ptr<Screen> { return CreateScreen1(); },
        [this]() -> std::unique_ptr<Screen> { return CreateScreen2(); }
        //[this]() -> std::unique_ptr<Screen> { return CreateScreen3(); }
      },
      Screens::ScreenListModes::UpDown          
    } {}

Settings::~Settings() {
  lv_obj_clean(lv_scr_act());
}

bool Settings::Refresh() {
  
  if(running)
    running = screens.Refresh();
  return running;
}

bool Settings::OnTouchEvent(Pinetime::Applications::TouchEvents event) {
  return screens.OnTouchEvent(event);
}

std::unique_ptr<Screen> Settings::CreateScreen1() {

  std::array<Screens::List::Applications, 4> applications {
          {                        
            {Symbols::backLight,        "Display",       Apps::Clock},
            {Symbols::shoe,             "Steps",         Apps::Clock},
            {Symbols::clock,            "Time format",   Apps::Clock},
            {Symbols::wface,            "Watch face",    Apps::Clock},
          }

  };

  return std::unique_ptr<Screen>(new Screens::List(0, 2, app, dateTimeController, settingsController, applications));
}


std::unique_ptr<Screen> Settings::CreateScreen2() {

  std::array<Screens::List::Applications, 4> applications {
          {                        
            {Symbols::batteryUnknown,   "Battery",   Apps::Charging},
            {Symbols::firmware,         "Firmware",  Apps::FirmwareValidation},
            {Symbols::about,            "About",     Apps::About},
            {Symbols::none,             "None",      Apps::None},
          }

  };

  return std::unique_ptr<Screen>(new Screens::List(1, 2, app, dateTimeController, settingsController, applications));
}
