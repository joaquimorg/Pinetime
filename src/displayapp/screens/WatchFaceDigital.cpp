#include <libs/lvgl/lvgl.h>
#include "WatchFaceDigital.h"
#include "BatteryIcon.h"
#include "BleIcon.h"
#include "Symbols.h"
#include "NotificationIcon.h"
#include "drivers/BMA421.h"

using namespace Pinetime::Applications::Screens;

extern lv_style_t* LabelStyle76;
extern lv_style_t* LabelStyle42;
extern lv_style_t* DefaultStyle;

WatchFaceDigital::WatchFaceDigital(Pinetime::Applications::DisplayApp *app,
                  Controllers::DateTime& dateTimeController,
                  Controllers::Battery& batteryController,
                  Controllers::Ble& bleController,
                  Controllers::NotificationManager& notificatioManager,
                  Controllers::Settings &settingsController,
                  Drivers::BMA421& stepCounter
                  ) : Screen(app), 
                  currentDateTime{{}},
                  dateTimeController{dateTimeController}, batteryController{batteryController},
                  bleController{bleController}, notificatioManager{notificatioManager},
                  settingsController{settingsController},
                  stepCounter{stepCounter}
{
  settingsController.SetClockFace(0);
  uint16_t year = dateTimeController.Year();
  uint8_t day = dateTimeController.Day();

  uint8_t hour = dateTimeController.Hours();
  uint8_t minute = dateTimeController.Minutes();
  uint8_t second = dateTimeController.Seconds();

  sHour = hour;
  sMinute = minute;
  sSecond = second;

  // Set the background to Black
  lv_obj_set_style_local_bg_color(lv_scr_act(), LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, lv_color_make(0, 0, 0));

  batteryIcon = lv_label_create(lv_scr_act(), nullptr);
  lv_label_set_text(batteryIcon, Symbols::batteryHalf);
  lv_obj_align(batteryIcon, lv_scr_act(), LV_ALIGN_IN_TOP_RIGHT, -5, 2);

  batteryPlug = lv_label_create(lv_scr_act(), nullptr);
  lv_label_set_text(batteryPlug, Symbols::plug);
  lv_obj_align(batteryPlug, batteryIcon, LV_ALIGN_OUT_LEFT_MID, -5, 0);
  
  lv_style_init(&ble_style);
  bleIcon = lv_label_create(lv_scr_act(), nullptr);
  lv_style_set_text_color(&ble_style, LV_STATE_DEFAULT, lv_color_hex(0x444444));
  lv_obj_add_style(bleIcon, LV_LABEL_PART_MAIN, &ble_style);
  lv_label_set_text(bleIcon, Symbols::bluetooth);
  lv_obj_align(bleIcon, nullptr, LV_ALIGN_IN_TOP_LEFT, 40, 0);

  static lv_style_t not_style;
  lv_style_init(&not_style);

  notificationIcon = lv_label_create(lv_scr_act(), NULL);
  lv_style_set_text_color(&not_style, LV_STATE_DEFAULT, lv_color_hex(0x00FF00));
  lv_obj_add_style(notificationIcon, LV_LABEL_PART_MAIN, &not_style);
  lv_label_set_text(notificationIcon, NotificationIcon::GetIcon(false));
  lv_obj_align(notificationIcon, nullptr, LV_ALIGN_IN_TOP_LEFT, 10, 0);


  // Time
  static lv_style_t time_style;
  lv_style_init(&time_style);
  lv_style_set_text_font(&time_style, LV_STATE_DEFAULT, &lv_font_clock_76);
  //lv_style_set_text_color(&time_style, LV_STATE_DEFAULT, lv_color_hex(0xFFFFFF));

  label_time = lv_label_create(lv_scr_act(), NULL);
  lv_obj_add_style(label_time, LV_LABEL_PART_MAIN, &time_style);  
  lv_label_set_text_fmt(label_time,  "%02i:%02i", sHour, sMinute);      
  lv_label_set_align( label_time, LV_LABEL_ALIGN_CENTER );    
  lv_obj_align(label_time, lv_scr_act(), LV_ALIGN_CENTER, -15, 0);


  // Seconds
  static lv_style_t seconds_style;
  lv_style_init(&seconds_style);
  lv_style_set_text_font(&seconds_style, LV_STATE_DEFAULT, &lv_font_clock_42);
  label_time_sec = lv_label_create(lv_scr_act(), NULL);
  
  lv_style_set_text_color(&seconds_style, LV_STATE_DEFAULT, lv_color_hex(0x444444));  
  lv_obj_add_style(label_time_sec, LV_LABEL_PART_MAIN, &seconds_style);
  lv_label_set_text_fmt(label_time_sec,  "%02i", sSecond);
  lv_label_set_align( label_time_sec, LV_LABEL_ALIGN_CENTER );
  lv_obj_align(label_time_sec, label_time, LV_ALIGN_OUT_RIGHT_TOP, 5, 0);

  // Date - Month / year
  static lv_style_t dateyear_style;
  lv_style_init(&dateyear_style);
  label_date = lv_label_create(lv_scr_act(), nullptr);
  lv_style_set_text_color(&dateyear_style, LV_STATE_DEFAULT, lv_color_hex(0x8400C2));    
  lv_obj_add_style(label_date, LV_LABEL_PART_MAIN, &dateyear_style);
  lv_label_set_text_fmt(label_date, "%s %d", dateTimeController.MonthToString(), year);
  lv_label_set_align( label_date, LV_LABEL_ALIGN_CENTER );
  lv_obj_align(label_date, label_time, LV_ALIGN_OUT_BOTTOM_RIGHT, 40, 10);

  // Date - Day / Week day
  static lv_style_t date_style;
  lv_style_init(&date_style);

  label_date_day = lv_label_create(lv_scr_act(), NULL);

  lv_style_set_text_color(&date_style, LV_STATE_DEFAULT, lv_color_hex(0xC7B300));    
  lv_obj_add_style(label_date_day, LV_LABEL_PART_MAIN, &date_style);  
  lv_label_set_text_fmt(label_date_day,  "%s %02i", dateTimeController.DayOfWeekToString(), day);
  //lv_label_set_align( label_date_day, LV_LABEL_ALIGN_CENTER );    
  lv_obj_align(label_date_day, label_time, LV_ALIGN_OUT_TOP_LEFT, 0, -12);  

  heartbeatIcon = lv_label_create(lv_scr_act(), nullptr);
  lv_label_set_text(heartbeatIcon, Symbols::heartBeat);
  lv_obj_align(heartbeatIcon, lv_scr_act(), LV_ALIGN_IN_BOTTOM_RIGHT, -50, -2);

  heartbeatValue = lv_label_create(lv_scr_act(), nullptr);
  if ( settingsController.GetHeartRate() == 0 )
    lv_label_set_text(heartbeatValue, "--");
  else
    lv_label_set_text_fmt(heartbeatValue, "%02i", settingsController.GetHeartRate());

  lv_obj_align(heartbeatValue, heartbeatIcon, LV_ALIGN_OUT_RIGHT_MID, 5, 0);

  /*heartbeatBpm = lv_label_create(lv_scr_act(), nullptr);
  lv_label_set_text(heartbeatBpm, "BPM");
  lv_obj_align(heartbeatBpm, heartbeatValue, LV_ALIGN_OUT_RIGHT_MID, 5, 0);*/

  static lv_style_t step_style;
  lv_style_init(&step_style);

  lv_style_set_text_color(&step_style, LV_STATE_DEFAULT, lv_color_hex(0x4678C2));    

  stepIcon = lv_label_create(lv_scr_act(), nullptr);
  lv_obj_add_style(stepIcon, LV_LABEL_PART_MAIN, &step_style);
  lv_label_set_text(stepIcon, Symbols::shoe);
  lv_obj_align(stepIcon, lv_scr_act(), LV_ALIGN_IN_BOTTOM_LEFT, 5, -2);

  stepValue = lv_label_create(lv_scr_act(), nullptr);
  lv_obj_add_style(stepValue, LV_LABEL_PART_MAIN, &step_style);
  lv_label_set_text_fmt(stepValue, "%lu", stepCount.Get());
  lv_obj_align(stepValue, stepIcon, LV_ALIGN_OUT_RIGHT_MID, 5, 0);


  backgroundLabel = lv_label_create(lv_scr_act(), nullptr);
  //backgroundLabel->user_data = this;
  //lv_obj_set_click(backgroundLabel, true);
  //lv_obj_set_event_cb(backgroundLabel, event_handler);
  lv_label_set_long_mode(backgroundLabel, LV_LABEL_LONG_CROP);
  lv_obj_set_size(backgroundLabel, 240, 240);
  lv_obj_set_pos(backgroundLabel, 0, 0);
  lv_label_set_text(backgroundLabel, "");

}

