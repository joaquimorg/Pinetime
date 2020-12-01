#include "HeartRate.h"
#include <lvgl/lvgl.h>
#include "../DisplayApp.h"
#include "drivers/HRS3300.h"
#include "board_config.h"

using namespace Pinetime::Applications::Screens;


HeartRate::HeartRate(
  Pinetime::Applications::DisplayApp *app, Drivers::HRS3300& hrs) : 
  Screen(app),
  hrs{hrs}
{

  hrs.SetEnable(Drivers::HRS3300::HRS_ENABLE,
                 Drivers::HRS3300::HRS_WAIT_TIME_400ms);

  llabel = lv_label_create(lv_scr_act(), NULL);
  lv_label_set_recolor(llabel, true);                      /*Enable re-coloring by commands in the text*/
  lv_label_set_text_fmt(llabel, "#0000FF Screen# %s", "Heart Rate"); 
  lv_label_set_align(llabel, LV_LABEL_ALIGN_CENTER);
  lv_obj_align(llabel, NULL, LV_ALIGN_CENTER, 0, -50);

  lhrs = lv_label_create(lv_scr_act(), NULL);
  lv_label_set_recolor(lhrs, true);                      /*Enable re-coloring by commands in the text*/
  lv_label_set_text_fmt(lhrs, "#00FF00 H.RateS # %i\n#0000FF A.LightS # %i\n#FF0000 H.Rate # %i", 0, 0, 0); 
  lv_label_set_align(lhrs, LV_LABEL_ALIGN_CENTER);
  lv_obj_align(lhrs, NULL, LV_ALIGN_CENTER, 0, 0);

}

HeartRate::~HeartRate() {
  hrs.SetEnable();

  lv_obj_clean(lv_scr_act());
}

bool HeartRate::Refresh() {

  uint32_t hrs_val = hrs.ReadHeartRateSensor();
  uint32_t als_val = hrs.ReadAmbientLightSensor();  
  uint8_t hrv = hrs.GetHeartRate();
  lv_label_set_text_fmt(lhrs, "#00FF00 H.RateS # %i\n#0000FF A.LightS # %i\n#FF0000 H.Rate # %i", hrs_val, als_val, hrv); 

  return running;
}

bool HeartRate::OnButtonPushed() {
  running = false;
  return true;
}
