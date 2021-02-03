#include "InfiniPaint.h"
#include "../DisplayApp.h"
#include "../LittleVgl.h"

using namespace Pinetime::Applications::Screens;
//extern lv_font_t jetbrains_mono_extrabold_compressed;
//extern lv_font_t jetbrains_mono_bold_20;

InfiniPaint::InfiniPaint(Pinetime::Applications::DisplayApp* app, Pinetime::Components::LittleVgl& lvgl) : Screen(app), lvgl{lvgl} {
  app->SetTouchMode(DisplayApp::TouchModes::Polling);
  std::fill(b, b + bufferSize, LV_COLOR_WHITE);

  lv_obj_t * backgroundLabel = lv_label_create(lv_scr_act(), nullptr);
  lv_label_set_long_mode(backgroundLabel, LV_LABEL_LONG_CROP);
  lv_obj_set_size(backgroundLabel, 240, 240);
  lv_obj_set_pos(backgroundLabel, 0, 0);
  lv_label_set_text_static(backgroundLabel, "");
  
}

InfiniPaint::~InfiniPaint() {
  // Reset the touchmode
  app->SetTouchMode(DisplayApp::TouchModes::Gestures);
  lv_obj_clean(lv_scr_act());
}

bool InfiniPaint::Refresh() {
  return running;
}

bool InfiniPaint::OnButtonPushed() {
  running = false;
  return true;
}

bool InfiniPaint::OnTouchEvent(Pinetime::Applications::TouchEvents event) {
  return true;
}

bool InfiniPaint::OnTouchEvent(uint16_t x, uint16_t y) {
  lv_area_t area;
  area.x1 = x - (width / 2);
  area.y1 = y - (height / 2);
  area.x2 = x + (width / 2) - 1;
  area.y2 = y + (height / 2) - 1;
  lvgl.SetFullRefresh(Components::LittleVgl::FullRefreshDirections::None);
  lvgl.FlushDisplay(&area, b);
  return true;
}