WatchFaceDigital::~WatchFaceDigital() {
  lv_obj_clean(lv_scr_act());
}



bool WatchFaceDigital::Refresh() {

  batteryPercentRemaining = batteryController.PercentRemaining();
  if (batteryPercentRemaining.IsUpdated()) {
    auto batteryPercent = batteryPercentRemaining.Get();
    lv_label_set_text(batteryIcon, BatteryIcon::GetBatteryIcon(batteryPercent));
    auto isCharging = batteryController.IsCharging() || batteryController.IsPowerPresent();
    lv_label_set_text(batteryPlug, BatteryIcon::GetPlugIcon(isCharging));
  }

  bleState = bleController.IsConnected();
  if (bleState.IsUpdated()) {
    if(bleState.Get() == true) {
      //lv_label_set_text(bleIcon, BleIcon::GetIcon(true));
      lv_style_set_text_color(&ble_style, LV_STATE_DEFAULT, lv_color_hex(0x0000FF));  
    } else {
      //lv_label_set_text(bleIcon, BleIcon::GetIcon(false));
      lv_style_set_text_color(&ble_style, LV_STATE_DEFAULT, lv_color_hex(0x444444));  
    }
  }

  /*lv_obj_align(batteryIcon, lv_scr_act(), LV_ALIGN_IN_TOP_RIGHT, -5, 5);
  lv_obj_align(batteryPlug, batteryIcon, LV_ALIGN_OUT_LEFT_MID, -5, 0);
  lv_obj_align(bleIcon, batteryPlug, LV_ALIGN_OUT_LEFT_MID, -5, 0);*/

  notificationState = notificatioManager.AreNewNotificationsAvailable();

  if(notificationState.IsUpdated()) {
    if(notificationState.Get() == true)
      lv_label_set_text(notificationIcon, NotificationIcon::GetIcon(true));
    else
      lv_label_set_text(notificationIcon, NotificationIcon::GetIcon(false));
      //lv_label_set_text(notificationIcon, NotificationIcon::GetIcon(true));
  }

  currentDateTime = dateTimeController.CurrentDateTime();

  if(currentDateTime.IsUpdated()) {

    uint16_t year = dateTimeController.Year();
    auto month = dateTimeController.Month();
    uint8_t day = dateTimeController.Day();
    auto dayOfWeek = dateTimeController.DayOfWeek();

    uint8_t hour = dateTimeController.Hours();
    uint8_t minute = dateTimeController.Minutes();
    uint8_t second = dateTimeController.Seconds();

    if(sHour != hour || sMinute != minute || sSecond != second) {
      sHour = hour;
      sMinute = minute;
      sSecond = second;
      lv_label_set_text_fmt(label_time,  "%02i:%02i", sHour, sMinute);
      lv_label_set_text_fmt(label_time_sec,  "%02i", sSecond);
    }
  
    if ((year != currentYear) || (month != currentMonth) || (dayOfWeek != currentDayOfWeek) || (day != currentDay)) {

      lv_label_set_text_fmt(label_date_day,  "%s %02i", dateTimeController.DayOfWeekToString(), day);

      lv_label_set_text_fmt(label_date, "%s %d", dateTimeController.MonthToString(), year);

      currentYear = year;
      currentMonth = month;
      currentDayOfWeek = dayOfWeek;
      currentDay = day;
    }
  }

  // TODO heartbeat = heartBeatController.GetValue();
  /*if(heartbeat.IsUpdated()) {    
    lv_label_set_text_fmt(heartbeatValue, "%d", heartbeat.Get());
    //lv_obj_align(heartbeatIcon, lv_scr_act(), LV_ALIGN_IN_BOTTOM_LEFT, 5, -2);
    //lv_obj_align(heartbeatValue, heartbeatIcon, LV_ALIGN_OUT_RIGHT_MID, 5, 0);
    //lv_obj_align(heartbeatBpm, heartbeatValue, LV_ALIGN_OUT_RIGHT_MID, 5, 0);
  }*/

  stepCount = stepCounter.GetSteps();
  if(stepCount.IsUpdated()) {        
    lv_label_set_text_fmt(stepValue, "%lu", stepCount.Get());
    //lv_obj_align(stepValue, lv_scr_act(), LV_ALIGN_IN_BOTTOM_RIGHT, -5, -2);
    //lv_obj_align(stepIcon, stepValue, LV_ALIGN_OUT_LEFT_MID, -5, 0);
  }

  //return running;
  return true;
}