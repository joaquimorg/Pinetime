#include <cstdio>

#include <libs/date/includes/date/date.h>
#include "components/datetime/DateTimeController.h"
#include <libs/lvgl/lvgl.h>
#include "Clock.h"
#include "../DisplayApp.h"
#include "BatteryIcon.h"
#include "BleIcon.h"
#include "Symbols.h"
#include "components/ble/NotificationManager.h"
#include "NotificationIcon.h"

using namespace Pinetime::Applications::Screens;

extern lv_style_t* LabelStyle76;
extern lv_style_t* LabelStyle42;
extern lv_style_t* DefaultStyle;

static void event_handler(lv_obj_t * obj, lv_event_t event) {
  Clock* screen = static_cast<Clock *>(obj->user_data);
  screen->OnObjectEvent(obj, event);
}

Clock::Clock(DisplayApp* app,
        Controllers::DateTime& dateTimeController,
        Controllers::Battery& batteryController,
        Controllers::Ble& bleController,
        Controllers::NotificationManager& notificatioManager) : Screen(app), currentDateTime{{}},
                                           dateTimeController{dateTimeController}, batteryController{batteryController},
                                           bleController{bleController}, notificatioManager{notificatioManager} {

  sHour = 99;
  sMinute = 99;
  sSecond = 99;

  batteryIcon = lv_label_create(lv_scr_act(), nullptr);
  lv_label_set_text(batteryIcon, Symbols::batteryHalf);
  lv_obj_align(batteryIcon, lv_scr_act(), LV_ALIGN_IN_TOP_RIGHT, -5, 2);

  batteryPlug = lv_label_create(lv_scr_act(), nullptr);
  lv_label_set_text(batteryPlug, Symbols::plug);
  lv_obj_align(batteryPlug, batteryIcon, LV_ALIGN_OUT_LEFT_MID, -5, 0);
  
  lv_style_copy(&ble_style, DefaultStyle);
  bleIcon = lv_label_create(lv_scr_act(), nullptr);
  ble_style.text.color = lv_color_hex(0x444444);  
  //ble_style.text.color = lv_color_hex(0x0000FF);  
  lv_label_set_style(bleIcon, LV_LABEL_STYLE_MAIN, &ble_style);
  lv_label_set_text(bleIcon, Symbols::bluetooth);
  lv_obj_align(bleIcon, nullptr, LV_ALIGN_IN_TOP_LEFT, 30, 0);

  static lv_style_t not_style;
  lv_style_copy(&not_style, DefaultStyle);

  notificationIcon = lv_label_create(lv_scr_act(), NULL);
  not_style.text.color = lv_color_hex(0x00FF00);  
  lv_label_set_style(notificationIcon, LV_LABEL_STYLE_MAIN, &not_style);
  lv_label_set_text(notificationIcon, NotificationIcon::GetIcon(false));
  lv_obj_align(notificationIcon, nullptr, LV_ALIGN_IN_TOP_LEFT, 10, 0);


  // Time
  label_time = lv_label_create(lv_scr_act(), NULL);
  lv_label_set_style(label_time, LV_LABEL_STYLE_MAIN, LabelStyle76);
  lv_label_set_text_fmt(label_time,  "%02i:%02i", 0, 0);      
  lv_label_set_align( label_time, LV_LABEL_ALIGN_CENTER );    
  lv_obj_align(label_time, lv_scr_act(), LV_ALIGN_CENTER, -15, 0);


  // Seconds
  static lv_style_t seconds_style;
  lv_style_copy(&seconds_style, LabelStyle42);

  label_time_sec = lv_label_create(lv_scr_act(), NULL);
  seconds_style.text.color = lv_color_hex(0x444444);  
  lv_label_set_style(label_time_sec, LV_LABEL_STYLE_MAIN, &seconds_style);
  lv_label_set_text_fmt(label_time_sec,  "%02i", 0);
  lv_label_set_align( label_time_sec, LV_LABEL_ALIGN_CENTER );
  lv_obj_align(label_time_sec, label_time, LV_ALIGN_OUT_RIGHT_TOP, 5, 0);

  // Date - Month / year
  static lv_style_t dateyear_style;
  lv_style_copy(&dateyear_style, DefaultStyle);  
  label_date = lv_label_create(lv_scr_act(), nullptr);
  dateyear_style.text.color = lv_color_hex(0x8400C2);  
  lv_label_set_style(label_date, LV_LABEL_STYLE_MAIN, &dateyear_style);
  lv_label_set_text_fmt(label_date, "%s %d", "JAN", 2020);
  lv_label_set_align( label_date, LV_LABEL_ALIGN_CENTER );
  lv_obj_align(label_date, label_time, LV_ALIGN_OUT_BOTTOM_RIGHT, 40, 10);

  // Date - Day / Week day
  static lv_style_t date_style;
  lv_style_copy(&date_style, DefaultStyle);

  label_date_day = lv_label_create(lv_scr_act(), NULL);
  date_style.text.color = lv_color_hex(0xC7B300);  
  lv_label_set_style(label_date_day, LV_LABEL_STYLE_MAIN, &date_style);
  lv_label_set_text_fmt(label_date_day,  "%s %02i", "SATURDAY", 1);
  //lv_label_set_align( label_date_day, LV_LABEL_ALIGN_CENTER );    
  lv_obj_align(label_date_day, label_time, LV_ALIGN_OUT_TOP_LEFT, 0, -12);  

  /*heartbeatIcon = lv_label_create(lv_scr_act(), nullptr);
  lv_label_set_text(heartbeatIcon, Symbols::heartBeat);
  lv_obj_align(heartbeatIcon, lv_scr_act(), LV_ALIGN_IN_BOTTOM_LEFT, 5, -2);

  heartbeatValue = lv_label_create(lv_scr_act(), nullptr);
  lv_label_set_text(heartbeatValue, "0");
  lv_obj_align(heartbeatValue, heartbeatIcon, LV_ALIGN_OUT_RIGHT_MID, 5, 0);

  heartbeatBpm = lv_label_create(lv_scr_act(), nullptr);
  lv_label_set_text(heartbeatBpm, "BPM");
  lv_obj_align(heartbeatBpm, heartbeatValue, LV_ALIGN_OUT_RIGHT_MID, 5, 0);*/

  static lv_style_t step_style;
  lv_style_copy(&step_style, DefaultStyle);
  step_style.text.color = lv_color_hex(0x4678C2);  

  stepIcon = lv_label_create(lv_scr_act(), nullptr);
  lv_label_set_style(stepIcon, LV_LABEL_STYLE_MAIN, &step_style);
  lv_label_set_text(stepIcon, Symbols::shoe);
  lv_obj_align(stepIcon, lv_scr_act(), LV_ALIGN_IN_BOTTOM_LEFT, 5, -2);

  stepValue = lv_label_create(lv_scr_act(), nullptr);
  lv_label_set_style(stepValue, LV_LABEL_STYLE_MAIN, &step_style);
  lv_label_set_text(stepValue, "2500");
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

Clock::~Clock() {
  lv_obj_clean(lv_scr_act());
}

bool Clock::Refresh() {
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
      ble_style.text.color = lv_color_hex(0x0000FF);  
    } else {
      //lv_label_set_text(bleIcon, BleIcon::GetIcon(false));
      ble_style.text.color = lv_color_hex(0x444444);  
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
  }

  currentDateTime = dateTimeController.CurrentDateTime();

  if(currentDateTime.IsUpdated()) {
        auto newDateTime = currentDateTime.Get();

    auto dp = date::floor<date::days>(newDateTime);
    auto time = date::make_time(newDateTime-dp);
    auto yearMonthDay = date::year_month_day(dp);

    auto year = (int)yearMonthDay.year();
    auto month = static_cast<Pinetime::Controllers::DateTime::Months>((unsigned)yearMonthDay.month());
    auto day = (unsigned)yearMonthDay.day();
    auto dayOfWeek = static_cast<Pinetime::Controllers::DateTime::Days>(date::weekday(yearMonthDay).iso_encoding());

    uint8_t hour = static_cast<int>(time.hours().count());
    uint8_t minute = static_cast<int>(time.minutes().count());
    uint8_t second = static_cast<int>(time.seconds().count());

    if(sHour != hour || sMinute != minute || sSecond != second) {
      sHour = hour;
      sMinute = minute;
      sSecond = second;
      lv_label_set_text_fmt(label_time,  "%02i:%02i", sHour, sMinute);
      lv_label_set_text_fmt(label_time_sec,  "%02i", sSecond);
    }
  

    if ((year != currentYear) || (month != currentMonth) || (dayOfWeek != currentDayOfWeek) || (day != currentDay)) {

      lv_label_set_text_fmt(label_date_day,  "%s %02i", DayOfWeekToString(dayOfWeek), day);

      lv_label_set_text_fmt(label_date, "%s %d", MonthToString(month), year);

      currentYear = year;
      currentMonth = month;
      currentDayOfWeek = dayOfWeek;
      currentDay = day;
    }
  }

  // TODO heartbeat = heartBeatController.GetValue();
  /*if(heartbeat.IsUpdated()) {
    char heartbeatBuffer[4];
    sprintf(heartbeatBuffer, "%d", heartbeat.Get());
    lv_label_set_text(heartbeatValue, heartbeatBuffer);
    lv_obj_align(heartbeatIcon, lv_scr_act(), LV_ALIGN_IN_BOTTOM_LEFT, 5, -2);
    lv_obj_align(heartbeatValue, heartbeatIcon, LV_ALIGN_OUT_RIGHT_MID, 5, 0);
    lv_obj_align(heartbeatBpm, heartbeatValue, LV_ALIGN_OUT_RIGHT_MID, 5, 0);
  }*/

  // TODO stepCount = stepController.GetValue();
  if(stepCount.IsUpdated()) {
    char stepBuffer[5];
    sprintf(stepBuffer, "%lu", stepCount.Get());
    lv_label_set_text(stepValue, stepBuffer);
    //lv_obj_align(stepValue, lv_scr_act(), LV_ALIGN_IN_BOTTOM_RIGHT, -5, -2);
    //lv_obj_align(stepIcon, stepValue, LV_ALIGN_OUT_LEFT_MID, -5, 0);
  }

  return running;
}

