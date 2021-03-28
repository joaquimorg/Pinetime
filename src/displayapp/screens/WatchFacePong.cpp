#include <math.h>
#include <libs/lvgl/lvgl.h>
#include "WatchFacePong.h"
#include "BatteryIcon.h"
#include "BleIcon.h"
#include "Symbols.h"
#include "NotificationIcon.h"
#include "resources_config.h"

#define BALLSIZE 4
#define PLAYERSIZE 10

//LV_IMG_DECLARE(background_16_1);
//LV_IMG_DECLARE(background_16_2);
//LV_FONT_DECLARE(pong_48)

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
                  Controllers::Settings &settingsController) : Screen(app),
                                           dateTimeController{dateTimeController}, batteryController{batteryController},
                                           bleController{bleController}, notificatioManager{notificatioManager},
                                           settingsController{settingsController} {
  
  settingsController.SetClockFace(3);
  settingsController.SaveSettings();

  srand(dateTimeController.Seconds());

  // Set the background
  lv_obj_set_style_local_bg_color(lv_scr_act(), LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0x001020));

  /*lv_obj_t * bg_clock_img_1 = lv_img_create(lv_scr_act(), NULL);
  lv_img_set_src(bg_clock_img_1, &background_16_2);
  lv_obj_align(bg_clock_img_1, NULL, LV_ALIGN_CENTER, 0, 0);

  lv_obj_t * bg_clock_img_2 = lv_img_create(lv_scr_act(), NULL);
  lv_img_set_src(bg_clock_img_2, &background_16_1);
  lv_obj_align(bg_clock_img_2, NULL, LV_ALIGN_CENTER, 0, 0);*/


  /*notificationIcon = lv_label_create(lv_scr_act(), NULL);
  lv_label_set_text(notificationIcon, NotificationIcon::GetIcon(true));
  lv_obj_align(notificationIcon, nullptr, LV_ALIGN_IN_BOTTOM_RIGHT, 0, 0);*/

  time_h = lv_label_create(lv_scr_act(), nullptr);
  lv_obj_set_style_local_text_color(time_h, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0xFFFFFF));
  lv_obj_set_style_local_text_font(time_h, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_font_clock_76);

  time_m = lv_label_create(lv_scr_act(), nullptr);
  lv_obj_set_style_local_text_color(time_m, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0xFFFFFF));
  lv_obj_set_style_local_text_font(time_m, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_font_clock_76);

  label_date = lv_label_create(lv_scr_act(), nullptr);
  lv_obj_set_style_local_text_color(label_date, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0xFFFFFF));
  lv_obj_align(label_date, lv_scr_act(), LV_ALIGN_IN_BOTTOM_LEFT, 4, -4);

  batteryIcon = lv_label_create(lv_scr_act(), nullptr);
  lv_obj_set_style_local_text_font(batteryIcon, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_font_sys_20);
  lv_label_set_text(batteryIcon, Symbols::batteryHalf);
  lv_obj_align(batteryIcon, NULL, LV_ALIGN_IN_BOTTOM_RIGHT, -8, -4);


  // ------------------------------------------------------------------------------------------------------
  ball_s = lv_label_create(lv_scr_act(), NULL);  
  lv_obj_set_style_local_text_color(ball_s, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0x000000));
  lv_obj_set_style_local_text_font(ball_s, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_font_sys_20);
  lv_label_set_text_static(ball_s, Symbols::dot);
  lv_label_set_align( ball_s, LV_LABEL_ALIGN_CENTER );

  player1 = lv_label_create(lv_scr_act(), NULL);  
  lv_obj_set_style_local_text_color(player1, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0xFF6600));
  lv_obj_set_style_local_text_font(player1, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_font_sys_48);
  lv_label_set_text_static(player1, Symbols::tennisRacket1);
  lv_label_set_align( player1, LV_LABEL_ALIGN_LEFT );

  draw_player1();


  player2 = lv_label_create(lv_scr_act(), NULL);  
  lv_obj_set_style_local_text_color(player2, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0xFF6600));
  lv_obj_set_style_local_text_font(player2, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_font_sys_48);
  lv_label_set_text_static(player2, Symbols::tennisRacket2);
  lv_label_set_align( player2, LV_LABEL_ALIGN_RIGHT );

  draw_player2();

  ball = lv_label_create(lv_scr_act(), NULL);  
  lv_obj_set_style_local_text_color(ball, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0xFFFF00));
  lv_obj_set_style_local_text_font(ball, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_font_sys_20);
  lv_label_set_text_static(ball, Symbols::dot);
  lv_label_set_align( ball, LV_LABEL_ALIGN_CENTER );

  draw_ball();

  lv_obj_t* backgroundLabel = lv_label_create(lv_scr_act(), nullptr);
  lv_label_set_long_mode(backgroundLabel, LV_LABEL_LONG_CROP);
  lv_obj_set_size(backgroundLabel, 240, 240);
  lv_obj_set_pos(backgroundLabel, 0, 0);
  lv_label_set_text(backgroundLabel, "");

  UpdateScreen();

  taskUpdate = lv_task_create(lv_update_task, 50000, LV_TASK_PRIO_LOW, this);

  taskPlay = lv_task_create(lv_play_task, 500, LV_TASK_PRIO_MID, this);

}

