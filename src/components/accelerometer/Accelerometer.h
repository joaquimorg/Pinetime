#pragma once

#include "drivers/TwiMaster.h"
#include "bma421/bma4.h"
#include "bma421/bma421.h"

namespace Pinetime {
  namespace Controllers {
    
    class Accelerometer {
        public :

            enum class Direction {
                TOP_EDGE    = 0,
                BOTTOM_EDGE = 1,
                LEFT_EDGE   = 2,
                RIGHT_EDGE  = 3,
                DISP_UP     = 4,
                DISP_DOWN   = 5,
                NONE        = 8,
                ERROR       = 9
            } ;
            
            Accelerometer(Pinetime::Drivers::TwiMaster& twiMaster);
           
            //char* status;

            void Init();
            void Update();

            void Wakeup();
            void Sleep();
            void UpdateAccel();
            void ResetSteps();

            uint32_t GetSteps() const { return step_count; };
            uint8_t GetActivity() const { return activity; };
            uint8_t GetTapStatus() const { return tapStatus; };
            float GetTemp() const { return tempC; };

            int16_t GetAccelX() const { return accelData.x; };
            int16_t GetAccelY() const { return accelData.y; };
            int16_t GetAccelZ() const { return accelData.z; };

            void ReadIRQStatus();
            bool isStepIRQ();

            bool isReady() const { return deviceReady; };

            Accelerometer::Direction GetDirection();

            bool WristRotate();

        private:

            Pinetime::Drivers::TwiMaster& twiMaster;
            uint8_t twiAddress;

            bool deviceReady = false;
            
            struct accl_data_struct {
              int16_t x;
              int16_t y;
              int16_t z;
            };

            struct bma4_dev bma;
            struct bma4_accel_config accel_conf;
            struct accl_data_struct accelData;
            uint32_t step_count = 0;
            uint8_t activity = 0;
            uint8_t tapStatus = 0;
            float tempC = 0;
            uint16_t irqStatus;
            
            Accelerometer::Direction previousDirection = Accelerometer::Direction::NONE;

            void bma4_error_codes_print_result(const char api_name[], uint16_t rslt);
            
    };
  }
}