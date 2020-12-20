#include "Steps.h"
#include <lvgl/lvgl.h>
#include "../DisplayApp.h"
#include "drivers/BMA421.h"
#include "board_config.h"
#include <libraries/gpiote/app_gpiote.h>

using namespace Pinetime::Applications::Screens;

LV_IMG_DECLARE(icon_running);

Steps::Steps(Pinetime::Applications::DisplayApp *app, Pinetime::Drivers::BMA421& stepCounter) : Screen(app), stepCounter{stepCounter} {

  //stepCounter.Update();

  lv_obj_t * steps_icon = lv_img_create(lv_scr_act(), NULL);
  lv_img_set_src(steps_icon, &icon_running);  
  lv_obj_align(steps_icon, NULL, LV_ALIGN_CENTER, -40, -50);

  static lv_style_t steps_style;
  lv_style_init(&steps_style);
  lv_style_set_text_color(&steps_style, LV_STATE_DEFAULT, lv_color_hex(0x0000FF));  
  lv_style_set_text_font(&steps_style, LV_STATE_DEFAULT, &lv_font_clock_42);

  lSteps = lv_label_create(lv_scr_act(), NULL);
  lv_obj_add_style(lSteps, LV_LABEL_PART_MAIN, &steps_style);   
  lv_label_set_text_fmt(lSteps,"%li", stepCounter.GetSteps()); 
  lv_obj_align(lSteps, steps_icon, LV_ALIGN_OUT_RIGHT_MID, 20, -5);

  lTapStatus = lv_label_create(lv_scr_act(), NULL);
  lv_label_set_recolor(lTapStatus, true);                      
  lv_label_set_text_fmt(lTapStatus,  "intStatus #00FF00 %i#", stepCounter.GetTapStatus()); 
  lv_label_set_align(lTapStatus, LV_LABEL_ALIGN_LEFT);
  lv_obj_align(lTapStatus, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 10, 0);

}

Steps::~Steps() {
  lv_obj_clean(lv_scr_act());
}

bool Steps::Refresh() {
  
  stepCounter.Update();

  lv_label_set_text_fmt(lSteps,"%li", stepCounter.GetSteps()); 
  lv_label_set_text_fmt(lTapStatus, "intStatus #00FF00 %2i#", stepCounter.GetTapStatus()); 

  return running;
}

bool Steps::OnButtonPushed() {
  running = false;
  return true;
}