WatchFacePong::~WatchFacePong() {
 
  lv_task_del(taskPlay);
  lv_task_del(taskUpdate);
  lv_obj_clean(lv_scr_act());
  lv_obj_set_style_local_bg_color(lv_scr_act(), LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, lv_color_make(0, 0, 0));
}


void WatchFacePong::draw_player1() {

  uint8_t y = player1y;
  
  if ( y < PLAYERSIZE + 3 ) y = PLAYERSIZE + 3;
  if ( y > 240 - (PLAYERSIZE + 3) ) y = 240 - (PLAYERSIZE + 3);

  lv_obj_set_pos(player1, -12, y - 12);
}

void WatchFacePong::draw_player2() {

  uint8_t y = player2y;
  
  if ( y < PLAYERSIZE + 3 ) y = PLAYERSIZE + 3;
  if ( y > 240 - (PLAYERSIZE + 3) ) y = 240 - (PLAYERSIZE + 3);

  lv_obj_set_pos(player2, 206, y - 12);
}

void WatchFacePong::draw_ball() {

  int8_t ballShadow = 15 - (abs(120 - (ballx - 9)) / 10);

  lv_obj_set_pos(ball, ballx - 9, bally - 12);
  lv_obj_set_pos(ball_s, (ballx - 9) + ballShadow, (bally - 12) + ballShadow);

}

void WatchFacePong::ball_angle() {
  int16_t ang;
  ang = rand() % 360;
  if ( ang < 100 ) diry = 0;
  if ( (ang > 100) && (ang < 240) ) diry = 1;
  if ( ang > 240 ) diry = 2;

  angball = rand() % 360;
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

    if ( player1y > ballyp1 ) player1y -= BALLSIZE;
    if ( player1y < ballyp1 ) player1y += BALLSIZE;

    draw_player1();

    if ( player2y > ballyp2 ) player2y -= BALLSIZE;
    if ( player2y < ballyp2 ) player2y += BALLSIZE;

    draw_player2();

  }

  if (bally <= BALLSIZE) {
    diry = 1;
  }

  if (bally >= 240 - BALLSIZE) {
    diry = 0;
  }

  if (point == 1 || point == 2) {
    ball_angle();
    if ( point == 1 ) {
      ballx = 228;
      bally = ballyp2;
      diry = 1;
    } else {
      ballx = 12;
      bally = ballyp1;
      diry = 0;
    }    
    point = 0;        
    delaynew = 70;
  }

}

void WatchFacePong::UpdateScreen() {

  lv_label_set_text_fmt(time_h, "%02i", dateTimeController.Hours());
  lv_obj_align(time_h, NULL, LV_ALIGN_IN_TOP_MID, -50, 10);
  lv_label_set_text_fmt(time_m, "%02i", dateTimeController.Minutes());
  lv_obj_align(time_m, NULL, LV_ALIGN_IN_TOP_MID, 50, 10);

  if ( settingsController.GetClockType() == Controllers::Settings::ClockType::H12 ) {
    lv_label_set_text_fmt(label_date, "%s, %s %02i", dateTimeController.DayOfWeekShortToStringLow(), dateTimeController.MonthToStringLow(), dateTimeController.Day());
  } else {
    lv_label_set_text_fmt(label_date, "%s, %02i %s", dateTimeController.DayOfWeekShortToStringLow(), dateTimeController.Day(), dateTimeController.MonthToStringLow());
  }  
  lv_label_set_text(batteryIcon, BatteryIcon::GetBatteryIcon(batteryController.PercentRemaining()));
}

bool WatchFacePong::Refresh() {

  return true;
}