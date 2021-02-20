#pragma once
#include <cstdint>
#include <drivers/include/nrfx_saadc.h>
#include <array>
#include <numeric>

namespace Pinetime {
  namespace Controllers {
    
    class Battery {
      public:
        void Init();
        void Update();
        int8_t PercentRemaining();
        float Voltage();// const { return voltage; }
        bool IsCharging() const { return isCharging; }
        //bool IsPowerPresent() const { return isPowerPresent; }

      private:
        static void SaadcEventHandler(nrfx_saadc_evt_t const * p_event);
        
        bool isCharging = false;
        //bool isPowerPresent = false;

        //float voltage = 0.0f;
        //int8_t percentRemaining = -1;

        
    };
  }
}