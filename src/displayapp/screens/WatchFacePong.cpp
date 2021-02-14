#include <libs/lvgl/lvgl.h>
#include "WatchFacePong.h"
#include "BatteryIcon.h"
#include "BleIcon.h"
#include "Symbols.h"
#include "NotificationIcon.h"

#define BALLSIZE 6
#define PLAYERSIZE 20

using namespace Pinetime::Applications::Screens;

static void lv_update_task(struct _lv_task_t *task) {  
  auto user_data = static_cast<WatchFacePong *>(task->user_data);
  user_data->UpdateScreen();
}

static void lv_play_task(struct _lv_task_t *task) {  
  auto user_data = static_cast<WatchFacePong *>(task->user_data);
  user_data->pong_play();
}

WatchFacePong::WatchFacePong(Pinetime::Applications::DisplayApp *app,
                  Controllers::DateTime& dateTimeController,
                  Controllers::Battery& batteryController,
                  Controllers::Ble& bleController,
                  Controllers::NotificationManager& notificatioManager,
                  Controllers::Settings &settingsController) : Screen(app), currentDateTime{{}},
                                           dateTimeController{dateTimeController}, batteryController{batteryController},
                                           bleController{bleController}, notificatioManager{notificatioManager},
                                           settingsController{settingsController} {
  
  settingsController.SetClockFace(3);

  uint8_t day = dateTimeController.Day();

  uint8_t hour = dateTimeController.Hours();
  uint8_t minute = dateTimeController.Minutes();
  uint8_t second = dateTimeController.Seconds();

  srand(second);

  sHour = hour;
  sMinute = minute;
  sSecond = second;

  /*notificationIcon = lv_label_create(lv_scr_act(), NULL);
  lv_label_set_text(notificationIcon, NotificationIcon::GetIcon(true));
  lv_obj_align(notificationIcon, nullptr, LV_ALIGN_IN_BOTTOM_RIGHT, 0, 0);*/

  time_h = lv_label_create(lv_scr_act(), nullptr);
  lv_obj_set_style_local_text_color(time_h, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0xFFFFFF));
  lv_obj_set_style_local_text_font(time_h, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_font_clock_42);
  lv_label_set_text_fmt(time_h, "%02i", sHour);
  lv_obj_align(time_h, NULL, LV_ALIGN_IN_TOP_MID, -30, 10);

  time_m = lv_label_create(lv_scr_act(), nullptr);
  lv_obj_set_style_local_text_color(time_m, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0xFFFFFF));
  lv_obj_set_style_local_text_font(time_m, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_font_clock_42);
  lv_label_set_text_fmt(time_m, "%02i", sMinute);
  lv_obj_align(time_m, NULL, LV_ALIGN_IN_TOP_MID, 30, 10);

  //Line style
  static lv_style_t line_style;
  lv_style_init(&line_style);
  lv_style_set_line_color(&line_style, LV_STATE_DEFAULT, lv_color_hex(0x444444));
  lv_style_set_line_width(&line_style, LV_STATE_DEFAULT, 3);
  lv_style_set_line_rounded(&line_style, LV_STATE_DEFAULT, false);

  //Top line
  static lv_point_t line_points1[] = { {0, 0}, {240, 0} };
  lv_obj_t *line1 = lv_line_create(lv_scr_act(), NULL);
  lv_line_set_points(line1, line_points1, 2);
  lv_obj_add_style(line1, LV_OBJ_PART_MAIN, &line_style);

  //Bottom line
  static lv_point_t line_points2[] = { {0, 238}, {240, 238} };
  lv_obj_t *line2 = lv_line_create(lv_scr_act(), NULL);
  lv_line_set_points(line2, line_points2, 2);
  lv_obj_add_style(line2, LV_OBJ_PART_MAIN, &line_style);

  //Middle line
  static lv_point_t line_points3[] = { {120, 2}, {120, 237} };
  lv_obj_t *line3 = lv_line_create(lv_scr_act(), NULL);
  lv_line_set_points(line3, line_points3, 2);
  lv_obj_add_style(line3, LV_OBJ_PART_MAIN, &line_style);


  //Ball style
  static lv_style_t ball_style;
  lv_style_init(&ball_style);
  lv_style_set_line_color(&ball_style, LV_STATE_DEFAULT, lv_color_hex(0xFFFF00));
  lv_style_set_line_width(&ball_style, LV_STATE_DEFAULT, BALLSIZE * 2);
  lv_style_set_line_rounded(&ball_style, LV_STATE_DEFAULT, true);

  ball = lv_line_create(lv_scr_act(), NULL);
  lv_obj_add_style(ball, LV_OBJ_PART_MAIN, &ball_style);

  draw_ball();


  //Player style
  static lv_style_t player_style;
  lv_style_init(&player_style);
  lv_style_set_line_color(&player_style, LV_STATE_DEFAULT, lv_color_hex(0x00FF00));
  lv_style_set_line_width(&player_style, LV_STATE_DEFAULT, 6);
  lv_style_set_line_rounded(&player_style, LV_STATE_DEFAULT, true);

  player1 = lv_line_create(lv_scr_act(), NULL);
  lv_obj_add_style(player1, LV_OBJ_PART_MAIN, &player_style);

  draw_player1();

  player2 = lv_line_create(lv_scr_act(), NULL);
  lv_obj_add_style(player2, LV_OBJ_PART_MAIN, &player_style);

  draw_player2();

  /*lv_obj_t* backgroundLabel = lv_label_create(lv_scr_act(), nullptr);
  lv_label_set_long_mode(backgroundLabel, LV_LABEL_LONG_CROP);
  lv_obj_set_size(backgroundLabel, 240, 240);
  lv_obj_set_pos(backgroundLabel, 0, 0);
  lv_label_set_text(backgroundLabel, "");*/

  taskUpdate = lv_task_create(lv_update_task, 500, LV_TASK_PRIO_MID, this);

  taskPlay = lv_task_create(lv_play_task, 50, LV_TASK_PRIO_MID, this);

}