const char *Clock::MonthToString(Pinetime::Controllers::DateTime::Months month) {
  return Clock::MonthsString[static_cast<uint8_t>(month)];
}

const char *Clock::DayOfWeekToString(Pinetime::Controllers::DateTime::Days dayOfWeek) {
  return Clock::DaysString[static_cast<uint8_t>(dayOfWeek)];
}

const char *Clock::DayOfWeekShortToString(Pinetime::Controllers::DateTime::Days dayOfWeek) {
  return Clock::DaysStringShort[static_cast<uint8_t>(dayOfWeek)];
}

char const *Clock::DaysString[] = {
        "",
        "MONDAY",
        "TUESDAY",
        "WEDNESDAY",
        "THURSDAY",
        "FRIDAY",
        "SATURDAY",
        "SUNDAY"
};

char const *Clock::DaysStringShort[] = {
        "",
        "MON",
        "TUE",
        "WED",
        "THU",
        "FRI",
        "SAT",
        "SUN"
};

char const *Clock::MonthsString[] = {
        "",
        "JAN",
        "FEB",
        "MAR",
        "APR",
        "MAY",
        "JUN",
        "JUL",
        "AUG",
        "SEP",
        "OCT",
        "NOV",
        "DEC"
};

void Clock::OnObjectEvent(lv_obj_t *obj, lv_event_t event) {
  if(obj == backgroundLabel) {
    if (event == LV_EVENT_CLICKED) {

      running = false;
    }
  }
}

bool Clock::OnButtonPushed() {
  running = false;
  return false;
}


