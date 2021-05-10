#include "SettingWatchFace.h"
#include <lvgl/lvgl.h>
#include "displayapp/DisplayApp.h"
#include "displayapp/screens/Screen.h"
#include "displayapp/screens/Symbols.h"
#include "displayapp/screens/Label.h"

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
  settingsController{settingsController},
  screens{app, 
          settingsController.GetClockFace(),
          {
                [this]() -> std::unique_ptr<Screen> { return CreateScreen("F:/wft1.bin", 0); },
                [this]() -> std::unique_ptr<Screen> { return CreateScreen("F:/wft2.bin", 1); },
                [this]() -> std::unique_ptr<Screen> { return CreateScreen("F:/wft3.bin", 2); },
                [this]() -> std::unique_ptr<Screen> { return CreateScreen("F:/wft4.bin", 3); },
                [this]() -> std::unique_ptr<Screen> { return CreateScreen("F:/wft5.bin", 4); },
                [this]() -> std::unique_ptr<Screen> { return CreateScreen("F:/wft6.bin", 5); },
                [this]() -> std::unique_ptr<Screen> { return CreateScreen("F:/wft7.bin", 6); },
                [this]() -> std::unique_ptr<Screen> { return CreateScreen("F:/wft8.bin", 7); },
                [this]() -> std::unique_ptr<Screen> { return CreateScreen("F:/wft9.bin", 8); },
          },
          Screens::ScreenListModes::UpDown
        } {
          selectedFace = settingsController.GetClockFace();
        }

SettingWatchFace::~SettingWatchFace() {
  lv_obj_clean(lv_scr_act());
  settingsController.SaveSettings();
}

bool SettingWatchFace::Refresh() {
  return running;
}

bool SettingWatchFace::OnTouchEvent(Pinetime::Applications::TouchEvents event) {
  return screens.OnTouchEvent(event);
}

std::unique_ptr<Screen> SettingWatchFace::CreateScreen(const char * imageName, uint8_t num) {
  lv_obj_t * title = lv_label_create(lv_scr_act(), NULL);  
  lv_label_set_text_static(title,"Select watch face");
  lv_label_set_align(title, LV_LABEL_ALIGN_CENTER);
  lv_obj_align(title, lv_scr_act(), LV_ALIGN_IN_TOP_MID, 10, 15);

  lv_obj_t * icon = lv_label_create(lv_scr_act(), NULL);
  lv_obj_set_style_local_text_color(icon, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_ORANGE);
  lv_obj_set_style_local_text_font(icon, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_font_sys_20);
  lv_label_set_text_static(icon, Symbols::wface);
  lv_label_set_align(icon, LV_LABEL_ALIGN_CENTER);
  lv_obj_align(icon, title, LV_ALIGN_OUT_LEFT_MID, -10, 0);

  lv_obj_t * image = lv_imgbtn_create(lv_scr_act(), nullptr);
  lv_imgbtn_set_src(image, LV_BTN_STATE_RELEASED, imageName);
  lv_obj_align(image, nullptr, LV_ALIGN_CENTER, 0, 20);
  lv_obj_set_style_local_image_recolor_opa(image, LV_IMGBTN_PART_MAIN, LV_STATE_PRESSED, LV_OPA_10);
  lv_obj_set_style_local_image_recolor(image, LV_IMGBTN_PART_MAIN, LV_STATE_PRESSED, LV_COLOR_BLACK);
  image->user_data = this;
  lv_obj_set_event_cb(image, event_handler);
  selectedFace = num;

  if ( settingsController.GetClockFace() == num ) {
    lv_obj_t * selected = lv_label_create(lv_scr_act(), NULL);
    lv_obj_set_style_local_text_color(selected, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_RED);
    lv_obj_set_style_local_text_font(selected, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_font_sys_20);
    lv_label_set_text_static(selected, Symbols::dot);
    lv_label_set_align(selected, LV_LABEL_ALIGN_CENTER);
    lv_obj_align(selected, image, LV_ALIGN_OUT_RIGHT_TOP, -2, -15);
  }

  return std::unique_ptr<Screen>(new Screens::Label(num, wfTotal, app, nullptr));
}

void SettingWatchFace::UpdateSelected(lv_obj_t *object, lv_event_t event) {
  if(event == LV_EVENT_PRESSED) {
    settingsController.SetClockFace(selectedFace);
    app->StartApp(Apps::Clock, DisplayApp::FullRefreshDirections::Up);
    running = false;
  }
}