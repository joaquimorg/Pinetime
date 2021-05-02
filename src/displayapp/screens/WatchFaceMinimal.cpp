#include <libs/lvgl/lvgl.h>
#include "WatchFaceMinimal.h"
#include "BatteryIcon.h"
#include "BleIcon.h"
#include "Symbols.h"
#include "NotificationIcon.h"
#include "resources_config.h"

using namespace Pinetime::Applications::Screens;

static void lv_update_task(struct _lv_task_t *task) {  
  auto user_data = static_cast<WatchFaceMinimal *>(task->user_data);
  user_data->UpdateScreen();
}

WatchFaceMinimal::WatchFaceMinimal(Pinetime::Applications::DisplayApp *app,
                  Controllers::DateTime& dateTimeController,
                  Controllers::Battery& batteryController,
                  Controllers::Ble& bleController,
                  Controllers::NotificationManager& notificatioManager,
                  Controllers::Settings &settingsController) : Screen(app),
                                           dateTimeController{dateTimeController}, batteryController{batteryController},
                                           bleController{bleController}, notificatioManager{notificatioManager},
                                           settingsController{settingsController} {
                                             
  settingsController.SetClockFace(3);
  settingsController.SaveSettings();

  sHour = 99;
  sMinute = 99;
  sSecond = 99;

  // Set the background to Black
  //lv_obj_set_style_local_bg_color(lv_scr_act(), LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, lv_color_make(0, 0, 0));
  /*
  // hours
  arcHours = lv_arc_create(lv_scr_act(), NULL);
  lv_arc_set_bg_angles(arcHours, 270, 269);
  lv_arc_set_range(arcHours, 0, 23);
  lv_obj_set_size(arcHours, 210, 210);
  lv_obj_align(arcHours, NULL, LV_ALIGN_CENTER, 0, 0);
  lv_obj_set_style_local_line_width(arcHours, LV_ARC_PART_INDIC, LV_STATE_DEFAULT, LV_DPX(10));
  lv_obj_set_style_local_line_width(arcHours, LV_ARC_PART_BG , LV_STATE_DEFAULT, 0);
  lv_obj_set_style_local_bg_opa(arcHours, LV_ARC_PART_BG, LV_STATE_DEFAULT, LV_OPA_0);
  lv_obj_set_style_local_border_width(arcHours, LV_ARC_PART_BG, LV_STATE_DEFAULT, 0);
  lv_obj_set_style_local_line_color(arcHours, LV_ARC_PART_INDIC, LV_STATE_DEFAULT, LV_COLOR_GREEN);   

  lv_arc_set_value(arcHours, 0);

  // minutes
  arcMinutes = lv_arc_create(lv_scr_act(), NULL);
  lv_arc_set_bg_angles(arcMinutes, 270, 269);
  lv_arc_set_range(arcMinutes, 0, 59);
  lv_obj_set_size(arcMinutes, 232, 232);
  lv_obj_align(arcMinutes, NULL, LV_ALIGN_CENTER, 0, 0);
  lv_obj_set_style_local_line_width(arcMinutes, LV_ARC_PART_INDIC, LV_STATE_DEFAULT, LV_DPX(10));
  lv_obj_set_style_local_line_width(arcMinutes, LV_ARC_PART_BG , LV_STATE_DEFAULT, 0);
  lv_obj_set_style_local_bg_opa(arcMinutes, LV_ARC_PART_BG, LV_STATE_DEFAULT, LV_OPA_0);
  lv_obj_set_style_local_border_width(arcMinutes, LV_ARC_PART_BG, LV_STATE_DEFAULT, 0);
  lv_obj_set_style_local_line_color(arcMinutes, LV_ARC_PART_INDIC, LV_STATE_DEFAULT, LV_COLOR_BLUE);   

  lv_arc_set_value(arcMinutes, 0);
  */
  // Seconds
  seconds = lv_arc_create(lv_scr_act(), NULL);
  lv_arc_set_bg_angles(seconds, 270, 269);
  lv_arc_set_range(seconds, 0, 59);
  lv_obj_set_size(seconds, 260, 260);
  lv_obj_align(seconds, NULL, LV_ALIGN_CENTER, 0, 0);
  lv_obj_set_style_local_line_width(seconds, LV_ARC_PART_INDIC, LV_STATE_DEFAULT, LV_DPX(10));
  lv_obj_set_style_local_line_width(seconds, LV_ARC_PART_BG , LV_STATE_DEFAULT, 0);
  lv_obj_set_style_local_bg_opa(seconds, LV_ARC_PART_BG, LV_STATE_DEFAULT, LV_OPA_0);
  lv_obj_set_style_local_border_width(seconds, LV_ARC_PART_BG, LV_STATE_DEFAULT, 0);
  lv_obj_set_style_local_line_color(seconds, LV_ARC_PART_INDIC, LV_STATE_DEFAULT, LV_COLOR_RED);   

  lv_arc_set_value(seconds, 0);

  // Hour  
  label_time = lv_label_create(lv_scr_act(), NULL);
  lv_obj_set_style_local_text_color(label_time, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0xFFC300));
  lv_obj_set_style_local_text_font(label_time, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_font_clock_76);

  if ( settingsController.GetClockType() == Controllers::Settings::ClockType::H12 ) {
    label_time_am_pm = lv_label_create(lv_scr_act(), NULL);
    lv_obj_set_style_local_text_color(label_time_am_pm, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0x202020));
  }

  // Minute  
  label_time_min = lv_label_create(lv_scr_act(), NULL);  
  lv_obj_set_style_local_text_color(label_time_min, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0x00A3FF));
  lv_obj_set_style_local_text_font(label_time_min, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_font_clock_76);
  lv_obj_align(label_time_min, nullptr, LV_ALIGN_CENTER, 0, 55);


  notificationIcon = lv_label_create(lv_scr_act(), NULL);
  lv_obj_set_style_local_text_color(notificationIcon, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0x666666));
  lv_obj_set_style_local_text_font(notificationIcon, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_font_sys_20);  
  lv_obj_align(notificationIcon, nullptr, LV_ALIGN_IN_BOTTOM_RIGHT, -5, -5);

  batteryIcon = lv_label_create(lv_scr_act(), nullptr);
  lv_obj_set_style_local_text_color(batteryIcon, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0x666666));
  lv_obj_set_style_local_text_font(batteryIcon, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_font_sys_20);
  lv_obj_align(batteryIcon, nullptr, LV_ALIGN_IN_BOTTOM_LEFT, 5, -5);
  
  // Date   
  label_date = lv_label_create(lv_scr_act(), nullptr);
  lv_obj_set_style_local_text_color(label_date, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0x1FFF00));
  lv_obj_align(label_date, nullptr, LV_ALIGN_CENTER, 0, 0);
  lv_obj_set_auto_realign(label_date, true);

  lv_obj_t* backgroundLabel = lv_label_create(lv_scr_act(), nullptr);
  lv_label_set_long_mode(backgroundLabel, LV_LABEL_LONG_CROP);
  lv_obj_set_size(backgroundLabel, 240, 240);
  lv_obj_set_pos(backgroundLabel, 0, 0);
  lv_label_set_text_static(backgroundLabel, "");

  UpdateScreen();
  taskUpdate = lv_task_create(lv_update_task, 10000, LV_TASK_PRIO_MID, this);
}

