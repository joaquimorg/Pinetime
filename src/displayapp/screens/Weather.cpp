#include "Weather.h"
#include <lvgl/lvgl.h>
#include "../DisplayApp.h"

using namespace Pinetime::Applications::Screens;


Weather::Weather(
  Pinetime::Applications::DisplayApp *app, Drivers::SpiNorFlash& spiNorFlash) : 
  Screen(app),
  spiNorFlash{spiNorFlash}
{
  // CF1F - 53023

  llabel = lv_label_create(lv_scr_act(), NULL);
  lv_label_set_recolor(llabel, true);
  lv_label_set_text(llabel, "#0000FF Screen# for Weather"); 
  lv_label_set_align(llabel, LV_LABEL_ALIGN_CENTER);
  lv_obj_align(llabel, NULL, LV_ALIGN_CENTER, 0, 0);


}

Weather::~Weather() {
  lv_obj_clean(lv_scr_act());
}

bool Weather::Refresh() {
  return running;
}