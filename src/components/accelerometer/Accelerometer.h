#pragma once

#include "drivers/TwiMaster.h"
#include "bma421/bma4.h"
#include "bma421/bma421.h"

struct accl_data_struct {
  int16_t x;
  int16_t y;
  int16_t z;
};

namespace Pinetime {
  namespace Controllers {
    
    class Accelerometer {
        public :
            
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
            accl_data_struct GetAccel() const { return accelData; };

            void ReadIRQStatus();
            bool isStepIRQ();

            bool isReady() const { return deviceReady; };

        private:

            Pinetime::Drivers::TwiMaster& twiMaster;
            uint8_t twiAddress;

            bool deviceReady = false;

            struct bma4_dev bma;
            struct bma4_accel_config accel_conf;
            struct accl_data_struct accelData;
            uint32_t step_count = 0;
            uint8_t activity = 0;
            uint8_t tapStatus = 0;
            float tempC = 0;
            uint16_t irqStatus;

            void bma4_error_codes_print_result(const char api_name[], uint16_t rslt);
            
    };
  }
}