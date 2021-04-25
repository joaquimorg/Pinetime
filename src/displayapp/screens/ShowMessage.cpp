#include "ShowMessage.h"
#include <lvgl/lvgl.h>
#include "../DisplayApp.h"

using namespace Pinetime::Applications::Screens;


ShowMessage::ShowMessage(
  Pinetime::Applications::DisplayApp *app, const char* message, const void * src_img) : 
  Screen(app)
{

  info = lv_label_create(lv_scr_act(), nullptr);  
  lv_obj_set_style_local_text_color(info, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_CYAN);
  lv_label_set_text_static(info, message);
  lv_obj_align(info, nullptr, LV_ALIGN_CENTER, 0, 30);
  lv_label_set_align(info, LV_LABEL_ALIGN_CENTER);

  iconAnim = lv_img_create(lv_scr_act(), nullptr);
  lv_img_set_src(iconAnim, src_img);
  lv_obj_align(iconAnim, nullptr, LV_ALIGN_CENTER, 0, -50);

  lv_obj_t * backgroundLabel = lv_label_create(lv_scr_act(), nullptr);
  lv_label_set_long_mode(backgroundLabel, LV_LABEL_LONG_CROP);
  lv_obj_set_size(backgroundLabel, 240, 240);
  lv_obj_set_pos(backgroundLabel, 0, 0);
  lv_label_set_text_static(backgroundLabel, "");
  
}

ShowMessage::~ShowMessage() {
  lv_obj_clean(lv_scr_act());
}

bool ShowMessage::Refresh() {
  return running;
}