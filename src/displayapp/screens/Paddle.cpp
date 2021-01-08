#include "Paddle.h"
#include "../DisplayApp.h"
#include "../LittleVgl.h"

using namespace Pinetime::Applications::Screens;

Paddle::Paddle(Pinetime::Applications::DisplayApp* app, 
      System::SystemTask &systemTask, 
      Pinetime::Drivers::BMA421 &stepCounter,
      Controllers::Settings &settingsController) : Screen(app), 
      systemTask{systemTask},
      stepCounter{stepCounter},
      settingsController{settingsController} {

  //app->SetTouchMode(DisplayApp::TouchModes::Polling);
	
  points = lv_label_create(lv_scr_act(), nullptr);
  lv_obj_set_style_local_text_font(points, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_font_clock_42);
  lv_obj_set_style_local_text_color(points, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0x404040));
  lv_label_set_text(points, "0000");
  lv_label_set_align( points, LV_LABEL_ALIGN_CENTER );
  lv_obj_align(points, lv_scr_act(), LV_ALIGN_IN_TOP_MID, 0, 10);
  
  ballObj = lv_label_create(lv_scr_act(), NULL);  
  lv_obj_set_style_local_text_color(ballObj, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0xFFFFFF));
  lv_label_set_text(ballObj, "");
  lv_label_set_align( ballObj, LV_LABEL_ALIGN_CENTER );
  lv_obj_set_pos(ballObj, ballX, ballY);

  paddleObj = lv_line_create(lv_scr_act(), NULL);
  lv_obj_set_style_local_line_width(paddleObj, LV_LINE_PART_MAIN, LV_STATE_DEFAULT, 7);
  lv_obj_set_style_local_line_color(paddleObj, LV_LINE_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLUE);
  lv_obj_set_style_local_line_rounded(paddleObj, LV_LINE_PART_MAIN, LV_STATE_DEFAULT, true);
  
  gameMsg = lv_label_create(lv_scr_act(), nullptr);  
  lv_obj_set_style_local_text_color(gameMsg, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0x00FFFF));
  lv_label_set_text(gameMsg, "Get ready...");
  lv_label_set_align( gameMsg, LV_LABEL_ALIGN_CENTER );
  lv_obj_align(gameMsg, lv_scr_act(), LV_ALIGN_CENTER, 0, 0);

  gameInfo = lv_label_create(lv_scr_act(), nullptr);  
  lv_obj_set_style_local_text_color(gameInfo, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0xFFFFFF));
  lv_label_set_text(gameInfo, "");  
  lv_obj_align(gameInfo, lv_scr_act(), LV_ALIGN_IN_BOTTOM_MID, 0, 0);

  lv_obj_t * backgroundLabel = lv_label_create(lv_scr_act(), nullptr);
  lv_label_set_long_mode(backgroundLabel, LV_LABEL_LONG_CROP);
  lv_obj_set_size(backgroundLabel, 240, 240);
  lv_obj_set_pos(backgroundLabel, 0, 0);
  lv_label_set_text(backgroundLabel, "");

  systemTask.PushMessage(Pinetime::System::SystemTask::Messages::DisableSleeping);
  gameRunning = false;

  stepCounter.UpdateAccel();
  accelData = stepCounter.GetAccel();
  centerAccelX = round(accelData.x / 3);
}

Paddle::~Paddle() {
  // Reset the touchmode
  //app->SetTouchMode(DisplayApp::TouchModes::Gestures);
  lv_obj_clean(lv_scr_act());
  systemTask.PushMessage(Pinetime::System::SystemTask::Messages::EnableSleeping);
}

void Paddle::DrawPaddle() {
  paddlePoint[0].x = 4;
  paddlePoint[0].y = paddlePos - 25;
  paddlePoint[1].x = 4;
  paddlePoint[1].y = paddlePos + 25;
  lv_line_set_points(paddleObj, paddlePoint, 2);
}

void Paddle::DrawGame() {
  //if((counter++ % 5) == 0){
    counter = 0;	
    
    ballX += dx;
    ballY += dy;	    
    
    //checks if it has touched the sides (floor and ceiling)	  
    if(ballY <= 0 || ballY >= 222){
      dy *= -1;
    }
  
    //checks if it has touched the side (left side)  
    if(ballX >= 222){
      dx *= -1; 
    }
  
    //checks if it is in the position of the paddle
    if( ballX >= 0 && ballX < 7) {
      if( ballY >= (paddlePos - 25) && ballY <= (paddlePos + 25) ) { 
        dx *= -1;
        dy *= -1;
        score++;
      }
    } else if( ballX <= -40 ) {
      //checks if it has gone behind the paddle

      lv_obj_set_style_local_line_color(paddleObj, LV_LINE_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_RED);
      gameRunning = false;
      counter = 0;
      lv_label_set_text(gameMsg, "Get ready\nfor new game...");
      lv_label_set_align( gameMsg, LV_LABEL_ALIGN_CENTER );
      lv_obj_align(gameMsg, lv_scr_act(), LV_ALIGN_CENTER, 0, 0);
    } 
    
    DrawPaddle();
    lv_obj_set_pos(ballObj, ballX, ballY);
  
    lv_label_set_text_fmt(points, "%04i", score);	  
  //}      
}

void Paddle::DrawInfo() {
  DrawPaddle();
  counter++;
  if ( counter > 100 ) {
    lv_obj_set_style_local_line_color(paddleObj, LV_LINE_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLUE);
    lv_label_set_text(gameMsg, "");
    ballX = 210;
    ballY = 120 - (int(rand() % 120) - 60);
    score = 0;
    counter = 0;
    gameRunning = true;
  }
  //lv_label_set_text_fmt(points, "%04i", score);
}

void Paddle::GetAccel() {
  stepCounter.UpdateAccel();
  accelData = stepCounter.GetAccel();
  int accelX = round(accelData.x / 3);

  //if ( lastY != accelX) {
    int dif = abs(accelX - centerAccelX);

    if ( centerAccelX > accelX ) {
        paddlePos = paddlePos + dif * 4;
    } else {
        paddlePos = paddlePos - dif * 4;
    }

    if ( paddlePos < 25 ) paddlePos = 25;
    if ( paddlePos > 215 ) paddlePos = 215;
    lastY = accelX;
  //}
  //lv_label_set_text_fmt(gameInfo, "%i", accelX);
}

bool Paddle::Refresh() {

  if((counterRefresh++ % 5) == 0){
    counterRefresh = 0;	
    GetAccel();

    if ( gameRunning ) {
      DrawGame();
    } else {
      DrawInfo();
    }
  }
  return running;
}

bool Paddle::OnButtonPushed() {
  running = false;
  return true;
}


