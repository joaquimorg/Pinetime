#include "SettingDisplay.h"
#include <lvgl/lvgl.h>
#include "../DisplayApp.h"
#include "Symbols.h"

using namespace Pinetime::Applications::Screens;

namespace {
  static void event_handler(lv_obj_t * obj, lv_event_t event) {
    SettingDisplay* screen = static_cast<SettingDisplay *>(obj->user_data);
    screen->UpdateSelected(obj, event);
  }
}

SettingDisplay::SettingDisplay(
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

  lv_obj_set_pos(container1, 30, 60);
  lv_obj_set_width(container1, LV_HOR_RES - 50);
  lv_obj_set_height(container1, LV_VER_RES - 60);
  //lv_obj_set_auto_realign(container1, true);                  
  //lv_obj_align_origo(container1, NULL, LV_ALIGN_CENTER, 0, 0);
  //lv_cont_set_fit(container1, LV_FIT_MAX);
  lv_cont_set_layout(container1, LV_LAYOUT_COLUMN_LEFT);

  lv_obj_t * title = lv_label_create(lv_scr_act(), NULL);  
  lv_label_set_text_static(title,"Display timeout");
  lv_label_set_align(title, LV_LABEL_ALIGN_CENTER);
  lv_obj_align(title, lv_scr_act(), LV_ALIGN_IN_TOP_MID, 10, 15);

  lv_obj_t * icon = lv_label_create(lv_scr_act(), NULL);
  lv_obj_set_style_local_text_color(icon, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_ORANGE);
  lv_obj_set_style_local_text_font(icon, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_font_sys_20);
  lv_label_set_text_static(icon, Symbols::backLight);
  lv_label_set_align(icon, LV_LABEL_ALIGN_CENTER);
  lv_obj_align(icon, title, LV_ALIGN_OUT_LEFT_MID, -10, 0);
  
  cbTimeOut[0] = lv_checkbox_create(container1, NULL);
  lv_checkbox_set_text_static(cbTimeOut[0], "\t  5 seconds");
  cbTimeOut[0]->user_data = this;
  lv_obj_set_event_cb(cbTimeOut[0], event_handler);  
  if (settingsController.GetScreenTimeOut() == 5000 ) {
    lv_checkbox_set_checked(cbTimeOut[0], true);
  }

  cbTimeOut[1] = lv_checkbox_create(container1, NULL);
  lv_checkbox_set_text_static(cbTimeOut[1], "\t15 seconds");
  cbTimeOut[1]->user_data = this;
  lv_obj_set_event_cb(cbTimeOut[1], event_handler);  
  if (settingsController.GetScreenTimeOut() == 15000 ) {
    lv_checkbox_set_checked(cbTimeOut[1], true);
  }

  cbTimeOut[2] = lv_checkbox_create(container1, NULL);
  lv_checkbox_set_text_static(cbTimeOut[2], "\t20 seconds");
  cbTimeOut[2]->user_data = this;
  lv_obj_set_event_cb(cbTimeOut[2], event_handler);
  if (settingsController.GetScreenTimeOut() == 20000 ) {
    lv_checkbox_set_checked(cbTimeOut[2], true);
  }

  cbTimeOut[3] = lv_checkbox_create(container1, NULL);
  lv_checkbox_set_text_static(cbTimeOut[3], "\t30 seconds");
  cbTimeOut[3]->user_data = this;
  lv_obj_set_event_cb(cbTimeOut[3], event_handler);
  if (settingsController.GetScreenTimeOut() == 30000 ) {
    lv_checkbox_set_checked(cbTimeOut[3], true);
  }

  cbTimeOut[4] = lv_checkbox_create(container1, NULL);
  lv_checkbox_set_text_static(cbTimeOut[4], "\t5 minutes");
  cbTimeOut[4]->user_data = this;
  lv_obj_set_event_cb(cbTimeOut[4], event_handler);
  if (settingsController.GetScreenTimeOut() == 300000 ) {
    lv_checkbox_set_checked(cbTimeOut[4], true);
  }

}

SettingDisplay::~SettingDisplay() {
  lv_obj_clean(lv_scr_act());
  settingsController.SaveSettings();
}

bool SettingDisplay::Refresh() {
  return running;
}


void SettingDisplay::UpdateSelected(lv_obj_t *object, lv_event_t event) {
  if(event == LV_EVENT_VALUE_CHANGED) {
    for(int i = 0; i < 5; i++) {
      if ( object == cbTimeOut[i] ) {
        lv_checkbox_set_checked(cbTimeOut[i], true);
        
        if ( i == 0 ) { settingsController.SetScreenTimeOut(5000); };
        if ( i == 1 ) { settingsController.SetScreenTimeOut(15000); };
        if ( i == 2 ) { settingsController.SetScreenTimeOut(20000); };
        if ( i == 3 ) { settingsController.SetScreenTimeOut(30000); };
        if ( i == 4 ) { settingsController.SetScreenTimeOut(300000); };
        
        app->PushMessage(Applications::DisplayApp::Messages::UpdateTimeOut);

      } else {
        lv_checkbox_set_checked(cbTimeOut[i], false);
      }
    }
  }
}