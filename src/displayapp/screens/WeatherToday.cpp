#include "WeatherToday.h"
#include <lvgl/lvgl.h>
#include "../DisplayApp.h"
#include "Symbols.h"
#include "board_config.h"
#include "components/datetime/DateTimeController.h"

using namespace Pinetime::Applications::Screens;

static void lv_update_task(struct _lv_task_t *task) {  
  auto user_data = static_cast<WeatherToday *>(task->user_data);
  user_data->UpdateScreen();
}

/* deducted values:
    0 = tornado*
    1 = typhoon*
    2 = hurricane*
    3 = thunderstorm*
    4 = rain and snow*
    5 = unavailable(cold)*
    6 = freezing rain*
    7 = drizzle*
    8 = showers*
    9 = snow flurries
    10 = blowing snow
    11 = snow*
    12 = sleet*
    13 = foggy*
    14 = windy*
    15 = cloudy*
    16 = partly cloudy (night)
    17 = partly cloudy (day)*
    18 = clear night
    19 = sunny*
    20 = thundershower*
    21 = hot*
    22 = scattered thunders*
    23 = snow showers*
    24 = heavy snow*
 */


char const *WeatherToday::Icons[] = {
        "F:/meteo_tornado.bin", // 0 = tornado*
        "F:/meteo_tornado.bin", // 1 = typhoon*
        "F:/meteo_tornado.bin", // 2 = hurricane*
        "F:/meteo_thunderstorm.bin", // 3 = thunderstorm*
        "F:/meteo_hail.bin", // 4 = rain and snow*
        "F:/meteo_sun.bin", // 5 = unavailable(cold)*
        "F:/meteo_snowy.bin", // 6 = freezing rain*
        "F:/meteo_raindrop.bin", // 7 = drizzle*
        "F:/meteo_rain.bin", // 8 = showers*
        "F:/meteo_snowy.bin", // 9 = snow flurries
        "F:/meteo_snow.bin", // 10 = blowing snow
        "F:/meteo_snow.bin", // 11 = snow*
        "F:/meteo_rain.bin", // 12 = sleet*
        "F:/meteo_haze.bin", // 13 = foggy*
        "F:/meteo_windy.bin", // 14 = windy*
        "F:/meteo_cloudy.bin", // 15 = cloudy*
        "F:/meteo_cloud.bin", // 16 = partly cloudy (night)
        "F:/meteo_cloud.bin", // 17 = partly cloudy (day)*
        "F:/meteo_sun.bin", // 18 = clear night
        "F:/meteo_sun.bin", // 19 = sunny*
        "F:/meteo_storm.bin", // 20 = thundershower*
        "F:/meteo_heat.bin", // 21 = hot*
        "F:/meteo_lighting.bin", // 22 = scattered thunders*
        "F:/meteo_snowy.bin", // 23 = snow showers*
        "F:/meteo_snow.bin"  // 24 = heavy snow*        
};

char const *WeatherToday::WeatherDescription[] = {
        "Tornado",
        "Typhoon",
        "Hurricane",
        "Thunderstorm",
        "Rain and snow",
        "unavailable",
        "Freezing rain",
        "Drizzle",
        "Showers",
        "Snow flurries",
        "Blowing snow",
        "Snow",
        "Sleet",
        "Foggy",
        "Windy",
        "Cloudy",
        "Partly cloudy",
        "Partly cloudy",
        "Clear night",
        "Sunny",
        "Thundershower",
        "Hot",
        "Scattered thunders",
        "Snow showers",
        "Heavy snow"
};

WeatherToday::WeatherToday(
  Pinetime::Applications::DisplayApp *app, Controllers::Settings &settingsController) : 
  Screen(app),
  settingsController{settingsController}
{

  weather = settingsController.GetWeather();  

  DisplayWeather();

  lv_obj_t * backgroundLabel = lv_label_create(lv_scr_act(), nullptr);
  lv_label_set_long_mode(backgroundLabel, LV_LABEL_LONG_CROP);
  lv_obj_set_size(backgroundLabel, 240, 240);
  lv_obj_set_pos(backgroundLabel, 0, 0);
  lv_label_set_text_static(backgroundLabel, "");

  taskUpdate = lv_task_create(lv_update_task, 5000, LV_TASK_PRIO_LOW, this);

}

WeatherToday::~WeatherToday() {
  lv_task_del(taskUpdate);
  lv_obj_clean(lv_scr_act());
}

