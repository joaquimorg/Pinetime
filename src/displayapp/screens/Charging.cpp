#include "Charging.h"
#include "../DisplayApp.h"
#include "board_config.h"
#include "components/battery/BatteryController.h"

using namespace Pinetime::Applications::Screens;

LV_IMG_DECLARE(icon_charging);

void CHTimerCallback(TimerHandle_t xTimer) {

  auto charging = static_cast<Charging *>(pvTimerGetTimerID(xTimer));
  charging->Reading();
}

Charging::Charging(
    Pinetime::Applications::DisplayApp *app, 
    Pinetime::Controllers::Battery& batteryController) : 
  Screen(app),
  batteryController{batteryController}
{

  batteryPercent = batteryController.PercentRemaining();
  batteryVoltage = batteryController.Voltage();

  lv_obj_t * charging_ico = lv_img_create(lv_scr_act(), NULL);
  lv_img_set_src(charging_ico, &icon_charging);  
  lv_obj_align(charging_ico, NULL, LV_ALIGN_CENTER, 0, -55);

  charging_bar = lv_bar_create(lv_scr_act(), NULL);
  lv_obj_set_size(charging_bar, 200, 15);
  lv_bar_set_range(charging_bar, 0, 100);
  lv_obj_align(charging_bar, NULL, LV_ALIGN_CENTER, 0, 0);
  lv_bar_set_anim_time(charging_bar, 2000);
  lv_obj_set_style_local_radius(charging_bar, LV_BAR_PART_BG, LV_STATE_DEFAULT, LV_RADIUS_CIRCLE);
  lv_obj_set_style_local_bg_color(charging_bar, LV_BAR_PART_BG, LV_STATE_DEFAULT, lv_color_hex(0x222222));
  lv_obj_set_style_local_bg_opa(charging_bar, LV_BAR_PART_BG, LV_STATE_DEFAULT, LV_OPA_100);
  lv_obj_set_style_local_bg_color(charging_bar, LV_BAR_PART_INDIC , LV_STATE_DEFAULT, lv_color_hex(0xFF0000));
  lv_bar_set_value(charging_bar, batteryPercent, LV_ANIM_OFF);

  llabel = lv_label_create(lv_scr_act(), NULL);  
  lv_label_set_text_fmt(llabel,"(%i)  %i%% / %.2f volts", readc, batteryPercent, batteryVoltage);
  lv_label_set_align(llabel, LV_LABEL_ALIGN_CENTER);
  lv_obj_align(llabel, NULL, LV_ALIGN_CENTER, 0, 60);

  lv_obj_t * backgroundLabel = lv_label_create(lv_scr_act(), nullptr);
  lv_label_set_long_mode(backgroundLabel, LV_LABEL_LONG_CROP);
  lv_obj_set_size(backgroundLabel, 240, 240);
  lv_obj_set_pos(backgroundLabel, 0, 0);
  lv_label_set_text(backgroundLabel, "");

  chTimer = xTimerCreate ("chTimer", pdMS_TO_TICKS( 5000 ), pdTRUE, this, CHTimerCallback);
  xTimerStart(chTimer, 0);

}

void Charging::Reading() {
  batteryController.Update();
  readc++;
}

Charging::~Charging() {
  xTimerStop(chTimer, 0);
  lv_obj_clean(lv_scr_act());
}

bool Charging::Refresh() {

  animation +=1;

  if (animation > batteryPercent) {
    animation = 0;
  }

  lv_bar_set_value(charging_bar, animation, LV_ANIM_OFF);
    
  if ( old_readc != readc ) {
    batteryPercent = batteryController.PercentRemaining();
    batteryVoltage = batteryController.Voltage();
    lv_label_set_text_fmt(llabel,"(%i)  %i%% / %.2f volts", readc, batteryPercent, batteryVoltage);
    old_readc = readc;
  }

  return running;
}

bool Charging::OnButtonPushed() {
  running = false;
  return true;
}
