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

  static lv_style_t arcbg_style;
  lv_style_init(&arcbg_style);
  lv_style_set_bg_opa(&arcbg_style, LV_STATE_DEFAULT, LV_OPA_0);
  lv_style_set_border_width(&arcbg_style, LV_STATE_DEFAULT, 0);
  lv_style_set_radius(&arcbg_style, LV_STATE_DEFAULT, 0);

  static lv_style_t arc_style;
  lv_style_init(&arc_style);
  lv_style_set_line_color(&arc_style, LV_STATE_DEFAULT, lv_color_hex(0x0000FF));

  stepsArc = lv_arc_create(lv_scr_act(), NULL);
  lv_obj_add_style(stepsArc, LV_ARC_PART_BG, &arcbg_style);   
  lv_obj_add_style(stepsArc, LV_ARC_PART_INDIC, &arc_style);   
  lv_arc_set_end_angle(stepsArc, 200);
  lv_obj_set_size(stepsArc, 220, 220);
  lv_arc_set_range(stepsArc, 0, 500);
  lv_obj_align(stepsArc, NULL, LV_ALIGN_CENTER, 0, 0);

  lv_obj_t * steps_icon = lv_img_create(lv_scr_act(), NULL);
  lv_img_set_src(steps_icon, &icon_running);  
  lv_obj_align(steps_icon, NULL, LV_ALIGN_CENTER, 0, -25);

  static lv_style_t steps_style;
  lv_style_init(&steps_style);
  lv_style_set_text_color(&steps_style, LV_STATE_DEFAULT, lv_color_hex(0x0000FF));  
  lv_style_set_text_font(&steps_style, LV_STATE_DEFAULT, &lv_font_clock_42);

  lSteps = lv_label_create(lv_scr_act(), NULL);
  lv_obj_add_style(lSteps, LV_LABEL_PART_MAIN, &steps_style);   
  lv_label_set_text_fmt(lSteps,"%li", stepCounter.GetSteps()); 
  lv_obj_align(lSteps, steps_icon, LV_ALIGN_OUT_BOTTOM_MID, 0, 20);

  lv_arc_set_value(stepsArc, int16_t(500 * stepCounter.GetSteps() / settingsController.GetStepsGoal()));

  lv_obj_t * lstepsL = lv_label_create(lv_scr_act(), NULL);  
  lv_label_set_text_fmt(lstepsL,"Steps\n/ %i", settingsController.GetStepsGoal()); 
  lv_obj_align(lstepsL, lSteps, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);

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

bool Steps::OnButtonPushed() {
  running = false;
  return true;
}
