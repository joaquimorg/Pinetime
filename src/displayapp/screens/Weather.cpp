#include "Weather.h"
#include <lvgl/lvgl.h>
#include "../DisplayApp.h"
#include "WeatherToday.h"
#include "WeatherForecast.h"

using namespace Pinetime::Applications::Screens;


Weather::Weather(DisplayApp* app, Controllers::Settings &settingsController) : 
  Screen(app),
  settingsController{settingsController},
  screens{app, 
    0,
    {
          [this]() -> std::unique_ptr<Screen> { return CreateScreen1(); },
          [this]() -> std::unique_ptr<Screen> { return CreateScreen2(); },
    },
    Screens::ScreenListModes::UpDown
  } {}

Weather::~Weather() {
  lv_obj_clean(lv_scr_act());
}

bool Weather::Refresh() {
  return running;
}

bool Weather::OnTouchEvent(Pinetime::Applications::TouchEvents event) {
  return screens.OnTouchEvent(event);
}

std::unique_ptr<Screen> Weather::CreateScreen1() {
  return std::unique_ptr<Screen>(new Screens::WeatherToday(app, settingsController));
}

std::unique_ptr<Screen> Weather::CreateScreen2() {
  return std::unique_ptr<Screen>(new Screens::WeatherForecast(app, settingsController));
}