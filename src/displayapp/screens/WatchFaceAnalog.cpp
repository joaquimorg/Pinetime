#include <libs/lvgl/lvgl.h>
#include "WatchFaceAnalog.h"
#include "BatteryIcon.h"
#include "BleIcon.h"
#include "Symbols.h"
#include "NotificationIcon.h"

using namespace Pinetime::Applications::Screens;

#define HOUR_LENGTH   70
#define MINUTE_LENGTH 90
#define SECOND_LENGTH 110
#define PI 3.14159265358979323846

LV_IMG_DECLARE(bg_clock);

extern lv_style_t* DefaultStyle;

// ##
static int16_t coordinate_x_relocate(int16_t x)
{
    return ((x) + LV_HOR_RES / 2);
}

// ##
static int16_t coordinate_y_relocate(int16_t y)
{
    return (((y) - LV_HOR_RES / 2) < 0) ? (0 - ((y) - LV_HOR_RES / 2)) : ((y) - LV_HOR_RES / 2);
}

WatchFaceAnalog::WatchFaceAnalog(Pinetime::Applications::DisplayApp *app,
                  Controllers::DateTime& dateTimeController,
                  Controllers::Battery& batteryController,
                  Controllers::Ble& bleController,
                  Controllers::NotificationManager& notificatioManager) : Screen(app), currentDateTime{{}},
                                           dateTimeController{dateTimeController}, batteryController{batteryController},
                                           bleController{bleController}, notificatioManager{notificatioManager} {

  sHour = 99;
  sMinute = 99;
  sSecond = 99;

  
  lv_obj_t * bg_clock_img = lv_img_create(lv_scr_act(), NULL);
  lv_img_set_src(bg_clock_img, &bg_clock);
  lv_obj_align(bg_clock_img, NULL, LV_ALIGN_CENTER, 0, 0);

  batteryIcon = lv_label_create(lv_scr_act(), nullptr);
  lv_label_set_text(batteryIcon, Symbols::batteryHalf);
  lv_obj_align(batteryIcon, lv_scr_act(), LV_ALIGN_CENTER, 45, 55);

  static lv_style_t not_style;
  lv_style_copy(&not_style, DefaultStyle);

  notificationIcon = lv_label_create(lv_scr_act(), NULL);
  not_style.text.color = lv_color_hex(0x00FF00);  
  lv_label_set_style(notificationIcon, LV_LABEL_STYLE_MAIN, &not_style);
  lv_label_set_text(notificationIcon, NotificationIcon::GetIcon(false));
  lv_obj_align(notificationIcon, lv_scr_act(), LV_ALIGN_CENTER, -45, 55);

  // Date - Day / Week day
  static lv_style_t date_style;
  lv_style_copy(&date_style, DefaultStyle);

  label_date_day = lv_label_create(lv_scr_act(), NULL);
  date_style.text.color = lv_color_hex(0x00F0FF);  
  lv_label_set_style(label_date_day, LV_LABEL_STYLE_MAIN, &date_style);
  lv_label_set_text_fmt(label_date_day,  "%s\n%02i", "SAT", 1);
  lv_label_set_align( label_date_day, LV_LABEL_ALIGN_CENTER );    
  lv_obj_align(label_date_day, lv_scr_act(), LV_ALIGN_CENTER, 50, 0);  
  
  minute_body = lv_line_create(lv_scr_act(), NULL);
  hour_body   = lv_line_create(lv_scr_act(), NULL);
  second_body = lv_line_create(lv_scr_act(), NULL);

  lv_style_copy(&minute_line_style, &lv_style_pretty);
  minute_line_style.line.color = lv_color_hex(0x606060);
  minute_line_style.line.width = 5;
  lv_line_set_style(minute_body, LV_LINE_STYLE_MAIN, &minute_line_style);

  lv_style_copy(&hour_line_style, &lv_style_pretty);
  hour_line_style.line.color = LV_COLOR_GREEN;
  hour_line_style.line.width = 7;
  lv_line_set_style(hour_body, LV_LINE_STYLE_MAIN, &hour_line_style);

  lv_style_copy(&second_line_style, &lv_style_pretty);
  second_line_style.line.color = LV_COLOR_RED;
  second_line_style.line.width = 3;
  lv_line_set_style(second_body, LV_LINE_STYLE_MAIN, &second_line_style);


  /*backgroundLabel = lv_label_create(lv_scr_act(), nullptr);
  lv_label_set_long_mode(backgroundLabel, LV_LABEL_LONG_CROP);
  lv_obj_set_size(backgroundLabel, 240, 240);
  lv_obj_set_pos(backgroundLabel, 0, 0);
  lv_label_set_text(backgroundLabel, "");*/

}

