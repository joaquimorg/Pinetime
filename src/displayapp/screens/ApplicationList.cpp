#include "ApplicationList.h"
#include <lvgl/lvgl.h>
#include <array>
#include "Symbols.h"
#include "Tile.h"
#include "displayapp/Apps.h"
#include "../DisplayApp.h"

using namespace Pinetime::Applications::Screens;

LV_IMG_DECLARE(icon_settings);
LV_IMG_DECLARE(icon_information);
LV_IMG_DECLARE(icon_phone);
LV_IMG_DECLARE(icon_brightness);

LV_IMG_DECLARE(icon_music);
LV_IMG_DECLARE(icon_game);
LV_IMG_DECLARE(icon_running);
LV_IMG_DECLARE(icon_heart_rate);

LV_IMG_DECLARE(icon_folder);
LV_IMG_DECLARE(icon_raining);
LV_IMG_DECLARE(icon_iot);
LV_IMG_DECLARE(icon_qr_code);

ApplicationList::ApplicationList(Pinetime::Applications::DisplayApp *app, 
        Pinetime::Controllers::DateTime& dateTimeController,
        Pinetime::Controllers::Settings &settingsController) :
        Screen(app),
        dateTimeController{dateTimeController},
        settingsController{settingsController},
        screens{app, {
                [this]() -> std::unique_ptr<Screen> { return CreateScreen1(); },
                [this]() -> std::unique_ptr<Screen> { return CreateScreen2(); },
                [this]() -> std::unique_ptr<Screen> { return CreateScreen3(); }
          },
          Screens::ScreenListModes::UpDown,
          settingsController.GetAppMenu()
        }        
{}


ApplicationList::~ApplicationList() {
  lv_obj_clean(lv_scr_act());
}

bool ApplicationList::Refresh() {
  if(running)
    running = screens.Refresh();
  return running;
}

bool ApplicationList::OnButtonPushed() {
  running = false;
  app->StartApp(Apps::Clock);
  return true;
}

bool ApplicationList::OnTouchEvent(Pinetime::Applications::TouchEvents event) {
  return screens.OnTouchEvent(event);
}

std::unique_ptr<Screen> ApplicationList::CreateScreen1() {
  std::array<Screens::Tile::Applications, 4> applications {
          {                        
            {&icon_settings,    "Settings",       Apps::Settings},
            {&icon_information, "Sysinfo",        Apps::SysInfo},            
            {&icon_phone,       "Notification",  Apps::Notifications},
            {&icon_brightness,  "Brightness",     Apps::Brightness}
          }

  };

  return std::unique_ptr<Screen>(new Screens::Tile(0, 3, app, dateTimeController, settingsController, applications));
}

std::unique_ptr<Screen> ApplicationList::CreateScreen2() {
  std::array<Screens::Tile::Applications, 4> applications {
          {                        
            {&icon_music,       "Music",      Apps::Music},
            {&icon_game,        "Paint",      Apps::Paint},            
            {&icon_running,     "Steps",      Apps::Steps},
            {&icon_heart_rate,  "Heart Rate", Apps::Meter}
          }

  };

  return std::unique_ptr<Screen>(new Screens::Tile(1, 3, app, dateTimeController, settingsController, applications));
}

std::unique_ptr<Screen> ApplicationList::CreateScreen3() {
  std::array<Screens::Tile::Applications, 4> applications {
          {                        
            {&icon_folder,      "File",       Apps::FileManager},
            {&icon_raining,     "Weather",    Apps::Meter},            
            {&icon_iot,         "Iot",        Apps::Meter},
            {&icon_qr_code,     "Mobile App", Apps::Meter}
          }

  };

  return std::unique_ptr<Screen>(new Screens::Tile(2, 3, app, dateTimeController, settingsController, applications));
}
