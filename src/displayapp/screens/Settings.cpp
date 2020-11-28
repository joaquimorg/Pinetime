#include "Settings.h"
#include <lvgl/lvgl.h>
#include "../DisplayApp.h"
#include "drivers/BMA421.h"
#include "board_config.h"
#include <libraries/gpiote/app_gpiote.h>

using namespace Pinetime::Applications::Screens;

Settings::Settings(Pinetime::Applications::DisplayApp *app) : Screen(app) {

  llabel = lv_label_create(lv_scr_act(), NULL);
  lv_label_set_recolor(llabel, true);                      /*Enable re-coloring by commands in the text*/
  lv_label_set_text_fmt(llabel,     "#0000FF Screen# %s", "Settings"); 
  lv_label_set_align(llabel, LV_LABEL_ALIGN_CENTER);
  lv_obj_align(llabel, NULL, LV_ALIGN_CENTER, 0, 0);

}

Settings::~Settings() {
  lv_obj_clean(lv_scr_act());
}

bool Settings::Refresh() {
  
  return running;
}

bool Settings::OnButtonPushed() {
  running = false;
  return true;
}
