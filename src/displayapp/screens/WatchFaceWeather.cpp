#include "WatchFaceWeather.h"
#include "BatteryIcon.h"
#include "BleIcon.h"
#include "Symbols.h"
#include "NotificationIcon.h"
#include "resources_config.h"
#include "WeatherToday.h"

using namespace Pinetime::Applications::Screens;

static void lv_update_task(struct _lv_task_t *task) {
  auto user_data = static_cast<WatchFaceWeather *>(task->user_data);
  user_data->UpdateScreen();
}


char const *WatchFaceWeather::Numbers[] = {
  "F:/N0W.bin",
  "F:/N1W.bin",
  "F:/N2W.bin",
  "F:/N3W.bin",
  "F:/N4W.bin",
  "F:/N5W.bin",
  "F:/N6W.bin",
  "F:/N7W.bin",
  "F:/N8W.bin",
  "F:/N9W.bin"
};

char const *WatchFaceWeather::NumbersS[] = {
  "F:/N0S.bin",
  "F:/N1S.bin",
  "F:/N2S.bin",
  "F:/N3S.bin",
  "F:/N4S.bin",
  "F:/N5S.bin",
  "F:/N6S.bin",
  "F:/N7S.bin",
  "F:/N8S.bin",
  "F:/N9S.bin"
};

char const *WatchFaceWeather::WeekDay[] = {
  "F:/MON.bin",
  "F:/TUE.bin",
  "F:/WED.bin",
  "F:/THU.bin",
  "F:/FRI.bin",
  "F:/SAT.bin",
  "F:/SUN.bin"
};

WatchFaceWeather::WatchFaceWeather(Pinetime::Applications::DisplayApp *app,
                  Controllers::DateTime& dateTimeController,
                  Controllers::Battery& batteryController,
                  Controllers::Settings &settingsController) : Screen(app),
                                           dateTimeController{dateTimeController}, batteryController{batteryController},
                                           settingsController{settingsController} {
  settingsController.SetClockFace(8);
  settingsController.SaveSettings();

  sHour = 99;
  sMinute = 99;
  sSecond = 99;

  hour = dateTimeController.Hours();
  minute = dateTimeController.Minutes();
  second = dateTimeController.Seconds();

  day = dateTimeController.Day();
  dayOfWeek = dateTimeController.DayOfWeek();

  currentDayOfWeek = dayOfWeek;
  currentDay = day;

  weatherIcon = (uint8_t)settingsController.GetWeather().current.conditionCode;
  sWeatherIcon = weatherIcon;

  // Set the background
  //lv_obj_set_style_local_bg_color(lv_scr_act(), LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0x000000));

  image[0] = lv_img_create(lv_scr_act(), nullptr);
  lv_obj_set_auto_realign(image[0], true);
  lv_img_set_src(image[0], Numbers[hour/10%10]);
  lv_obj_align(image[0], nullptr, LV_ALIGN_CENTER, -85, -70);

  image[1] = lv_img_create(lv_scr_act(), nullptr);
  lv_obj_set_auto_realign(image[1], true);
  lv_img_set_src(image[1], Numbers[hour%10]);
  lv_obj_align(image[1], image[0], LV_ALIGN_OUT_RIGHT_MID, 5, 0);

  image[2] = lv_img_create(lv_scr_act(), nullptr);
  lv_obj_set_auto_realign(image[2], true);
  lv_img_set_src(image[2], Numbers[minute/10%10]);
  lv_obj_align(image[2], nullptr, LV_ALIGN_CENTER, -85, 30);

  image[3] = lv_img_create(lv_scr_act(), nullptr);
  lv_obj_set_auto_realign(image[3], true);
  lv_img_set_src(image[3], Numbers[minute%10]);
  lv_obj_align(image[3], image[2], LV_ALIGN_OUT_RIGHT_MID, 5, 0);

  image[4] = lv_img_create(lv_scr_act(), nullptr);
  lv_obj_set_auto_realign(image[4], true);
  lv_img_set_src(image[4], NumbersS[second/10%10]);
  lv_obj_align(image[4], nullptr, LV_ALIGN_CENTER, 50, 30);

  image[5] = lv_img_create(lv_scr_act(), nullptr);
  lv_obj_set_auto_realign(image[5], true);
  lv_img_set_src(image[5], NumbersS[second%10]);
  lv_obj_align(image[5], nullptr, LV_ALIGN_CENTER, 80, 30);

  image[6] = lv_img_create(lv_scr_act(), nullptr);
  lv_obj_set_auto_realign(image[6], true);
  lv_img_set_src(image[6], WeatherToday::Icons[weatherIcon]);
  lv_obj_align(image[6], nullptr, LV_ALIGN_CENTER, 65, -70);

  batteryIcon = lv_label_create(lv_scr_act(), nullptr);
  lv_obj_set_style_local_text_font(batteryIcon, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_font_sys_20);
  lv_obj_set_style_local_text_color(batteryIcon, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0x2c89a0));
  lv_label_set_text(batteryIcon, BatteryIcon::GetBatteryIcon(batteryController.PercentRemaining()));
  lv_obj_align(batteryIcon, nullptr, LV_ALIGN_IN_BOTTOM_RIGHT, -15, -10);

  label_day = lv_label_create(lv_scr_act(), nullptr);
  lv_obj_set_style_local_text_color(label_day, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0x225500));
  lv_obj_set_style_local_text_font(label_day, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_font_clock_42);
  lv_label_set_text_fmt(label_day, "%d", day);
  lv_obj_align(label_day, nullptr, LV_ALIGN_IN_BOTTOM_LEFT, 5, -5);
  lv_obj_set_auto_realign(label_day, true);

  image[7] = lv_img_create(lv_scr_act(), nullptr);
  lv_obj_set_auto_realign(image[7], true);
  lv_img_set_src(image[7], WeekDay[((uint8_t)dayOfWeek) - 1]);
  lv_obj_align(image[7], label_day, LV_ALIGN_OUT_RIGHT_MID, 10, 0);

  lv_obj_t* backgroundLabel = lv_label_create(lv_scr_act(), nullptr);
  lv_label_set_long_mode(backgroundLabel, LV_LABEL_LONG_CROP);
  lv_obj_set_size(backgroundLabel, 240, 240);
  lv_obj_set_pos(backgroundLabel, 0, 0);
  lv_label_set_text(backgroundLabel, "");

  taskUpdate = lv_task_create(lv_update_task, 10000, LV_TASK_PRIO_MID, this);

}

