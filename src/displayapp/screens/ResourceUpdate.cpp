#include "ResourceUpdate.h"
#include <lvgl/lvgl.h>
#include "../DisplayApp.h"

using namespace Pinetime::Applications::Screens;


ResourceUpdate::ResourceUpdate(
  Pinetime::Applications::DisplayApp *app) : 
  Screen(app)
{

  lv_obj_t* llabel = lv_label_create(lv_scr_act(), NULL);
  lv_label_set_recolor(llabel, true);                      /*Enable re-coloring by commands in the text*/
  lv_label_set_text(llabel, "#0000FF Screen# resources"); 
  lv_label_set_align(llabel, LV_LABEL_ALIGN_CENTER);
  lv_obj_align(llabel, NULL, LV_ALIGN_CENTER, 0, 0);


}

ResourceUpdate::~ResourceUpdate() {
  lv_obj_clean(lv_scr_act());
}

bool ResourceUpdate::Refresh() {
  return running;
}