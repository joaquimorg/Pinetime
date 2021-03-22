#include <libs/lvgl/lvgl.h>
#include "WatchFaceCustom.h"
#include "BatteryIcon.h"
#include "BleIcon.h"
#include "Symbols.h"
#include "NotificationIcon.h"

  
//LV_IMG_DECLARE(bg_img_1);

using namespace Pinetime::Applications::Screens;

WatchFaceCustom::WatchFaceCustom(uint8_t imgnum, Pinetime::Applications::DisplayApp *app,
                  Controllers::DateTime& dateTimeController,
                  Controllers::Battery& batteryController,
                  Controllers::Ble& bleController,
                  Controllers::NotificationManager& notificatioManager,
                  Controllers::Settings &settingsController) : Screen(app), currentDateTime{{}},
                                           dateTimeController{dateTimeController}, batteryController{batteryController},
                                           bleController{bleController}, notificatioManager{notificatioManager},
                                           settingsController{settingsController} {
  settingsController.SetClockFace(4 + imgnum);
  settingsController.SaveSettings();

  uint8_t day = dateTimeController.Day();

  uint8_t hour = dateTimeController.Hours();
  uint8_t minute = dateTimeController.Minutes();
  uint8_t second = dateTimeController.Seconds();

  sHour = hour;
  sMinute = minute;
  sSecond = second;

  lv_obj_t * bg_clock_img = lv_img_create(lv_scr_act(), NULL);
  
  if ( imgnum == 0 ) {
    lv_img_set_src(bg_clock_img, "F:/bg_clock_02.bin");
  } else if ( imgnum == 1 ) {
    lv_img_set_src(bg_clock_img, "F:/bg_clock_03.bin");
  } else {
    lv_img_set_src(bg_clock_img, "F:/bg_clock_04.bin");
  }

  lv_obj_align(bg_clock_img, NULL, LV_ALIGN_CENTER, 0, 0);

  notificationIcon = lv_label_create(lv_scr_act(), NULL);
  lv_label_set_text(notificationIcon, NotificationIcon::GetIcon(true));
  lv_obj_align(notificationIcon, nullptr, LV_ALIGN_IN_BOTTOM_RIGHT, 0, 0);

  // Hour  
  lv_style_init(&hour_style);
  lv_style_set_text_font(&hour_style, LV_STATE_DEFAULT, &lv_font_clock_76);
  lv_style_set_text_color(&hour_style, LV_STATE_DEFAULT, lv_color_hex(0xFFFFFF));

  label_time = lv_label_create(lv_scr_act(), NULL);
  lv_obj_add_style(label_time, LV_LABEL_PART_MAIN, &hour_style);
  lv_label_set_text_fmt(label_time,  "%02i", hour);      
  //lv_label_set_align( label_time, LV_LABEL_ALIGN_CENTER );    
  lv_obj_align(label_time, lv_scr_act(), LV_ALIGN_CENTER, -55, -60);

  // Minute
  label_time_min = lv_label_create(lv_scr_act(), NULL);  
  lv_obj_add_style(label_time_min, LV_LABEL_PART_MAIN, &hour_style);
  lv_label_set_text_fmt(label_time_min,  "%02i", minute);
  //lv_label_set_align( label_time_min, LV_LABEL_ALIGN_CENTER );
  lv_obj_align(label_time_min, lv_scr_act(), LV_ALIGN_CENTER, 55, -60);

  // :  
  lv_style_init(&sep_style);
  lv_style_set_text_font(&sep_style, LV_STATE_DEFAULT, &lv_font_clock_76);
  lv_style_set_text_color(&sep_style, LV_STATE_DEFAULT, lv_color_hex(0xFFFFFF));

  label_time_sep = lv_label_create(lv_scr_act(), NULL);
  lv_obj_add_style(label_time_sep, LV_LABEL_PART_MAIN, &sep_style);
  lv_label_set_text_static(label_time_sep,  ":");      
  //lv_label_set_align( label_time, LV_LABEL_ALIGN_CENTER );    
  lv_obj_align(label_time_sep, lv_scr_act(), LV_ALIGN_CENTER, 0, -60);
  
  
  lv_style_init(&label_shadow_style);
  //lv_style_set_text_opa(&label_shadow_style, LV_STATE_DEFAULT, LV_OPA_50);
  lv_style_set_text_color(&label_shadow_style, LV_STATE_DEFAULT, lv_color_hex(0x444444));

  /*Create a label for the shadow first (it's in the background) */
  label_date_shadow = lv_label_create(lv_scr_act(), NULL);
  lv_obj_add_style(label_date_shadow, LV_LABEL_PART_MAIN, &label_shadow_style);

  // Date  
  lv_style_init(&dateyear_style);
  label_date = lv_label_create(lv_scr_act(), nullptr);
  lv_style_set_text_color(&dateyear_style, LV_STATE_DEFAULT, lv_color_hex(0xFFFFFF));
  lv_obj_add_style(label_date, LV_LABEL_PART_MAIN, &dateyear_style);
  lv_label_set_text_fmt(label_date, "%s %02i %s", dateTimeController.DayOfWeekShortToStringLow(), day, dateTimeController.MonthToStringLow());
  lv_label_set_align( label_date, LV_LABEL_ALIGN_CENTER );
  lv_obj_align(label_date, lv_scr_act(), LV_ALIGN_CENTER, 0, 100);

  lv_label_set_text(label_date_shadow, lv_label_get_text(label_date));
  lv_label_set_align( label_date_shadow, LV_LABEL_ALIGN_CENTER );
  lv_obj_align(label_date_shadow, label_date, LV_ALIGN_IN_TOP_LEFT, 1, 1);

  lv_obj_t* backgroundLabel = lv_label_create(lv_scr_act(), nullptr);
  lv_label_set_long_mode(backgroundLabel, LV_LABEL_LONG_CROP);
  lv_obj_set_size(backgroundLabel, 240, 240);
  lv_obj_set_pos(backgroundLabel, 0, 0);
  lv_label_set_text(backgroundLabel, "");

}

WatchFaceCustom::~WatchFaceCustom() {

  lv_style_reset(&sep_style);
  lv_style_reset(&hour_style);
  lv_style_reset(&label_shadow_style);
  lv_style_reset(&dateyear_style);
  
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
        //lv_style_set_text_color(&sep_style, LV_STATE_DEFAULT, lv_color_hex(0xFFFFFF));
        lv_label_set_text_static(label_time_sep,  ":");
      } else {
        //sep_style.text.color = lv_color_hex(0x000000);
        //lv_style_set_text_color(&sep_style, LV_STATE_DEFAULT, lv_color_hex(0x000000));
        lv_label_set_text_static(label_time_sep,  " ");
      }
    }
  
    if ((month != currentMonth) || (dayOfWeek != currentDayOfWeek) || (day != currentDay)) {

      lv_label_set_text_fmt(label_date, "%s %02i %s", dateTimeController.DayOfWeekShortToStringLow(), day, dateTimeController.MonthToStringLow());
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