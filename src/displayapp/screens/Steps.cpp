#include "Steps.h"
#include <lvgl/lvgl.h>
#include "../DisplayApp.h"
#include "drivers/BMA421.h"
#include "board_config.h"
#include <libraries/gpiote/app_gpiote.h>

LV_IMG_DECLARE(icon_running);

using namespace Pinetime::Applications::Screens;

Steps::Steps(
    Pinetime::Applications::DisplayApp *app, 
    Pinetime::Drivers::BMA421& stepCounter,
    Controllers::Settings &settingsController) 
      : Screen(app), 
      stepCounter{stepCounter},
      settingsController{settingsController} {

  stepCounter.Update();

  stepsArc = lv_arc_create(lv_scr_act(), NULL);

  lv_obj_set_style_local_bg_opa(stepsArc, LV_ARC_PART_BG, LV_STATE_DEFAULT, LV_OPA_0);  
  lv_obj_set_style_local_border_width(stepsArc, LV_ARC_PART_BG, LV_STATE_DEFAULT, 0);  
  lv_obj_set_style_local_radius(stepsArc, LV_ARC_PART_BG, LV_STATE_DEFAULT, 0);  
  lv_obj_set_style_local_line_color(stepsArc, LV_ARC_PART_INDIC, LV_STATE_DEFAULT, lv_color_hex(0x0000FF));  
  lv_arc_set_end_angle(stepsArc, 200);
  lv_obj_set_size(stepsArc, 220, 220);
  lv_arc_set_range(stepsArc, 0, 500);
  lv_obj_align(stepsArc, NULL, LV_ALIGN_CENTER, 0, 0);

  lv_obj_t * steps_icon = lv_img_create(lv_scr_act(), NULL);
  lv_img_set_src(steps_icon, &icon_running);  
  lv_obj_align(steps_icon, NULL, LV_ALIGN_CENTER, 0, -25);

  lSteps = lv_label_create(lv_scr_act(), NULL);
  lv_obj_set_style_local_text_color(lSteps, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0x0000FF));
  lv_obj_set_style_local_text_font(lSteps, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_font_clock_42);   
  lv_label_set_text_fmt(lSteps,"%li", stepCounter.GetSteps()); 
  lv_obj_align(lSteps, steps_icon, LV_ALIGN_OUT_BOTTOM_MID, 0, 20);

  lv_arc_set_value(stepsArc, int16_t(500 * stepCounter.GetSteps() / settingsController.GetStepsGoal()));

  lv_obj_t * lstepsL = lv_label_create(lv_scr_act(), NULL);  
  lv_label_set_text_fmt(lstepsL,"Steps\n/ %i", settingsController.GetStepsGoal()); 
  lv_obj_align(lstepsL, lSteps, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);

  lv_obj_t * backgroundLabel = lv_label_create(lv_scr_act(), nullptr);
  lv_label_set_long_mode(backgroundLabel, LV_LABEL_LONG_CROP);
  lv_obj_set_size(backgroundLabel, 240, 240);
  lv_obj_set_pos(backgroundLabel, 0, 0);
  lv_label_set_text_static(backgroundLabel, "");

}

Steps::~Steps() {
  lv_obj_clean(lv_scr_act());
}

bool Steps::Refresh() {
  
  stepCounter.Update();

  lv_label_set_text_fmt(lSteps,"%li", stepCounter.GetSteps()); 

  stepCount = stepCounter.GetSteps();
  if(stepCount.IsUpdated()) {        
    lv_arc_set_value(stepsArc, int16_t(500 * stepCount.Get() / settingsController.GetStepsGoal()));
  }  

  return running;
}


