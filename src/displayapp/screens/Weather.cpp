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

  lv_obj_t * _icon1 = lv_img_create(lv_scr_act(), nullptr);
  lv_img_set_src(_icon1, "F:/met_01.bin");
  lv_obj_align(_icon1, nullptr, LV_ALIGN_CENTER, -60, -30);

  lv_obj_t * _icon2 = lv_img_create(lv_scr_act(), nullptr);
  lv_img_set_src(_icon2, "F:/met_02.bin");
  lv_obj_align(_icon2, nullptr, LV_ALIGN_CENTER, 60, -30);

  lv_obj_t * _label = lv_label_create(lv_scr_act(), nullptr);
  lv_label_set_recolor(_label, true);
  lv_label_set_text_static(_label, "#00ff00 Nice weather!#\n#444444 Demo app testing icons...#");
  lv_obj_align(_label, nullptr, LV_ALIGN_CENTER, 0, 60);

  lv_obj_t * backgroundLabel = lv_label_create(lv_scr_act(), nullptr);
  lv_label_set_long_mode(backgroundLabel, LV_LABEL_LONG_CROP);
  lv_obj_set_size(backgroundLabel, 240, 240);
  lv_obj_set_pos(backgroundLabel, 0, 0);
  lv_label_set_text_static(backgroundLabel, "");

}

Weather::~Weather() {
  lv_obj_clean(lv_scr_act());
}

bool Weather::Refresh() { 
  return running;
}