void WeatherToday::DisplayWeather() {

  currentTemp = lv_label_create(lv_scr_act(), nullptr);
  symbTemp = lv_label_create(lv_scr_act(), nullptr);
  symbNav = lv_label_create(lv_scr_act(), nullptr);
  iconAnimW = lv_img_create(lv_scr_act(), nullptr);
  iconWDesc = lv_label_create(lv_scr_act(), nullptr);
  location = lv_label_create(lv_scr_act(), nullptr);
  dateInfo = lv_label_create(lv_scr_act(), nullptr);
  maxmin = lv_label_create(lv_scr_act(), nullptr);

  lv_obj_set_style_local_text_color(currentTemp, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_CYAN);
  lv_obj_set_style_local_text_font(currentTemp, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_font_clock_76);
  lv_obj_set_style_local_text_color(symbTemp, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_CYAN);
  lv_obj_set_style_local_text_font(symbTemp, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_font_sys_20);
  lv_obj_set_style_local_text_color(iconWDesc, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_YELLOW);
  //lv_obj_set_style_local_text_color(symbNav, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_CYAN);
  lv_obj_set_style_local_text_font(symbNav, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_font_sys_20);
  lv_label_set_recolor(maxmin, true);
  lv_label_set_recolor(dateInfo, true);
  lv_label_set_recolor(location, true);

  lv_label_set_text_static(symbTemp, Symbols::dot);

  lv_label_set_text_static(symbNav, Symbols::expandMore);
  lv_obj_align(symbNav, nullptr, LV_ALIGN_IN_BOTTOM_MID, 0, 0);

  UpdateWeather();
}

void WeatherToday::UpdateWeather() {

  lv_label_set_text_fmt(currentTemp, "%d", weather.current.temp);
  lv_obj_align(currentTemp, nullptr, LV_ALIGN_CENTER, -50, -30);  
  
  lv_obj_align(symbTemp, currentTemp, LV_ALIGN_OUT_RIGHT_TOP, 0, 0);

  lv_img_set_src(iconAnimW, Icons[(uint8_t)weather.current.conditionCode]);
  lv_obj_align(iconAnimW, currentTemp, LV_ALIGN_OUT_RIGHT_MID, 40 + anim, 0);

  lv_label_set_text(iconWDesc, WeatherDescription[(uint8_t)weather.current.conditionCode]);
  lv_obj_align(iconWDesc, nullptr, LV_ALIGN_CENTER, 0, 25);

  lv_label_set_text_fmt(location, "%s", weather.location.data());
  lv_obj_align(location, nullptr, LV_ALIGN_IN_BOTTOM_MID, 0, -25);
  lv_label_set_align(location, LV_LABEL_ALIGN_CENTER);
  
  if ( settingsController.GetClockType() == Controllers::Settings::ClockType::H12 ) {
    lv_label_set_text_fmt(dateInfo, "Today #ADFF00 %s %d#", Controllers::DateTime::MonthsLow[(uint8_t)weather.month], weather.day);
  } else {
    lv_label_set_text_fmt(dateInfo, "Today #ADFF00 %d %s#", weather.day, Controllers::DateTime::MonthsLow[(uint8_t)weather.month]);
  }
  lv_obj_align(dateInfo, nullptr, LV_ALIGN_IN_TOP_MID, 0, 15);
  
  lv_label_set_text_fmt(maxmin, "#FF6C00 %d°# / #0094FF %d°#", weather.current.maxTemp, weather.current.minTemp);
  lv_obj_align(maxmin, nullptr, LV_ALIGN_CENTER, 0, 50);
  lv_label_set_align(maxmin, LV_LABEL_ALIGN_CENTER);
  
}

void WeatherToday::UpdateScreen() {  

  anim += animDelta;
  if(anim == 10 or anim == -10) animDelta = -animDelta;

  //animNav += animNavDelta;
  //if(animNav == 0 or animNav == 5) animNavDelta = -animNavDelta;
  ++animNav %= 5;
  //UpdateWeather();
  lv_obj_align(iconAnimW, currentTemp, LV_ALIGN_OUT_RIGHT_MID, 40 + anim, 0);
  lv_obj_align(symbNav, nullptr, LV_ALIGN_IN_BOTTOM_MID, 0, animNav);
}

bool WeatherToday::Refresh() { 
  return running;
}