#include "WeatherForecast.h"
#include <lvgl/lvgl.h>
#include "../DisplayApp.h"
#include "Symbols.h"
#include "WeatherToday.h"
#include "components/settings/Settings.h"

using namespace Pinetime::Applications::Screens;

static void lv_update_task(struct _lv_task_t *task) {  
  auto user_data = static_cast<WeatherForecast *>(task->user_data);
  user_data->UpdateScreen();
}

WeatherForecast::WeatherForecast(
  Pinetime::Applications::DisplayApp *app, Controllers::Settings &settingsController) : 
  Screen(app),
  settingsController{settingsController}
{

  weather = settingsController.GetWeather();

  iconW[0] = lv_img_create(lv_scr_act(), nullptr);
  lv_img_set_src(iconW[0], WeatherToday::Icons[(uint8_t)weather.forecast[0].conditionCode]);
  lv_obj_align(iconW[0], nullptr, LV_ALIGN_CENTER, 70, -75);  
  iconW[1] = lv_img_create(lv_scr_act(), nullptr);
  lv_img_set_src(iconW[1], WeatherToday::Icons[(uint8_t)weather.forecast[1].conditionCode]);
  lv_obj_align(iconW[1], nullptr, LV_ALIGN_CENTER, 70, 0);
  iconW[2] = lv_img_create(lv_scr_act(), nullptr);
  lv_img_set_src(iconW[2], WeatherToday::Icons[(uint8_t)weather.forecast[2].conditionCode]);
  lv_obj_align(iconW[2], nullptr, LV_ALIGN_CENTER, 70, 75); 

  // ----------------------------------------------------------------

  maxmin[0] = lv_label_create(lv_scr_act(), nullptr);
  lv_label_set_recolor(maxmin[0], true);
  lv_label_set_text_fmt(maxmin[0], "#FF6C00 %d°# / #0094FF %d°#", weather.forecast[0].maxTemp, weather.forecast[0].minTemp);
  lv_obj_align(maxmin[0], nullptr, LV_ALIGN_IN_LEFT_MID, 10, -80);
  maxmin[1] = lv_label_create(lv_scr_act(), nullptr);
  lv_label_set_recolor(maxmin[1], true);
  lv_label_set_text_fmt(maxmin[1], "#FF6C00 %d°# / #0094FF %d°#", weather.forecast[1].maxTemp, weather.forecast[1].minTemp);
  lv_obj_align(maxmin[1], nullptr, LV_ALIGN_IN_LEFT_MID, 10, -10);
  maxmin[2] = lv_label_create(lv_scr_act(), nullptr);
  lv_label_set_recolor(maxmin[2], true);
  lv_label_set_text_fmt(maxmin[2], "#FF6C00 %d°# / #0094FF %d°#", weather.forecast[2].maxTemp, weather.forecast[2].minTemp);
  lv_obj_align(maxmin[2], nullptr, LV_ALIGN_IN_LEFT_MID, 10, 60);

  // ----------------------------------------------------------------

  iconWDesc[0] = lv_label_create(lv_scr_act(), nullptr);
  //lv_obj_set_style_local_text_color(iconWDesc[0], LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_YELLOW);
  lv_label_set_text(iconWDesc[0], WeatherToday::WeatherDescription[(uint8_t)weather.forecast[0].conditionCode]);
  lv_obj_align(iconWDesc[0], nullptr, LV_ALIGN_IN_LEFT_MID, 10, -50);

  iconWDesc[1] = lv_label_create(lv_scr_act(), nullptr);
  //lv_obj_set_style_local_text_color(iconWDesc[1], LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_YELLOW);
  lv_label_set_text(iconWDesc[1], WeatherToday::WeatherDescription[(uint8_t)weather.forecast[1].conditionCode]);
  lv_obj_align(iconWDesc[1], nullptr, LV_ALIGN_IN_LEFT_MID, 10, 20);

  iconWDesc[2] = lv_label_create(lv_scr_act(), nullptr);
  //lv_obj_set_style_local_text_color(iconWDesc[2], LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_YELLOW);
  lv_label_set_text(iconWDesc[2], WeatherToday::WeatherDescription[(uint8_t)weather.forecast[2].conditionCode]);
  lv_obj_align(iconWDesc[2], nullptr, LV_ALIGN_IN_LEFT_MID, 10, 90);


  symbNav = lv_label_create(lv_scr_act(), nullptr);
  lv_obj_set_style_local_text_font(symbNav, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_font_sys_20);
  lv_label_set_text_static(symbNav, Symbols::expandLess);
  lv_obj_align(symbNav, nullptr, LV_ALIGN_IN_TOP_MID, 0, 0);

  lv_obj_t * backgroundLabel = lv_label_create(lv_scr_act(), nullptr);
  lv_label_set_long_mode(backgroundLabel, LV_LABEL_LONG_CROP);
  lv_obj_set_size(backgroundLabel, 240, 240);
  lv_obj_set_pos(backgroundLabel, 0, 0);
  lv_label_set_text_static(backgroundLabel, "");

  taskUpdate = lv_task_create(lv_update_task, 5000, LV_TASK_PRIO_LOW, this);

}

WeatherForecast::~WeatherForecast() {
  lv_task_del(taskUpdate);
  lv_obj_clean(lv_scr_act());
}

bool WeatherForecast::Refresh() {
  return running;
}

void WeatherForecast::UpdateScreen() {
  ++animNav %= 5;
  lv_obj_align(symbNav, nullptr, LV_ALIGN_IN_TOP_MID, 0, 5 - animNav);
}