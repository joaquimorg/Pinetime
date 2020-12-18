#pragma once

#include "TwiMaster.h"
#include "bma421/bma4.h"
#include "bma421/bma421.h"

namespace Pinetime {
  namespace Drivers {

    struct accl_data_struct {
      int16_t x;
      int16_t y;
      int16_t z;      
    };

    class BMA421 {
        public :
            
            BMA421(TwiMaster& twiMaster);
            BMA421(const BMA421&) = delete;
            BMA421& operator=(const BMA421&) = delete;
            BMA421(BMA421&&) = delete;
            BMA421& operator=(BMA421&&) = delete;

            char* status;

            void Init();
            void Update();

            void Wakeup();
            void Sleep();

            uint32_t GetSteps() const { return step_count; };
            uint8_t GetActivity() const { return activity; };
            uint8_t GetTapStatus() const { return tapStatus; };
            float GetTemp() const { return tempC; };
            accl_data_struct GetAccel() const { return accelData; };

        private:

            TwiMaster& twiMaster;
            uint8_t twiAddress;

            uint8_t deviceReady = 0;

            struct bma4_dev bma;
            struct bma4_accel_config accel_conf;

            struct accl_data_struct accelData;
            uint32_t step_count = 0;
            uint8_t activity = 0;
            uint8_t tapStatus = 0;
            float tempC = 0;            

            void bma4_error_codes_print_result(const char api_name[], uint16_t rslt);
            
    };
  }
}