WatchFaceAnalog::~WatchFaceAnalog() {
  lv_obj_clean(lv_scr_act());
}


bool WatchFaceAnalog::Refresh() {

  batteryPercentRemaining = batteryController.PercentRemaining();
  if (batteryPercentRemaining.IsUpdated()) {
    auto batteryPercent = batteryPercentRemaining.Get();
    lv_label_set_text(batteryIcon, BatteryIcon::GetBatteryIcon(batteryPercent));
  }


  notificationState = notificatioManager.AreNewNotificationsAvailable();

  if(notificationState.IsUpdated()) {
    if(notificationState.Get() == true)
      lv_label_set_text(notificationIcon, NotificationIcon::GetIcon(true));
    else
      lv_label_set_text(notificationIcon, NotificationIcon::GetIcon(false));
  }

  currentDateTime = dateTimeController.CurrentDateTime();

  if(currentDateTime.IsUpdated()) {

    auto month = dateTimeController.Month();
    uint8_t day = dateTimeController.Day();
    auto dayOfWeek = dateTimeController.DayOfWeek();

    uint8_t hour = dateTimeController.Hours();
    uint8_t minute = dateTimeController.Minutes();
    uint8_t second = dateTimeController.Seconds();

    if(sMinute != minute) {
      sMinute = minute;
      minute_point[0].x = coordinate_x_relocate(10 * sin((180 + minute * 6) * PI / 180));
      minute_point[0].y = coordinate_y_relocate(10 * cos((180 + minute * 6) * PI / 180));
      minute_point[1].x = coordinate_x_relocate(MINUTE_LENGTH * sin(minute * 6 * PI / 180));
      minute_point[1].y = coordinate_y_relocate(MINUTE_LENGTH * cos(minute * 6 * PI / 180));
      lv_line_set_points(minute_body, minute_point, 2);
    }

    if(sHour != hour) {
      sHour = hour;
      hour_point[0].x   = coordinate_x_relocate(10 * sin((180 + (hour > 12 ? hour - 12 : hour) * 30) * PI / 180));
      hour_point[0].y   = coordinate_y_relocate(10 * cos((180 + (hour > 12 ? hour - 12 : hour) * 30) * PI / 180));
      hour_point[1].x   = coordinate_x_relocate(HOUR_LENGTH * sin((hour > 12 ? hour - 12 : hour) * 30 * PI / 180));
      hour_point[1].y   = coordinate_y_relocate(HOUR_LENGTH * cos((hour > 12 ? hour - 12 : hour) * 30 * PI / 180));
      lv_line_set_points(hour_body,   hour_point,   2);
    }

    if(sSecond != second) {      
      sSecond = second;
      second_point[0].x = coordinate_x_relocate(20 * sin((180 + second * 6) * PI / 180));
      second_point[0].y = coordinate_y_relocate(20 * cos((180 + second * 6) * PI / 180));
      second_point[1].x = coordinate_x_relocate(SECOND_LENGTH * sin(second * 6 * PI / 180));
      second_point[1].y = coordinate_y_relocate(SECOND_LENGTH * cos(second * 6 * PI / 180));
      lv_line_set_points(second_body, second_point, 2);

    }
  
    if ((month != currentMonth) || (dayOfWeek != currentDayOfWeek) || (day != currentDay)) {

      lv_label_set_text_fmt(label_date_day,  "%s\n%02i", dateTimeController.DayOfWeekShortToString(), day);

      currentMonth = month;
      currentDayOfWeek = dayOfWeek;
      currentDay = day;
    }
  }

  return true;
}