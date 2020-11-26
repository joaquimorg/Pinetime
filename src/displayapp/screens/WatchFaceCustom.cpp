#include <libs/lvgl/lvgl.h>
#include "WatchFaceCustom.h"
#include "BatteryIcon.h"
#include "BleIcon.h"
#include "Symbols.h"
#include "NotificationIcon.h"

using namespace Pinetime::Applications::Screens;

LV_IMG_DECLARE(bg_img_1);

extern lv_style_t* LabelStyle76;
extern lv_style_t* DefaultStyle;

WatchFaceCustom::WatchFaceCustom(Pinetime::Applications::DisplayApp *app,
                  Controllers::DateTime& dateTimeController,
                  Controllers::Battery& batteryController,
                  Controllers::Ble& bleController,
                  Controllers::NotificationManager& notificatioManager,
                  Controllers::Settings &settingsController) : Screen(app), currentDateTime{{}},
                                           dateTimeController{dateTimeController}, batteryController{batteryController},
                                           bleController{bleController}, notificatioManager{notificatioManager},
                                           settingsController{settingsController} {
  settingsController.SetClockFace(3);
  sHour = 99;
  sMinute = 99;
  sSecond = 99;

  lv_obj_t * bg_clock_img = lv_img_create(lv_scr_act(), NULL);
  lv_img_set_src(bg_clock_img, &bg_img_1);
  lv_obj_align(bg_clock_img, NULL, LV_ALIGN_CENTER, 0, 0);

  static lv_style_t not_style;
  lv_style_copy(&not_style, DefaultStyle);

  notificationIcon = lv_label_create(lv_scr_act(), NULL);
  not_style.text.color = lv_color_hex(0x00FF00);  
  lv_label_set_style(notificationIcon, LV_LABEL_STYLE_MAIN, &not_style);
  lv_label_set_text(notificationIcon, NotificationIcon::GetIcon(true));
  lv_obj_align(notificationIcon, nullptr, LV_ALIGN_IN_BOTTOM_RIGHT, 0, 0);

  // Hour
  static lv_style_t hour_style;
  lv_style_copy(&hour_style, LabelStyle76);
  hour_style.text.color = lv_color_hex(0xFFFFFF);  

  label_time = lv_label_create(lv_scr_act(), NULL);
  lv_label_set_style(label_time, LV_LABEL_STYLE_MAIN, &hour_style);
  lv_label_set_text_fmt(label_time,  "%02i", 0);      
  //lv_label_set_align( label_time, LV_LABEL_ALIGN_CENTER );    
  lv_obj_align(label_time, lv_scr_act(), LV_ALIGN_CENTER, -50, 45);

  // Minute
  label_time_min = lv_label_create(lv_scr_act(), NULL);  
  lv_label_set_style(label_time_min, LV_LABEL_STYLE_MAIN, LabelStyle76);
  lv_label_set_text_fmt(label_time_min,  "%02i", 0);
  //lv_label_set_align( label_time_min, LV_LABEL_ALIGN_CENTER );
  lv_obj_align(label_time_min, lv_scr_act(), LV_ALIGN_CENTER, 50, 45);

  // :  
  lv_style_copy(&sep_style, LabelStyle76);
  sep_style.text.color = lv_color_hex(0xFFFFFF);  

  label_time_sep = lv_label_create(lv_scr_act(), NULL);
  lv_label_set_style(label_time_sep, LV_LABEL_STYLE_MAIN, &sep_style);
  lv_label_set_text(label_time_sep,  ":");      
  //lv_label_set_align( label_time, LV_LABEL_ALIGN_CENTER );    
  lv_obj_align(label_time_sep, lv_scr_act(), LV_ALIGN_CENTER, 0, 45);
  
  // Date
  static lv_style_t dateyear_style;
  lv_style_copy(&dateyear_style, DefaultStyle);  
  label_date = lv_label_create(lv_scr_act(), nullptr);
  dateyear_style.text.color = lv_color_hex(0xFFA800);  
  lv_label_set_style(label_date, LV_LABEL_STYLE_MAIN, &dateyear_style);
  lv_label_set_text_fmt(label_date, "%s %02i %s", "WED", 18, "NOV");
  lv_label_set_align( label_date, LV_LABEL_ALIGN_CENTER );
  lv_obj_align(label_date, lv_scr_act(), LV_ALIGN_CENTER, 0, 90);

  backgroundLabel = lv_label_create(lv_scr_act(), nullptr);
  lv_label_set_long_mode(backgroundLabel, LV_LABEL_LONG_CROP);
  lv_obj_set_size(backgroundLabel, 240, 240);
  lv_obj_set_pos(backgroundLabel, 0, 0);
  lv_label_set_text(backgroundLabel, "");

}

WatchFaceCustom::~WatchFaceCustom() {
  lv_obj_clean(lv_scr_act());
}


bool WatchFaceCustom::Refresh() {

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

    if(sHour != hour || sMinute != minute) {
      sHour = hour;
      sMinute = minute;
      lv_label_set_text_fmt(label_time,  "%02i", sHour);
      lv_label_set_text_fmt(label_time_min,  "%02i", sMinute);

    }

    if(sSecond != second) {
      
      sSecond = second;

      if ( second % 2 == 0 ) {
        sep_style.text.color = lv_color_hex(0xFFFFFF);
      } else {
        sep_style.text.color = lv_color_hex(0x000000);
      }
      lv_label_set_text(label_time_sep,  ":");   
    }
  
    if ((month != currentMonth) || (dayOfWeek != currentDayOfWeek) || (day != currentDay)) {

      lv_label_set_text_fmt(label_date, "%s %02i %s", dateTimeController.DayOfWeekShortToString(), day, dateTimeController.MonthToString());
      lv_label_set_align( label_date, LV_LABEL_ALIGN_CENTER );
      currentMonth = month;
      currentDayOfWeek = dayOfWeek;
      currentDay = day;
    }
  }

  return true;
}