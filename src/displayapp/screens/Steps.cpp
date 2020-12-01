#include "Steps.h"
#include <lvgl/lvgl.h>
#include "../DisplayApp.h"
#include "drivers/BMA421.h"
#include "board_config.h"
#include <libraries/gpiote/app_gpiote.h>

using namespace Pinetime::Applications::Screens;

Steps::Steps(Pinetime::Applications::DisplayApp *app, Pinetime::Drivers::BMA421& stepCounter) : Screen(app), stepCounter{stepCounter} {

  stepCounter.Update();

  lirq = lv_label_create(lv_scr_act(), NULL);
  lv_label_set_recolor(lirq, true);                      
  lv_label_set_text_fmt(lirq,     "IRQ #00FF00 %i#", nrf_gpio_pin_read(BMA421_IRQ)); 
  lv_label_set_align(lirq, LV_LABEL_ALIGN_LEFT);
  lv_obj_align(lirq, NULL, LV_ALIGN_IN_LEFT_MID, 10, -90);

  lSteps = lv_label_create(lv_scr_act(), NULL);
  lv_label_set_recolor(lSteps, true);                      
  lv_label_set_text_fmt(lSteps,     "Steps #00FF00 %li#", stepCounter.GetSteps()); 
  lv_label_set_align(lSteps, LV_LABEL_ALIGN_LEFT);
  lv_obj_align(lSteps, NULL, LV_ALIGN_IN_LEFT_MID, 10, -60);

  lActivity = lv_label_create(lv_scr_act(), NULL);
  lv_label_set_recolor(lActivity, true);                      
  lv_label_set_text_fmt(lActivity,  "Activity #00FF00 %i#", stepCounter.GetActivity()); 
  lv_label_set_align(lActivity, LV_LABEL_ALIGN_LEFT);
  lv_obj_align(lActivity, NULL, LV_ALIGN_IN_LEFT_MID, 10, -30);

  /*lTapStatus = lv_label_create(lv_scr_act(), NULL);
  lv_label_set_recolor(lTapStatus, true);                      
  lv_label_set_text_fmt(lTapStatus,  "TapStatus #00FF00 %i#", stepCounter.GetTapStatus()); 
  lv_label_set_align(lTapStatus, LV_LABEL_ALIGN_LEFT);
  lv_obj_align(lTapStatus, NULL, LV_ALIGN_IN_LEFT_MID, 10, 0);*/

  lTemp = lv_label_create(lv_scr_act(), NULL);
  lv_label_set_recolor(lTemp, true);                      
  lv_label_set_text_fmt(lTemp,        "Temp #00FF00 %.2f#", stepCounter.GetTemp()); 
  lv_label_set_align(lTemp, LV_LABEL_ALIGN_LEFT);
  lv_obj_align(lTemp, NULL, LV_ALIGN_IN_LEFT_MID, 10, 30);

  sens_data = stepCounter.GetAccel();
  lAccel = lv_label_create(lv_scr_act(), NULL);
  lv_label_set_recolor(lAccel, true);                      
  lv_label_set_text_fmt(lAccel,       "Accel #00FF00 %i, %i, %i#", sens_data.x, sens_data.y, sens_data.z); 
  lv_label_set_align(lAccel, LV_LABEL_ALIGN_LEFT);
  lv_obj_align(lAccel, NULL, LV_ALIGN_IN_LEFT_MID, 10, 60);
}

Steps::~Steps() {
  lv_obj_clean(lv_scr_act());
}

bool Steps::Refresh() {
  
  stepCounter.Update();
  
  lv_label_set_text_fmt(lirq,       "IRQ #00FF00 %i#", nrf_gpio_pin_read(BMA421_IRQ)); 
  lv_label_set_text_fmt(lSteps,     "Steps #00FF00 %li#", stepCounter.GetSteps()); 
  lv_label_set_text_fmt(lActivity,  "Activity #00FF00 %2i#", stepCounter.GetActivity()); 
  //lv_label_set_text_fmt(lTapStatus, "TapStatus #00FF00 %2i#", stepCounter.GetTapStatus()); 
  lv_label_set_text_fmt(lTemp,      "Temp #00FF00 %.2f#", stepCounter.GetTemp()); 

  sens_data = stepCounter.GetAccel();
  lv_label_set_text_fmt(lAccel,     "Accel #00FF00 %i, %i, %i#", sens_data.x, sens_data.y, sens_data.z); 
  return running;
}

bool Steps::OnButtonPushed() {
  running = false;
  return true;
}
