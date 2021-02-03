#include "Settings.h"
#include <lvgl/lvgl.h>
#include "../DisplayApp.h"
#include "drivers/BMA421.h"
#include "board_config.h"
#include <libraries/gpiote/app_gpiote.h>
#include "components/battery/BatteryController.h"
#include <mdk/nrf.h>

using namespace Pinetime::Applications::Screens;


namespace {
  static void ButtonEventHandler(lv_obj_t * obj, lv_event_t event)
  {
    Settings* screen = static_cast<Settings *>(obj->user_data);
    screen->OnButtonEvent(obj, event);
  }

}

Settings::Settings(
  Pinetime::Applications::DisplayApp *app, 
  Controllers::Battery& batteryController) : 
  Screen(app), 
  batteryController{batteryController}
{

  llabel = lv_label_create(lv_scr_act(), NULL);
  lv_label_set_recolor(llabel, true);                      /*Enable re-coloring by commands in the text*/
  lv_label_set_text_fmt(llabel,     "#0000FF Screen# %s", "Settings"); 
  lv_label_set_align(llabel, LV_LABEL_ALIGN_CENTER);
  lv_obj_align(llabel, NULL, LV_ALIGN_CENTER, 0, 0);

  buttonPwrOff = lv_btn_create(lv_scr_act(), nullptr);
  buttonPwrOff->user_data = this;
  lv_obj_align(buttonPwrOff, nullptr, LV_ALIGN_IN_BOTTOM_MID, 0, 0);
  lv_obj_set_event_cb(buttonPwrOff, ButtonEventHandler);

  lv_obj_t* labelButtonPwrOff = lv_label_create(buttonPwrOff, nullptr);
  lv_label_set_recolor(labelButtonPwrOff, true);
  lv_label_set_text_static(labelButtonPwrOff, "#ff0000 Reset#");

  lv_obj_t * backgroundLabel = lv_label_create(lv_scr_act(), nullptr);
  lv_label_set_long_mode(backgroundLabel, LV_LABEL_LONG_CROP);
  lv_obj_set_size(backgroundLabel, 240, 240);
  lv_obj_set_pos(backgroundLabel, 0, 0);
  lv_label_set_text_static(backgroundLabel, "");

}

Settings::~Settings() {
  lv_obj_clean(lv_scr_act());
}

bool Settings::Refresh() {
  
  return running;
}

bool Settings::OnButtonPushed() {
  running = false;
  return true;
}


void Settings::OnButtonEvent(lv_obj_t *object, lv_event_t event) {
  if(object == buttonPwrOff && event == LV_EVENT_PRESSED) {
    NVIC_SystemReset();
  }
}