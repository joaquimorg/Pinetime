#include "SettingWakeUp.h"
#include <lvgl/lvgl.h>
#include "../DisplayApp.h"
#include "Symbols.h"
#include "drivers/Cst816s.h"

using namespace Pinetime::Applications::Screens;

namespace {
  static void event_handler(lv_obj_t * obj, lv_event_t event) {
    SettingWakeUp* screen = static_cast<SettingWakeUp *>(obj->user_data);
    screen->UpdateSelected(obj, event);
  }
}

SettingWakeUp::SettingWakeUp(
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
  lv_label_set_text_static(title,"Display Wake Up");
  lv_label_set_align(title, LV_LABEL_ALIGN_CENTER);
  lv_obj_align(title, lv_scr_act(), LV_ALIGN_IN_TOP_MID, 15, 15);

  lv_obj_t * icon = lv_label_create(lv_scr_act(), NULL);
  lv_obj_set_style_local_text_color(icon, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_ORANGE);
  lv_obj_set_style_local_text_font(icon, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_font_sys_20);
  lv_label_set_text_static(icon, Symbols::clock);
  lv_label_set_align(icon, LV_LABEL_ALIGN_CENTER);
  lv_obj_align(icon, title, LV_ALIGN_OUT_LEFT_MID, -10, 0);

  
  cbWakeUp[0] = lv_checkbox_create(container1, NULL);
  lv_checkbox_set_text_static(cbWakeUp[0], "\tNone");
  cbWakeUp[0]->user_data = this;
  lv_obj_set_event_cb(cbWakeUp[0], event_handler);  
  if (settingsController.getWakeUpTap() == Pinetime::Drivers::Cst816S::Gestures::None ) {
    lv_checkbox_set_checked(cbWakeUp[0], true);
  }

  cbWakeUp[1] = lv_checkbox_create(container1, NULL);
  lv_checkbox_set_text_static(cbWakeUp[1], "\tSingle Tap");
  cbWakeUp[1]->user_data = this;
  lv_obj_set_event_cb(cbWakeUp[1], event_handler);  
  if (settingsController.getWakeUpTap() == Pinetime::Drivers::Cst816S::Gestures::SingleTap ) {
    lv_checkbox_set_checked(cbWakeUp[1], true);
  }

  cbWakeUp[2] = lv_checkbox_create(container1, NULL);
  lv_checkbox_set_text_static(cbWakeUp[2], "\tDouble Tap");
  cbWakeUp[2]->user_data = this;
  lv_obj_set_event_cb(cbWakeUp[2], event_handler);  
  if (settingsController.getWakeUpTap() == Pinetime::Drivers::Cst816S::Gestures::DoubleTap ) {
    lv_checkbox_set_checked(cbWakeUp[2], true);
  }

}

SettingWakeUp::~SettingWakeUp() {
  lv_obj_clean(lv_scr_act());
  settingsController.SaveSettings();
}

bool SettingWakeUp::Refresh() {
  return running;
}


void SettingWakeUp::UpdateSelected(lv_obj_t *object, lv_event_t event) {
  if(event == LV_EVENT_VALUE_CHANGED) {
    for(int i = 0; i < 3; i++) {
      if ( object == cbWakeUp[i] ) {
        lv_checkbox_set_checked(cbWakeUp[i], true);
        
        if ( i == 0 ) { settingsController.setWakeUpTap(Pinetime::Drivers::Cst816S::Gestures::None); };
        if ( i == 1 ) { settingsController.setWakeUpTap(Pinetime::Drivers::Cst816S::Gestures::SingleTap); };
        if ( i == 2 ) { settingsController.setWakeUpTap(Pinetime::Drivers::Cst816S::Gestures::DoubleTap); };
        
      } else {
        lv_checkbox_set_checked(cbWakeUp[i], false);
      }
    }
  }
}