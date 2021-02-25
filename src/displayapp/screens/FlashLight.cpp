#include "FlashLight.h"
#include "../DisplayApp.h"
#include "board_config.h"
#include "Symbols.h"

using namespace Pinetime::Applications::Screens;


FlashLight::FlashLight(
    Pinetime::Applications::DisplayApp *app,
    Controllers::BrightnessController& brightness) : 
  Screen(app),
  brightness{brightness}
  
{
  brightness.Backup();
  brightness.Set(Controllers::BrightnessController::Levels::High);
  // Set the background
  lv_obj_set_style_local_bg_color(lv_scr_act(), LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0xFFFFFF));
  
  lv_obj_t * flashLight = lv_label_create(lv_scr_act(), NULL);  
  lv_obj_set_style_local_text_color(flashLight, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0x000000));
  lv_obj_set_style_local_text_font(flashLight, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_font_sys_48);
  lv_label_set_text_static(flashLight, Symbols::highlight);
  lv_obj_align(flashLight, NULL, LV_ALIGN_CENTER, 0, 0);
}


FlashLight::~FlashLight() {
  lv_obj_clean(lv_scr_act());
  lv_obj_set_style_local_bg_color(lv_scr_act(), LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0x000000));
  brightness.Restore();
}


bool FlashLight::Refresh() {  
  return running;
}

bool FlashLight::OnTouchEvent(Pinetime::Applications::TouchEvents event) {
  return true;
}

bool FlashLight::OnButtonPushed() {
  running = false;
  app->StartApp(Apps::Clock, DisplayApp::FullRefreshDirections::None);
  return true;
}
