#include "ApplicationList.h"
#include <lvgl/lvgl.h>
#include <array>
#include "Symbols.h"
#include "Tile.h"
#include "displayapp/Apps.h"
#include "../DisplayApp.h"

  LV_IMG_DECLARE(icon_running);
  LV_IMG_DECLARE(icon_raining);
  
  /*LV_IMG_DECLARE(icon_charging);
  LV_IMG_DECLARE(icon_running);

  
  LV_IMG_DECLARE(icon_brightness);

  LV_IMG_DECLARE(icon_raining);
  LV_IMG_DECLARE(icon_folder);
  LV_IMG_DECLARE(icon_running);
  LV_IMG_DECLARE(icon_heart_rate);

  LV_IMG_DECLARE(icon_game);
  LV_IMG_DECLARE(icon_two);
  LV_IMG_DECLARE(icon_music);
  LV_IMG_DECLARE(icon_paddle);
*/

using namespace Pinetime::Applications::Screens;


ApplicationList::ApplicationList(Pinetime::Applications::DisplayApp *app, 
        Pinetime::Controllers::DateTime& dateTimeController,
        Pinetime::Controllers::Settings &settingsController,
        Pinetime::Controllers::Battery& batteryController) :
        Screen(app),
        dateTimeController{dateTimeController},
        settingsController{settingsController},
        batteryController{batteryController},
        screens{app, 
          settingsController.GetAppMenu(),
          {
                [this]() -> std::unique_ptr<Screen> { return CreateScreen1(); },
                //[this]() -> std::unique_ptr<Screen> { return CreateScreen2(); },
                //[this]() -> std::unique_ptr<Screen> { return CreateScreen3(); }
          },
          Screens::ScreenListModes::UpDown          
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

bool ApplicationList::OnTouchEvent(Pinetime::Applications::TouchEvents event) {
  return screens.OnTouchEvent(event);
}

std::unique_ptr<Screen> ApplicationList::CreateScreen1() {

  std::array<Screens::Tile::Applications, 4> applications {
          { 
            {&icon_running,     "Steps",      Apps::Steps},                       
            {&icon_running,     "--",         Apps::None},
            {&icon_raining,     "Weather",    Apps::Clock},
            {&icon_running,     "--",         Apps::None},
          }

  };

  return std::unique_ptr<Screen>(new Screens::Tile(0, 1, app, dateTimeController, settingsController, batteryController, applications));
}

/*
std::unique_ptr<Screen> ApplicationList::CreateScreen2() {  

  std::array<Screens::Tile::Applications, 4> applications {
          {                                    
            {&icon_running,     "Steps",      Apps::Steps},
            {&icon_heart_rate,  "Heart Rate", Apps::FlashLight},
            {&icon_paddle,     "Paddle",    Apps::LowBatt},
            {&icon_two,      "2048",       Apps::FirmwareUpdate}
          }

  };

  return std::unique_ptr<Screen>(new Screens::Tile(1, 2, app, dateTimeController, settingsController, applications));
}
*/
/*
std::unique_ptr<Screen> ApplicationList::CreateScreen3() {

  std::array<Screens::Tile::Applications, 4> applications {
          {
            {&icon_paddle,      "Paddle",     Apps::Paddle}, 
            {&icon_music,       "Music",      Apps::None},
            {&icon_game,        "Paint",      Apps::None},
            {&icon_two,         "2048",       Apps::Twos}
          }

  };

  return std::unique_ptr<Screen>(new Screens::Tile(2 3, app, dateTimeController, settingsController, applications));
}

*/