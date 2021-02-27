#include "Settings.h"
#include <lvgl/lvgl.h>
#include <array>
#include "Symbols.h"
#include "List.h"
#include "displayapp/Apps.h"
#include "../DisplayApp.h"

using namespace Pinetime::Applications::Screens;

LV_IMG_DECLARE(icon_settings);

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
        //[this]() -> std::unique_ptr<Screen> { return CreateScreen2(); },
        //[this]() -> std::unique_ptr<Screen> { return CreateScreen3(); }
      },
      Screens::ScreenListModes::UpDown          
    }

{

  /*lv_obj_t * backgroundLabel = lv_label_create(lv_scr_act(), nullptr);
  lv_label_set_long_mode(backgroundLabel, LV_LABEL_LONG_CROP);
  lv_obj_set_size(backgroundLabel, 240, 240);
  lv_obj_set_pos(backgroundLabel, 0, 0);
  lv_label_set_text_static(backgroundLabel, "");*/

}

Settings::~Settings() {
  lv_obj_clean(lv_scr_act());
}

bool Settings::Refresh() {
  
  if(running)
    running = screens.Refresh();
  return running;
}

bool Settings::OnTouchEvent(Pinetime::Applications::TouchEvents event) {
  switch (event) {
    case Pinetime::Applications::TouchEvents::SwipeRight:
      running = false;
      app->StartApp(Apps::QuickSettings, DisplayApp::FullRefreshDirections::None);
      return true;

    default:
      return screens.OnTouchEvent(event);
  }  
}

bool Settings::OnButtonPushed() {
  running = false;
  app->StartApp(Apps::QuickSettings, DisplayApp::FullRefreshDirections::None);
  return true;
}


std::unique_ptr<Screen> Settings::CreateScreen1() {

  std::array<Screens::List::Applications, 5> applications {
          {                        
            {Symbols::backLight,        "Backlight",     Apps::Clock},
            {Symbols::shoe,             "Steps",         Apps::Clock},
            {Symbols::clock,            "Clock",         Apps::Clock},
            {Symbols::firmware,         "Firmware",      Apps::Clock},
            {Symbols::about,            "About",         Apps::Clock},
          }

  };

  return std::unique_ptr<Screen>(new Screens::List(0, 1, app, dateTimeController, settingsController, applications));
}

/*
std::unique_ptr<Screen> Settings::CreateScreen2() {

  std::array<Screens::List::Applications, 5> applications {
          {                        
            {Symbols::info,    "demo 1",     Apps::Clock},
            {Symbols::dot,    "demo 2",         Apps::Steps},
            {Symbols::batteryError,    "demo 3",         Apps::SysInfo},
            {Symbols::dot,    "Firmware",      Apps::None},
            {Symbols::dot,    "About",         Apps::None},
          }

  };

  return std::unique_ptr<Screen>(new Screens::List(1, 2, app, dateTimeController, settingsController, applications));
}
*/