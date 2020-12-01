#include "HeartRate.h"
#include <lvgl/lvgl.h>
#include "../DisplayApp.h"
#include "drivers/HRS3300.h"
#include "board_config.h"

using namespace Pinetime::Applications::Screens;

LV_IMG_DECLARE(icon_heart_rate);
extern lv_style_t* LabelStyle42;

HeartRate::HeartRate(
  Pinetime::Applications::DisplayApp *app, Drivers::HRS3300 &hrs, Controllers::Settings &settingsController) : 
  Screen(app),
  hrs{hrs},
  settingsController{settingsController}
{

  hrs.SetEnable(Drivers::HRS3300::HRS_ENABLE,
                 Drivers::HRS3300::HRS_WAIT_TIME_75ms);

  llabel = lv_label_create(lv_scr_act(), NULL);
  lv_label_set_recolor(llabel, true);                     
  lv_label_set_text_fmt(llabel, "#00FF00 %s#", "Heart Rate"); 
  lv_label_set_align(llabel, LV_LABEL_ALIGN_CENTER);
  lv_obj_align(llabel, NULL, LV_ALIGN_CENTER, 0, 60);

  lv_obj_t * heart_rate = lv_img_create(lv_scr_act(), NULL);
  lv_img_set_src(heart_rate, &icon_heart_rate);
  lv_obj_align(heart_rate, NULL, LV_ALIGN_CENTER, 0, -45);

  static lv_style_t heart_rate_style;
  lv_style_copy(&heart_rate_style, LabelStyle42);
  heart_rate_style.text.color = lv_color_hex(0xFF0000);  


  lhrs = lv_label_create(lv_scr_act(), NULL);
  lv_label_set_style(lhrs, LV_LABEL_STYLE_MAIN, &heart_rate_style);
  lv_label_set_text(lhrs, "--"); 
  lv_label_set_align(lhrs, LV_LABEL_ALIGN_CENTER);
  lv_obj_align(lhrs, NULL, LV_ALIGN_CENTER, 0, 20);

}

HeartRate::~HeartRate() {
  hrs.SetEnable();

  lv_obj_clean(lv_scr_act());
}

bool HeartRate::Refresh() {

  uint8_t hrv = hrs.ReadHeartRate(); // If 0 read ok
  heartRate = hrs.GetHeartRate(); // heartRate.hr_result
  
  if ( hrv == 251 ) {
    lv_label_set_text_fmt(lhrs, "%02i", heartRate.hr_result); 
    settingsController.SetHeartRate( heartRate.hr_result );
  } else
    lv_label_set_text(lhrs, "--"); 

  //lv_label_set_align(lhrs, LV_LABEL_ALIGN_CENTER);
  lv_obj_align(lhrs, NULL, LV_ALIGN_CENTER, 0, 20);

  return running;
}

bool HeartRate::OnButtonPushed() {
  running = false;
  return true;
}
