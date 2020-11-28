#pragma once

#include "TwiMaster.h"

namespace Pinetime {
  namespace Drivers {
    class HRS3300 {
        public :
            HRS3300(TwiMaster& twiMaster, uint8_t twiAddress);
            HRS3300(const HRS3300&) = delete;
            HRS3300& operator=(const HRS3300&) = delete;
            HRS3300(HRS3300&&) = delete;
            HRS3300& operator=(HRS3300&&) = delete;

            void Init();

        private:

            TwiMaster& twiMaster;
            uint8_t twiAddress;
    };
  }
}