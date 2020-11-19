#include <libs/lvgl/lvgl.h>
#include <displayapp/DisplayApp.h>
#include <functional>
#include "ApplicationList.h"
#include "Tile.h"
#include "Symbols.h"

using namespace Pinetime::Applications::Screens;

LV_IMG_DECLARE(icon_settings);
LV_IMG_DECLARE(icon_information);
LV_IMG_DECLARE(icon_phone);
LV_IMG_DECLARE(icon_brightness);

ApplicationList::ApplicationList(Pinetime::Applications::DisplayApp *app) :
        Screen(app),
        screens{app, {
                [this]() -> std::unique_ptr<Screen> { return CreateScreen1(); }
                //[this]() -> std::unique_ptr<Screen> { return CreateScreen2(); },
                //[this]() -> std::unique_ptr<Screen> { return CreateScreen3(); }
          }
        } {}


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
            {&icon_settings,    "Firmware",       Apps::Gauge},
            {&icon_information, "Sysinfo",        Apps::SysInfo},            
            {&icon_phone,       "Notifications",  Apps::Notifications},
            {&icon_brightness,  "Brightness",     Apps::Brightness}
          }

  };

  return std::unique_ptr<Screen>(new Screens::Tile(app, applications));
}

/*
std::unique_ptr<Screen> ApplicationList::CreateScreen2() {
  std::array<Screens::Tile::Applications, 6> applications {
          {{Symbols::tachometer, Apps::Gauge},
           {Symbols::asterisk, Apps::Meter},
           {Symbols::paintbrush, Apps::Paint},
                  {Symbols::info, Apps::Notifications},
                  {Symbols::none, Apps::None},
                  {Symbols::none, Apps::None}
          }
  };

  return std::unique_ptr<Screen>(new Screens::Tile(app, applications));
}

std::unique_ptr<Screen> ApplicationList::CreateScreen3() {
  std::array<Screens::Tile::Applications, 6> applications {
          {{"A", Apps::Meter},
           {"B", Apps::Gauge},
           {"C", Apps::Clock},
           {"D", Apps::Music},
           {"E", Apps::SysInfo},
           {"F", Apps::Brightness}
          }
  };

  return std::unique_ptr<Screen>(new Screens::Tile(app, applications));
}
*/
