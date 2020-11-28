#pragma once

#include "TwiMaster.h"
#include "bma421/bma4.h"
#include "bma421/bma421.h"

namespace Pinetime {
  namespace Drivers {

    class BMA421 {
        public :
            
            BMA421(TwiMaster& twiMaster);
            BMA421(const BMA421&) = delete;
            BMA421& operator=(const BMA421&) = delete;
            BMA421(BMA421&&) = delete;
            BMA421& operator=(BMA421&&) = delete;

            void Init();
            void Update();
            uint32_t GetSteps() const { return step_count; };

        private:

            TwiMaster& twiMaster;
            uint8_t twiAddress;

            uint8_t deviceReady = 0;

            struct bma4_dev bma;
            struct bma4_accel_config accel_conf;

            uint32_t step_count = 0;
            
    };
  }
}