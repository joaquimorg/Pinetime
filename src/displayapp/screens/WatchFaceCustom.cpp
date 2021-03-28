#include "WatchFaceCustom.h"
#include "BatteryIcon.h"
#include "BleIcon.h"
#include "Symbols.h"
#include "NotificationIcon.h"
#include "resources_config.h"

using namespace Pinetime::Applications::Screens;

static void lv_update_task(struct _lv_task_t *task) {  
  auto user_data = static_cast<WatchFaceCustom *>(task->user_data);
  user_data->UpdateScreen();
}

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

  clock_76 = lv_font_load(FNT_CLOCK_76);

  sHour = 99;
  sMinute = 99;
  sSecond = 99;

  lv_obj_t * bg_clock_img = lv_img_create(lv_scr_act(), NULL);
  
  if ( imgnum == 0 ) {
    lv_img_set_src(bg_clock_img, WF_BACKGROUND_C1);
  } else if ( imgnum == 1 ) {
    lv_img_set_src(bg_clock_img, WF_BACKGROUND_C2);
  } else {
    lv_img_set_src(bg_clock_img, WF_BACKGROUND_C3);
  }

  lv_obj_align(bg_clock_img, NULL, LV_ALIGN_CENTER, 0, 0);

  notificationIcon = lv_label_create(lv_scr_act(), NULL);
  lv_label_set_text(notificationIcon, NotificationIcon::GetIcon(true));
  lv_obj_align(notificationIcon, nullptr, LV_ALIGN_IN_BOTTOM_RIGHT, 0, 0);

  // Hour  
  lv_style_init(&hour_style);
  lv_style_set_text_font(&hour_style, LV_STATE_DEFAULT, clock_76);
  lv_style_set_text_color(&hour_style, LV_STATE_DEFAULT, lv_color_hex(0xFFFFFF));

  label_time = lv_label_create(lv_scr_act(), NULL);
  lv_obj_add_style(label_time, LV_LABEL_PART_MAIN, &hour_style);

  if ( settingsController.GetClockType() == Controllers::Settings::ClockType::H12 ) {
    label_time_am_pm = lv_label_create(lv_scr_act(), NULL);
    lv_label_set_align( label_time_am_pm, LV_LABEL_ALIGN_CENTER );
  }

  // Minute
  label_time_min = lv_label_create(lv_scr_act(), NULL);  
  lv_obj_add_style(label_time_min, LV_LABEL_PART_MAIN, &hour_style);
  
  // :  
  label_time_sep = lv_label_create(lv_scr_act(), NULL);
  lv_obj_add_style(label_time_sep, LV_LABEL_PART_MAIN, &hour_style);
  lv_obj_align(label_time_sep, lv_scr_act(), LV_ALIGN_CENTER, 0, -60);
  

  label_date = lv_label_create(lv_scr_act(), nullptr);
  lv_obj_set_style_local_text_color(label_date, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0xFFFFFF));
  lv_obj_align(label_date, lv_scr_act(), LV_ALIGN_IN_BOTTOM_LEFT, 4, -4);

  lv_obj_t* backgroundLabel = lv_label_create(lv_scr_act(), nullptr);
  lv_label_set_long_mode(backgroundLabel, LV_LABEL_LONG_CROP);
  lv_obj_set_size(backgroundLabel, 240, 240);
  lv_obj_set_pos(backgroundLabel, 0, 0);
  lv_label_set_text(backgroundLabel, "");

  UpdateScreen();
  taskUpdate = lv_task_create(lv_update_task, 10000, LV_TASK_PRIO_MID, this);

}

WatchFaceCustom::~WatchFaceCustom() {
  
  lv_task_del(taskUpdate);
  lv_obj_clean(lv_scr_act());
  lv_style_reset(&hour_style);
  lv_font_free(clock_76);
  
}


void WatchFaceCustom::UpdateScreen() {

  if(notificatioManager.AreNewNotificationsAvailable())
    lv_label_set_text(notificationIcon, NotificationIcon::GetIcon(true));
  else
    lv_label_set_text(notificationIcon, NotificationIcon::GetIcon(false));


  auto month = dateTimeController.Month();
  uint8_t day = dateTimeController.Day();
  auto dayOfWeek = dateTimeController.DayOfWeek();
  uint8_t hour;
  if ( settingsController.GetClockType() == Controllers::Settings::ClockType::H24 ) {
    hour = dateTimeController.Hours();
  } else {
    hour = dateTimeController.Hours12();
  }
  uint8_t minute = dateTimeController.Minutes();
  uint8_t second = dateTimeController.Seconds();

  if(sHour != hour || sMinute != minute) {
    sHour = hour;
    sMinute = minute;
    
    lv_label_set_text_fmt(label_time_min,  "%02i", sMinute);
    lv_obj_align(label_time_min, lv_scr_act(), LV_ALIGN_CENTER, 55, -60);

    if ( settingsController.GetClockType() == Controllers::Settings::ClockType::H12 ) {
      lv_label_set_text_fmt(label_time, "%2i", hour);
      if ( hour > 9 ) {
        lv_obj_align(label_time, lv_scr_act(), LV_ALIGN_CENTER, -50, -60);
      } else {
        lv_obj_align(label_time, lv_scr_act(), LV_ALIGN_CENTER, -35, -60);
      }
      lv_label_set_text(label_time_am_pm, dateTimeController.HourAMPM(true));
      lv_obj_align(label_time_am_pm, label_time, LV_ALIGN_OUT_LEFT_MID, 0, 0);
      lv_label_set_align( label_time_am_pm, LV_LABEL_ALIGN_CENTER );
    } else {
      lv_obj_align(label_time, lv_scr_act(), LV_ALIGN_CENTER, -55, -60);
      lv_label_set_text_fmt(label_time, "%02i", hour);
    }

  }

  if(sSecond != second) {
    
    sSecond = second;

    if ( second % 2 == 0 ) {
      lv_label_set_text_static(label_time_sep,  ":");
    } else {
      lv_label_set_text_static(label_time_sep,  " ");
    }
  }

  if ((month != currentMonth) || (dayOfWeek != currentDayOfWeek) || (day != currentDay)) {

    if ( settingsController.GetClockType() == Controllers::Settings::ClockType::H12 ) {
      lv_label_set_text_fmt(label_date, "%s, %s %02i", dateTimeController.DayOfWeekShortToStringLow(), dateTimeController.MonthToStringLow(), dateTimeController.Day());
    } else {
      lv_label_set_text_fmt(label_date, "%s, %02i %s", dateTimeController.DayOfWeekShortToStringLow(), dateTimeController.Day(), dateTimeController.MonthToStringLow());
    }

    currentMonth = month;
    currentDayOfWeek = dayOfWeek;
    currentDay = day;
  }
  
}

bool WatchFaceCustom::Refresh() {
  return true;
}