#include <libs/lvgl/lvgl.h>
#include "WatchFaceCustom.h"
#include "BatteryIcon.h"
#include "BleIcon.h"
#include "Symbols.h"
#include "NotificationIcon.h"

using namespace Pinetime::Applications::Screens;

LV_IMG_DECLARE(xmas);
//LV_FONT_DECLARE(lv_font_clock_76)

//extern lv_style_t* LabelStyle76;
//extern lv_style_t* DefaultStyle;

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
  uint16_t year = dateTimeController.Year();
  uint8_t day = dateTimeController.Day();

  uint8_t hour = dateTimeController.Hours();
  uint8_t minute = dateTimeController.Minutes();
  uint8_t second = dateTimeController.Seconds();

  sHour = hour;
  sMinute = minute;
  sSecond = second;

  lv_obj_t * bg_clock_img = lv_img_create(lv_scr_act(), NULL);
  lv_img_set_src(bg_clock_img, &xmas);
  lv_obj_align(bg_clock_img, NULL, LV_ALIGN_CENTER, 0, 0);

  notificationIcon = lv_label_create(lv_scr_act(), NULL);
  lv_label_set_text(notificationIcon, NotificationIcon::GetIcon(true));
  lv_obj_align(notificationIcon, nullptr, LV_ALIGN_IN_BOTTOM_RIGHT, 0, 0);

  // Hour
  static lv_style_t hour_style;
  lv_style_init(&hour_style);
  lv_style_set_text_font(&hour_style, LV_STATE_DEFAULT, &lv_font_clock_76);
  lv_style_set_text_color(&hour_style, LV_STATE_DEFAULT, lv_color_hex(0xFFFFFF));

  label_time = lv_label_create(lv_scr_act(), NULL);
  lv_obj_add_style(label_time, LV_LABEL_PART_MAIN, &hour_style);
  lv_label_set_text_fmt(label_time,  "%02i", hour);      
  //lv_label_set_align( label_time, LV_LABEL_ALIGN_CENTER );    
  lv_obj_align(label_time, lv_scr_act(), LV_ALIGN_CENTER, -50, 45);

  // Minute
  label_time_min = lv_label_create(lv_scr_act(), NULL);  
  lv_obj_add_style(label_time_min, LV_LABEL_PART_MAIN, &hour_style);
  lv_label_set_text_fmt(label_time_min,  "%02i", minute);
  //lv_label_set_align( label_time_min, LV_LABEL_ALIGN_CENTER );
  lv_obj_align(label_time_min, lv_scr_act(), LV_ALIGN_CENTER, 50, 45);

  // :  
  lv_style_init(&sep_style);
  lv_style_set_text_font(&sep_style, LV_STATE_DEFAULT, &lv_font_clock_76);
  lv_style_set_text_color(&sep_style, LV_STATE_DEFAULT, lv_color_hex(0xFFFFFF));

  label_time_sep = lv_label_create(lv_scr_act(), NULL);
  lv_obj_add_style(label_time_sep, LV_LABEL_PART_MAIN, &sep_style);
  lv_label_set_text(label_time_sep,  ":");      
  //lv_label_set_align( label_time, LV_LABEL_ALIGN_CENTER );    
  lv_obj_align(label_time_sep, lv_scr_act(), LV_ALIGN_CENTER, 0, 45);
  

  static lv_style_t label_shadow_style;
  lv_style_init(&label_shadow_style);
  //lv_style_set_text_opa(&label_shadow_style, LV_STATE_DEFAULT, LV_OPA_50);
  lv_style_set_text_color(&label_shadow_style, LV_STATE_DEFAULT, lv_color_hex(0x00FF00));

  /*Create a label for the shadow first (it's in the background) */
  label_date_shadow = lv_label_create(lv_scr_act(), NULL);
  lv_obj_add_style(label_date_shadow, LV_LABEL_PART_MAIN, &label_shadow_style);

  // Date
  static lv_style_t dateyear_style;
  lv_style_init(&dateyear_style);
  label_date = lv_label_create(lv_scr_act(), nullptr);
  lv_style_set_text_color(&dateyear_style, LV_STATE_DEFAULT, lv_color_hex(0xFFFFFF));
  lv_obj_add_style(label_date, LV_LABEL_PART_MAIN, &dateyear_style);
  lv_label_set_text_fmt(label_date, "%s %02i %s", dateTimeController.DayOfWeekShortToString(), day, dateTimeController.MonthToString());
  lv_label_set_align( label_date, LV_LABEL_ALIGN_CENTER );
  lv_obj_align(label_date, lv_scr_act(), LV_ALIGN_CENTER, 0, 90);

  lv_label_set_text(label_date_shadow, lv_label_get_text(label_date));
  lv_label_set_align( label_date_shadow, LV_LABEL_ALIGN_CENTER );
  lv_obj_align(label_date_shadow, label_date, LV_ALIGN_IN_TOP_LEFT, 1, 1);

  /*backgroundLabel = lv_label_create(lv_scr_act(), nullptr);
  lv_label_set_long_mode(backgroundLabel, LV_LABEL_LONG_CROP);
  lv_obj_set_size(backgroundLabel, 240, 240);
  lv_obj_set_pos(backgroundLabel, 0, 0);
  lv_label_set_text(backgroundLabel, "");*/

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
        //sep_style.text.color = lv_color_hex(0xFFFFFF);
        lv_style_set_text_color(&sep_style, LV_STATE_DEFAULT, lv_color_hex(0xFFFFFF));
      } else {
        //sep_style.text.color = lv_color_hex(0x000000);
        lv_style_set_text_color(&sep_style, LV_STATE_DEFAULT, lv_color_hex(0xFF0000));
      }
      lv_label_set_text(label_time_sep,  ":");   
    }
  
    if ((month != currentMonth) || (dayOfWeek != currentDayOfWeek) || (day != currentDay)) {

      lv_label_set_text_fmt(label_date, "%s %02i %s", dateTimeController.DayOfWeekShortToString(), day, dateTimeController.MonthToString());
      lv_label_set_text(label_date_shadow, lv_label_get_text(label_date));
      lv_label_set_align( label_date, LV_LABEL_ALIGN_CENTER );
      lv_label_set_align( label_date_shadow, LV_LABEL_ALIGN_CENTER );
      currentMonth = month;
      currentDayOfWeek = dayOfWeek;
      currentDay = day;
    }
  }

  return true;
}