WatchFaceMinimal::~WatchFaceMinimal() {
  
  lv_task_del(taskUpdate);
  lv_obj_clean(lv_scr_act());
}

void WatchFaceMinimal::UpdateScreen() {
  month = dateTimeController.Month();
  day = dateTimeController.Day();
  dayOfWeek = dateTimeController.DayOfWeek();

  uint8_t hour;
  if ( settingsController.GetClockType() == Controllers::Settings::ClockType::H24 ) {
    hour = dateTimeController.Hours();
  } else {
    hour = dateTimeController.Hours12();
  }
  minute = dateTimeController.Minutes();
  second = dateTimeController.Seconds();

  if(sHour != hour || sMinute != minute) {
    sHour = hour;
    sMinute = minute;

    if ( settingsController.GetClockType() == Controllers::Settings::ClockType::H12 ) {
      lv_label_set_text_fmt(label_time, "%2i", sHour);
      if ( hour > 9 ) {
        lv_obj_align(label_time, nullptr, LV_ALIGN_CENTER, 0, -55);
      } else {
        lv_obj_align(label_time, nullptr, LV_ALIGN_CENTER, 10, -55);
      }
      lv_label_set_text(label_time_am_pm, dateTimeController.HourAMPM(true));
      lv_obj_align(label_time_am_pm, label_time, LV_ALIGN_OUT_LEFT_MID, -5, 0);
      lv_label_set_align( label_time_am_pm, LV_LABEL_ALIGN_CENTER );
    } else {
      lv_label_set_text_fmt(label_time, "%02i", sHour);
      lv_obj_align(label_time, nullptr, LV_ALIGN_CENTER, 0, -55);
    }
    
    lv_label_set_text_fmt(label_time_min,  "%02i", sMinute);
    lv_obj_align(label_time_min, nullptr, LV_ALIGN_CENTER, 0, 55);

    //lv_arc_set_value(arcHours, sHour);
    //lv_arc_set_value(arcMinutes, sMinute);
  }

  if(sSecond != second) {
    sSecond = second;
    lv_arc_set_value(seconds, second);
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

  lv_label_set_text(batteryIcon, BatteryIcon::GetBatteryIcon(batteryController.PercentRemaining()));  

  if(notificatioManager.AreNewNotificationsAvailable() == true) {
    lv_label_set_text(notificationIcon, NotificationIcon::GetIcon(true));
  } else {
    lv_label_set_text(notificationIcon, NotificationIcon::GetIcon(false));
  }

}

bool WatchFaceMinimal::Refresh() {
  return true;
}