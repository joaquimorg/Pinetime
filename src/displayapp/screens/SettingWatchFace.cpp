#include "SettingWatchFace.h"
#include <lvgl/lvgl.h>
#include "../DisplayApp.h"
#include "Symbols.h"

using namespace Pinetime::Applications::Screens;

namespace {
  static void event_handler(lv_obj_t * obj, lv_event_t event) {
    SettingWatchFace* screen = static_cast<SettingWatchFace *>(obj->user_data);
    screen->UpdateSelected(obj, event);
  }
}

SettingWatchFace::SettingWatchFace(
  Pinetime::Applications::DisplayApp *app, Pinetime::Controllers::Settings &settingsController) :
  Screen(app),
  settingsController{settingsController}
{

  lv_obj_t * container1 = lv_cont_create(lv_scr_act(), nullptr);

  //lv_obj_set_style_local_bg_color(container1, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0x111111));
  lv_obj_set_style_local_bg_opa(container1, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_TRANSP);
  lv_obj_set_style_local_pad_all(container1, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 10);
  lv_obj_set_style_local_pad_inner(container1, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 5);
  lv_obj_set_style_local_border_width(container1, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 0);

  lv_obj_set_pos(container1, 20, 60);
  lv_obj_set_width(container1, LV_HOR_RES - 30);
  lv_obj_set_height(container1, LV_VER_RES - 60);
  //lv_obj_set_auto_realign(container1, true);                  
  //lv_obj_align_origo(container1, NULL, LV_ALIGN_CENTER, 0, 0);
  //lv_cont_set_fit(container1, LV_FIT_MAX);
  lv_cont_set_layout(container1, LV_LAYOUT_COLUMN_LEFT);

  lv_obj_t * title = lv_label_create(lv_scr_act(), NULL);  
  lv_label_set_text_static(title,"Default watch face");
  lv_label_set_align(title, LV_LABEL_ALIGN_CENTER);
  lv_obj_align(title, lv_scr_act(), LV_ALIGN_IN_TOP_MID, 10, 15);

  lv_obj_t * icon = lv_label_create(lv_scr_act(), NULL);
  lv_obj_set_style_local_text_color(icon, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_ORANGE);
  lv_obj_set_style_local_text_font(icon, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_font_sys_20);
  lv_label_set_text_static(icon, Symbols::wface);
  lv_label_set_align(icon, LV_LABEL_ALIGN_CENTER);
  lv_obj_align(icon, title, LV_ALIGN_OUT_LEFT_MID, -10, 0);

  
  cbTimeOut[0] = lv_checkbox_create(container1, NULL);
  lv_checkbox_set_text_static(cbTimeOut[0], "\tDigital face");
  cbTimeOut[0]->user_data = this;
  lv_obj_set_event_cb(cbTimeOut[0], event_handler);  
  if (settingsController.GetClockFace() == 0 ) {
    lv_checkbox_set_checked(cbTimeOut[0], true);
  }

  cbTimeOut[1] = lv_checkbox_create(container1, NULL);
  lv_checkbox_set_text_static(cbTimeOut[1], "\tAnalog face");
  cbTimeOut[1]->user_data = this;
  lv_obj_set_event_cb(cbTimeOut[1], event_handler);  
  if (settingsController.GetClockFace() == 1 ) {
    lv_checkbox_set_checked(cbTimeOut[1], true);
  }

  cbTimeOut[2] = lv_checkbox_create(container1, NULL);
  lv_checkbox_set_text_static(cbTimeOut[2], "\tBig circle face");
  cbTimeOut[2]->user_data = this;
  lv_obj_set_event_cb(cbTimeOut[2], event_handler);
  if (settingsController.GetClockFace() == 2 ) {
    lv_checkbox_set_checked(cbTimeOut[2], true);
  }

  cbTimeOut[3] = lv_checkbox_create(container1, NULL);
  lv_checkbox_set_text_static(cbTimeOut[3], "\tPong game face");
  cbTimeOut[3]->user_data = this;
  lv_obj_set_event_cb(cbTimeOut[3], event_handler);
  if (settingsController.GetClockFace() == 3 ) {
    lv_checkbox_set_checked(cbTimeOut[3], true);
  }

}

SettingWatchFace::~SettingWatchFace() {
  lv_obj_clean(lv_scr_act());
}

bool SettingWatchFace::Refresh() {
  return running;
}


void SettingWatchFace::UpdateSelected(lv_obj_t *object, lv_event_t event) {
  if(event == LV_EVENT_VALUE_CHANGED) {
    for(int i = 0; i < 4; i++) {
      if ( object == cbTimeOut[i] ) {
        lv_checkbox_set_checked(cbTimeOut[i], true);
        
        settingsController.SetClockFace(i);

      } else {
        lv_checkbox_set_checked(cbTimeOut[i], false);
      }
    }
  }
}