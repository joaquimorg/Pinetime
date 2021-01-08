#pragma once

#include <lvgl/lvgl.h>
#include <cstdint>
#include "Screen.h"
#include "systemtask/SystemTask.h"
#include "drivers/BMA421.h"

namespace Pinetime {
  namespace Drivers {
    class BMA421;
  }

  namespace Controllers {
    class Settings;
  }
  namespace Applications {
    namespace Screens {

      class Paddle : public Screen{
        public:
          Paddle(DisplayApp* app, 
            System::SystemTask &systemTask, 
            Pinetime::Drivers::BMA421& stepCounter,
            Controllers::Settings &settingsController);

          ~Paddle() override;

          bool Refresh() override;
          bool OnButtonPushed() override;
          
        private:  
          
          Pinetime::System::SystemTask& systemTask;
          Pinetime::Drivers::BMA421& stepCounter;
          Pinetime::Controllers::Settings& settingsController;          

          int paddlePos = 120;
          int lastY = 0;
          int centerAccelX = 0;

          int ballX = -40;	            // Initial x_coordinate for the ball (12px offset from the center to counteract the ball's 24px size)
          int ballY = 0;	            // Initial y_coordinate for the ball

          int dx = 4;		            // Velocity of the ball in the x_coordinate
          int dy = 5;		            // Velocity of the ball in the y_coordinate

          int counter = 0;
          int counterRefresh = 0;
          int score = 0;   

          lv_point_t paddlePoint[2];
          lv_obj_t* paddleObj;
          lv_obj_t* ballObj;

          lv_obj_t* points;
          lv_obj_t* gameMsg;
          lv_obj_t* gameInfo;
          lv_obj_t* paddle_image;		// pointer to paddle image
          lv_obj_t* ball_image;		// pointer to ball image

          bool running = true;
          bool gameRunning = false;

          struct Pinetime::Drivers::accl_data_struct accelData;

          void DrawPaddle();
          void DrawGame();
          void DrawInfo();
          void GetAccel();
      };
    }
  }
}
