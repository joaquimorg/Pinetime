#include "Weather.h"
#include <lvgl/lvgl.h>
#include "../DisplayApp.h"
#include "board_config.h"
#include <hal/nrf_gpio.h>

using namespace Pinetime::Applications::Screens;


Weather::Weather(
  Pinetime::Applications::DisplayApp *app) : 
  Screen(app)
{

  llabel = lv_label_create(lv_scr_act(), NULL);
  lv_label_set_recolor(llabel, true);
  lv_label_set_text_static(llabel, "!!!Demo Weather!!!");
  lv_label_set_align(llabel, LV_LABEL_ALIGN_CENTER);
  lv_obj_align(llabel, NULL, LV_ALIGN_CENTER, 0, 0);


}

Weather::~Weather() {
  lv_obj_clean(lv_scr_act());
}

bool Weather::Refresh() { 
  return running;
}