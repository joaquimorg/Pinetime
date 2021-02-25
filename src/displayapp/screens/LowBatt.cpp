#include "LowBatt.h"
#include "../DisplayApp.h"
#include "board_config.h"
#include "components/battery/BatteryController.h"

using namespace Pinetime::Applications::Screens;

LV_IMG_DECLARE(icon_low_battery);

static void lv_update_task(struct _lv_task_t *task) {  
  auto user_data = static_cast<LowBatt *>(task->user_data);
  user_data->UpdateScreen();
}


LowBatt::LowBatt(
    Pinetime::Applications::DisplayApp *app, 
    Pinetime::Controllers::Battery& batteryController) : 
  Screen(app),
  batteryController{batteryController}
{

  batteryPercent = batteryController.PercentRemaining();
  batteryVoltage = batteryController.Voltage();

  lv_obj_t * low_battery_ico = lv_img_create(lv_scr_act(), NULL);
  lv_img_set_src(low_battery_ico, &icon_low_battery);  
  lv_obj_align(low_battery_ico, NULL, LV_ALIGN_CENTER, 0, -65);

  lv_obj_t * status = lv_label_create(lv_scr_act(), NULL);  
  lv_label_set_text_static(status,"Battery is low,\nplease charge");
  lv_label_set_align(status, LV_LABEL_ALIGN_CENTER);
  lv_obj_align(status, NULL, LV_ALIGN_CENTER, 0, -10);
  
  percent = lv_label_create(lv_scr_act(), NULL);
  lv_obj_set_style_local_text_font(percent, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_font_clock_42);
  if ( batteryPercent >= 0) {
    lv_label_set_text_fmt(percent,"%02i%%", batteryPercent);
  } else {
    lv_label_set_text(percent,"--%");
  }
  lv_label_set_align(percent, LV_LABEL_ALIGN_CENTER);
  lv_obj_align(percent, NULL, LV_ALIGN_CENTER, 0, 50);

  voltage = lv_label_create(lv_scr_act(), NULL);  
  lv_obj_set_style_local_text_color(voltage, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0xC6A600));
  lv_label_set_text_fmt(voltage,"%.2f volts", batteryVoltage);
  lv_label_set_align(voltage, LV_LABEL_ALIGN_CENTER);
  lv_obj_align(voltage, NULL, LV_ALIGN_CENTER, 0, 95);

  lv_obj_t * backgroundLabel = lv_label_create(lv_scr_act(), nullptr);
  lv_label_set_long_mode(backgroundLabel, LV_LABEL_LONG_CROP);
  lv_obj_set_size(backgroundLabel, 240, 240);
  lv_obj_set_pos(backgroundLabel, 0, 0);
  lv_label_set_text_static(backgroundLabel, "");

  taskUpdate = lv_task_create(lv_update_task, 500000, LV_TASK_PRIO_MID, this);

}


LowBatt::~LowBatt() {
  lv_task_del(taskUpdate);
  lv_obj_clean(lv_scr_act());
}

void LowBatt::UpdateScreen() {

  batteryController.Update();

  batteryPercent = batteryController.PercentRemaining();
  batteryVoltage = batteryController.Voltage();

  if ( batteryPercent >= 0 ) {    
    lv_label_set_text_fmt(percent,"%02i%%", batteryPercent);
  } else {
    lv_label_set_text(percent,"--%");
  }

  lv_label_set_text_fmt(voltage,"%.2f volts", batteryVoltage);
 
}

bool LowBatt::Refresh() {
  
  return running;
}

bool LowBatt::OnButtonPushed() {
  running = false;
  return true;
}
