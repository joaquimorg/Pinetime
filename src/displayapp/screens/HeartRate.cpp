#include "HeartRate.h"
#include <lvgl/lvgl.h>
#include "../DisplayApp.h"
#include "drivers/HRS3300.h"
#include "board_config.h"

using namespace Pinetime::Applications::Screens;

LV_IMG_DECLARE(icon_heart_rate);
extern lv_style_t* LabelStyle42;


void HRTimerCallback(TimerHandle_t xTimer) {

  auto heartRate = static_cast<HeartRate *>(pvTimerGetTimerID(xTimer));
  heartRate->EndHRReading();
}

HeartRate::HeartRate(
  Pinetime::Applications::DisplayApp *app, 
  Drivers::HRS3300 &hrs, 
  Controllers::Settings &settingsController,
  System::SystemTask &systemTask
  ) : 
  Screen(app),
  hrs{hrs},
  settingsController{settingsController},
  systemTask{systemTask}
{

  /*Create a chart*/
  /*lv_obj_t * chart;
  chart = lv_chart_create(lv_scr_act(), NULL);
  lv_obj_set_size(chart, 220, 50);
  lv_obj_align(chart, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, 0);
  lv_chart_set_type(chart, LV_CHART_TYPE_POINT );   
  //lv_chart_set_series_opa(chart, LV_OPA_70);                            
  //lv_chart_set_series_width(chart, 4);  
  //lv_chart_set_point_count(chart, 8);                               

  lv_chart_set_range(chart, 30, 120);

  lv_chart_series_t * ser1 = lv_chart_add_series(chart, LV_COLOR_RED);
  for (uint8_t i = 0; i < 8; i++)
  {
    ser1->points[i] = hrs.GetHistory(i);
  }
  lv_chart_refresh(chart);*/
  
  hrs.SetEnable(Drivers::HRS3300::HRS_ENABLE,
                 Drivers::HRS3300::HRS_WAIT_TIME_75ms);
 
  llabel = lv_label_create(lv_scr_act(), NULL);
  lv_label_set_recolor(llabel, true);                     
  lv_label_set_text_fmt(llabel, "#00FF00 %s#", "Reading heart rate"); 
  lv_label_set_align(llabel, LV_LABEL_ALIGN_CENTER);
  lv_obj_align(llabel, NULL, LV_ALIGN_CENTER, 0, 75);

  lv_obj_t * heart_rate = lv_img_create(lv_scr_act(), NULL);
  lv_img_set_src(heart_rate, &icon_heart_rate);
  lv_obj_align(heart_rate, NULL, LV_ALIGN_CENTER, 0, -50);

  static lv_style_t heart_rate_style;
  lv_style_copy(&heart_rate_style, LabelStyle42);
  heart_rate_style.text.color = lv_color_hex(0xFF0000);  

  lhrs = lv_label_create(lv_scr_act(), NULL);
  lv_label_set_style(lhrs, LV_LABEL_STYLE_MAIN, &heart_rate_style);
  lv_label_set_text(lhrs, "--"); 
  lv_label_set_align(lhrs, LV_LABEL_ALIGN_CENTER);
  lv_obj_align(lhrs, NULL, LV_ALIGN_CENTER, 0, 20);

  hrTimer = xTimerCreate ("hrTimer", pdMS_TO_TICKS( 20000 ), pdFALSE, this, HRTimerCallback);
  xTimerStart(hrTimer, 0);
  heartRateReading = 1;
}

HeartRate::~HeartRate() {
  xTimerStop(hrTimer, 0);
  hrs.SetEnable();  
  lv_obj_clean(lv_scr_act());
}

bool HeartRate::Refresh() {

  if ( heartRateReading == 1 ) {
    systemTask.PushMessage(Pinetime::System::SystemTask::Messages::ReloadIdleTimer);

    hrv = hrs.GetStatus(); // If 244 read ok
    heartRate = hrs.GetHeartRate(); // heartRate
    //bpHigh = hrs.GetBPHigh();
    //bpLow = hrs.GetBPLow();
    
    if ( hrv == 244 ) {
      lv_label_set_text_fmt(lhrs, "%02i", heartRate); 
      settingsController.SetHeartRate( heartRate );
    } else if ( hrv == 245 ) {
      EndHRReading();
      lv_label_set_text(llabel, "#FF0000 Timeout#");
    } else {
      lv_label_set_text(lhrs, "--"); 
    }

    /*if ( hrv == 234 ) {
      lv_label_set_text_fmt(llabel, "#00FF00 SYS# %i #00FF00 DIA# %i#", bpHigh, bpLow);    
    } else {
      lv_label_set_text_fmt(llabel, "#FF0000 Status# %i", hrv);
    }*/
    
    lv_obj_align(llabel, NULL, LV_ALIGN_CENTER, 0, 60); 
    lv_obj_align(lhrs, NULL, LV_ALIGN_CENTER, 0, 20);
  }

  return running;
}

void HeartRate::EndHRReading() {
  heartRateReading = 0;
  hrs.SetEnable();
  if ( hrv != 244 ) {
    lv_label_set_text_fmt(llabel, "#FF0000 Error reading sensor# %i", hrv);
    lv_obj_align(llabel, NULL, LV_ALIGN_CENTER, 0, 60); 
  } else {
    lv_label_set_text(llabel, "#00FF00 Done#");
    lv_obj_align(llabel, NULL, LV_ALIGN_CENTER, 0, 60);
  }
}

bool HeartRate::OnButtonPushed() {
  running = false;
  return true;
}
