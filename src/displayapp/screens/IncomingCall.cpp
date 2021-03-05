#include "IncomingCall.h"
#include <lvgl/lvgl.h>
#include "Symbols.h"
#include "../DisplayApp.h"

using namespace Pinetime::Applications::Screens;

namespace {
  static void ButtonEventHandler(lv_obj_t * obj, lv_event_t event) {
    IncomingCall* screen = static_cast<IncomingCall *>(obj->user_data);
    screen->OnButtonEvent(obj, event);
  }

}

IncomingCall::IncomingCall(
  Pinetime::Applications::DisplayApp *app, 
  Pinetime::Controllers::CallNotificationManager& callNotificationManager,
  Pinetime::Controllers::AlertNotificationService& alertNotificationService) : 
  Screen(app),
  callNotificationManager{callNotificationManager},
  alertNotificationService{alertNotificationService}
{

  Controllers::CallNotificationManager::Notification notifCall = callNotificationManager.Get();

  lv_obj_t * callNumber = lv_label_create(lv_scr_act(), NULL);                     /*Enable re-coloring by commands in the text*/
  lv_label_set_text_fmt(callNumber, "%s", notifCall.number.data()); 
  lv_obj_set_style_local_text_color(callNumber, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_YELLOW);
  lv_label_set_align(callNumber, LV_LABEL_ALIGN_CENTER);
  lv_obj_align(callNumber, NULL, LV_ALIGN_CENTER, 0, -75);

  lv_obj_t * callName = lv_label_create(lv_scr_act(), NULL);                     /*Enable re-coloring by commands in the text*/
  lv_label_set_text_fmt(callName, "%s", notifCall.callername.data()); 
  lv_label_set_align(callName, LV_LABEL_ALIGN_CENTER);
  lv_obj_align(callName, NULL, LV_ALIGN_CENTER, 0, -40);

  lv_obj_t * callInfo = lv_label_create(lv_scr_act(), NULL);                     /*Enable re-coloring by commands in the text*/
  lv_label_set_text_fmt(callInfo, "Incoming call"); 
  lv_obj_set_style_local_text_color(callInfo, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_GREEN);
  lv_label_set_align(callInfo, LV_LABEL_ALIGN_CENTER);
  lv_obj_align(callInfo, NULL, LV_ALIGN_CENTER, 0, -10);


  lv_obj_t * lbl_btn;

  btn1 = lv_btn_create(lv_scr_act(), NULL);
  btn1->user_data = this;
  lv_obj_set_event_cb(btn1, ButtonEventHandler);
  lv_obj_align(btn1, NULL, LV_ALIGN_CENTER, -60, 55);  
  lv_obj_set_style_local_radius(btn1, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, 20);
  lv_obj_set_style_local_bg_color(btn1, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0x990000));
  lv_btn_set_fit2(btn1, LV_FIT_TIGHT, LV_FIT_TIGHT);  

  lbl_btn = lv_label_create(btn1, NULL);
  lv_obj_set_style_local_text_font(lbl_btn, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_font_sys_48);
  lv_label_set_text_static(lbl_btn, Symbols::PhoneOff);


  btn2 = lv_btn_create(lv_scr_act(), NULL);
  btn2->user_data = this;
  lv_obj_set_event_cb(btn2, ButtonEventHandler);
  lv_obj_align(btn2, NULL, LV_ALIGN_CENTER, 60, 55);  
  lv_obj_set_style_local_radius(btn2, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, 20);
  lv_obj_set_style_local_bg_color(btn2, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0x404040));
  lv_btn_set_fit2(btn2, LV_FIT_TIGHT, LV_FIT_TIGHT);  

  lbl_btn = lv_label_create(btn2, NULL);
  lv_obj_set_style_local_text_font(lbl_btn, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_font_sys_48);
  lv_label_set_text_static(lbl_btn, Symbols::phoneDisabled);


  lv_obj_t * backgroundLabel = lv_label_create(lv_scr_act(), nullptr);
  lv_label_set_long_mode(backgroundLabel, LV_LABEL_LONG_CROP);
  lv_obj_set_size(backgroundLabel, 240, 240);
  lv_obj_set_pos(backgroundLabel, 0, 0);
  lv_label_set_text_static(backgroundLabel, "");

}

IncomingCall::~IncomingCall() {
  lv_obj_clean(lv_scr_act());
}

bool IncomingCall::Refresh() {
  return running;
}

void IncomingCall::OnButtonEvent(lv_obj_t *object, lv_event_t event) {
  if(object == btn1 && event == LV_EVENT_PRESSED) {
    alertNotificationService.RejectIncomingCall();
    running  = false;
  } else if(object == btn2 && event == LV_EVENT_PRESSED) {
    alertNotificationService.MuteIncomingCall();
    running  = false;
  }

}
