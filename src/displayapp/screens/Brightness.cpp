#include "Brightness.h"
#include <lvgl/lvgl.h>

LV_IMG_DECLARE(icon_brightness);

using namespace Pinetime::Applications::Screens;

void slider_event_cb(lv_obj_t * slider, lv_event_t event) {
  if(event == LV_EVENT_VALUE_CHANGED) {
    auto* brightnessSlider = static_cast<Brightness*>(slider->user_data);
    brightnessSlider->OnValueChanged();
  }
}

Brightness::Brightness(Pinetime::Applications::DisplayApp *app, Controllers::BrightnessController& brightness) : Screen(app), brightness{brightness} {

  lv_obj_t * iBrightness = lv_img_create(lv_scr_act(), NULL);
  lv_img_set_src(iBrightness, &icon_brightness);  
  lv_obj_align(iBrightness, NULL, LV_ALIGN_CENTER, -60, -70);

  lv_obj_t* lBrightness = lv_label_create(lv_scr_act(), NULL);
  lv_obj_set_style_local_text_color(lBrightness, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0xffc107));
  lv_label_set_text_static(lBrightness, "Brightness");   
  lv_obj_align(lBrightness, iBrightness, LV_ALIGN_OUT_RIGHT_MID, 20, 0);

  slider = lv_slider_create(lv_scr_act(), nullptr);
  lv_obj_set_user_data(slider, this);
  lv_obj_set_width(slider, 220);
  lv_obj_align(slider, nullptr, LV_ALIGN_CENTER, 0, 0);
  lv_obj_set_event_cb(slider, slider_event_cb);
  lv_slider_set_range(slider, 0, 4);
  lv_slider_set_value(slider, LevelToInt(brightness.Level()), LV_ANIM_OFF);

  slider_label = lv_label_create(lv_scr_act(), nullptr);
  lv_label_set_text(slider_label, LevelToString(brightness.Level()));
  lv_obj_set_auto_realign(slider_label, true);
  lv_obj_align(slider_label, slider, LV_ALIGN_OUT_BOTTOM_MID, 0, 30);

  lv_obj_t *backgroundLabel = lv_label_create(lv_scr_act(), nullptr);
  //backgroundLabel->user_data = this;
  lv_label_set_long_mode(backgroundLabel, LV_LABEL_LONG_CROP);
  lv_obj_set_size(backgroundLabel, 240, 240);
  lv_obj_set_pos(backgroundLabel, 0, 0);
  lv_label_set_text_static(backgroundLabel, "");

}

Brightness::~Brightness() {
  lv_obj_clean(lv_scr_act());
}

bool Brightness::Refresh() {
  return running;
}

bool Brightness::OnButtonPushed() {
  running = false;
  return true;
}

const char *Brightness::LevelToString(Pinetime::Controllers::BrightnessController::Levels level) {
  switch(level) {
    case Pinetime::Controllers::BrightnessController::Levels::Off: return "Off";
    case Pinetime::Controllers::BrightnessController::Levels::Low: return "Low";
    case Pinetime::Controllers::BrightnessController::Levels::Medium: return "Medium";
    case Pinetime::Controllers::BrightnessController::Levels::High: return "High";
    default : return "???";
  }
}

void Brightness::OnValueChanged() {
  SetValue(lv_slider_get_value(slider));
}

void Brightness::SetValue(uint8_t value) {
  switch(value) {
    case 0: brightness.Set(Controllers::BrightnessController::Levels::Low); break;
    case 1: brightness.Set(Controllers::BrightnessController::Levels::Medium); break;
    case 2: brightness.Set(Controllers::BrightnessController::Levels::High); break;
  }
  lv_label_set_text(slider_label, LevelToString(brightness.Level()));
}

uint8_t Brightness::LevelToInt(Pinetime::Controllers::BrightnessController::Levels level) {
  switch(level) {    
    case Pinetime::Controllers::BrightnessController::Levels::Low: return 0;
    case Pinetime::Controllers::BrightnessController::Levels::Medium: return 1;
    case Pinetime::Controllers::BrightnessController::Levels::High: return 2;
    default : return 0;
  }
}

bool Brightness::OnTouchEvent(Pinetime::Applications::TouchEvents event) {
  switch(event) {
    case TouchEvents::SwipeLeft:      
      brightness.Lower();
      if ( brightness.Level() == Pinetime::Controllers::BrightnessController::Levels::Off) {
        brightness.Set(Controllers::BrightnessController::Levels::Low);
      }
      SetValue();
      return true;
    case TouchEvents::SwipeRight:
      brightness.Higher();
      SetValue();
      return true;
    default:
      return false;
  }
}

void Brightness::SetValue() {
  lv_slider_set_value(slider, LevelToInt(brightness.Level()), LV_ANIM_OFF);
  lv_label_set_text(slider_label, LevelToString(brightness.Level()));
}