WatchFacePong::~WatchFacePong() {
 
  lv_task_del(taskPlay);
  lv_task_del(taskUpdate);
  lv_obj_clean(lv_scr_act());
}


void WatchFacePong::draw_player1() {

  uint8_t x = 3;
  uint8_t y = player1y;
  
  if ( y < PLAYERSIZE + 3 ) y = PLAYERSIZE + 3;
  if ( y > 240 - (PLAYERSIZE + 3) ) y = 240 - (PLAYERSIZE + 3);

  palyer1_points[0].x = x;
  palyer1_points[0].y = y - PLAYERSIZE;

  palyer1_points[1].x = x;
  palyer1_points[1].y = y + PLAYERSIZE;

  lv_line_set_points(player1, palyer1_points, 2);
}

void WatchFacePong::draw_player2() {

  uint8_t x = 237;
  uint8_t y = player2y;
  
  if ( y < PLAYERSIZE + 3 ) y = PLAYERSIZE + 3;
  if ( y > 240 - (PLAYERSIZE + 3) ) y = 240 - (PLAYERSIZE + 3);

  palyer2_points[0].x = x;
  palyer2_points[0].y = y - PLAYERSIZE;

  palyer2_points[1].x = x;
  palyer2_points[1].y = y + PLAYERSIZE;

  lv_line_set_points(player2, palyer2_points, 2);
}

void WatchFacePong::draw_ball() {

  ball_points[0].x = ballx;
  ball_points[0].y = bally;

  ball_points[1].x = ballx + 1;
  ball_points[1].y = bally + 1;
  lv_line_set_points(ball, ball_points, 2);

}

void WatchFacePong::ball_angle() {
  int16_t ang;
  ang = rand() % 360; //random(360);
  if ( ang < 100 ) diry = 0;
  if ( (ang > 100) && (ang < 240) ) diry = 1;
  if ( ang > 240 ) diry = 2;

  angball = rand() % 360; //random(360);
  angball = angball % 10;
  angball = angball % 5;
  if (angball == 0) angball = 1;
}

void WatchFacePong::pong_play() {

  if ( delaynew == 0 ) {

    if (dirx == 1) ballx += BALLSIZE;
    if (dirx == 0) ballx -= BALLSIZE;

    if (diry == 1) bally += BALLSIZE / angball;
    if (diry == 0) bally -= BALLSIZE / angball;
  } else {
    delaynew--;
  }

  draw_ball();

  if ( ((bally >= player1y - PLAYERSIZE) && (bally <= player1y + PLAYERSIZE)) && (ballx == (BALLSIZE * 2))) {
    dirx = 1;
    ball_angle();
  } else if (ballx == 0) {
    point = 1;
  }

  if ( ((bally >= player2y - PLAYERSIZE) && (bally <= player2y + PLAYERSIZE)) && (ballx == 240 - (BALLSIZE * 2)) ) {
    dirx = 0;
    ball_angle();
    //Serial.println(angball);
  } else if (ballx == 240 - BALLSIZE ) {
    point = 2;
  }

  if ( delaynew == 0 ) {
    if ( (dirx == 0) && (ballx < 160) ) {

      if ( player1y > bally ) player1y -= BALLSIZE;
      if ( player1y < bally ) player1y += BALLSIZE;

      draw_player1();
    }

    if ( (dirx == 1) && (ballx > 80) ) {

      if ( player2y > bally ) player2y -= BALLSIZE;
      if ( player2y < bally ) player2y += BALLSIZE;

      draw_player2();
    }
  } else {

    if ( player1y > bally ) player1y -= BALLSIZE;
    if ( player1y < bally ) player1y += BALLSIZE;

    draw_player1();

    if ( player2y > bally ) player2y -= BALLSIZE;
    if ( player2y < bally ) player2y += BALLSIZE;

    draw_player2();

  }

  if (bally <= BALLSIZE) {
    diry = 1;
  }

  if (bally >= 240 - BALLSIZE) {
    diry = 0;
  }

  if (point == 1 || point == 2) {
    if ( point == 1 ) {
      ballx = 228;
    } else {
      ballx = 12;
    }
    point = 0;
    bally = 120;
    ball_angle();
    delaynew = 100;
  }

}

void WatchFacePong::UpdateScreen() {
  uint8_t hour = dateTimeController.Hours();
  uint8_t minute = dateTimeController.Minutes();
  lv_label_set_text_fmt(time_h, "%02i", hour);
  lv_label_set_text_fmt(time_m, "%02i", minute);
}

bool WatchFacePong::Refresh() {

  /*notificationState = notificatioManager.AreNewNotificationsAvailable();

  if(notificationState.IsUpdated()) {
    if(notificationState.Get() == true)
      lv_label_set_text(notificationIcon, NotificationIcon::GetIcon(true));
    else
      lv_label_set_text(notificationIcon, NotificationIcon::GetIcon(false));
  }*/

  //pong_play();

  /*
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
        lv_style_set_text_color(&sep_style, LV_STATE_DEFAULT, lv_color_hex(0x000000));
      }
      lv_label_set_text_static(label_time_sep,  ":");   
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
  */

  return true;
}