WatchFaceWeather::~WatchFaceWeather() {

  lv_task_del(taskUpdate);
  lv_obj_clean(lv_scr_act());
  //lv_obj_set_style_local_bg_color(lv_scr_act(), LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0x000000));

}


void WatchFaceWeather::UpdateScreen() {

  hour = dateTimeController.Hours();
  minute = dateTimeController.Minutes();
  second = dateTimeController.Seconds();

  if (sHour != hour) {
    sHour = hour;
    lv_img_set_src(image[0], Numbers[hour/10%10]);
    lv_img_set_src(image[1], Numbers[hour%10]);
  }

  if (sMinute != minute) {
    sMinute = minute;
    lv_img_set_src(image[2], Numbers[minute/10%10]);
    lv_img_set_src(image[3], Numbers[minute%10]);
  }

  if(sSecond != second) {
    sSecond = second;
    lv_img_set_src(image[4], NumbersS[second/10%10]);
    lv_img_set_src(image[5], NumbersS[second%10]);
  }

  weatherIcon = (uint8_t)settingsController.GetWeather().current.conditionCode;
  if(sWeatherIcon != weatherIcon) {
    sWeatherIcon = weatherIcon;
    lv_img_set_src(image[6], WeatherToday::Icons[weatherIcon]);
  }

  day = dateTimeController.Day();
  dayOfWeek = dateTimeController.DayOfWeek();  

  if (day != currentDay) {
    currentDay = day;
    lv_label_set_text_fmt(label_day, "%d", day);
  }

  if (dayOfWeek != currentDayOfWeek) {
    currentDayOfWeek = dayOfWeek;
    lv_img_set_src(image[7], WeekDay[((uint8_t)dayOfWeek) - 1]);
  }

  lv_label_set_text(batteryIcon, BatteryIcon::GetBatteryIcon(batteryController.PercentRemaining()));

  /*
  uint8_t hour;
  if ( settingsController.GetClockType() == Controllers::Settings::ClockType::H24 ) {
    hour = dateTimeController.Hours();
  } else {
    hour = dateTimeController.Hours12();
  }
  
  }*/

}

bool WatchFaceWeather::Refresh() {
  return true;
}