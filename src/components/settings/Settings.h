#pragma once

#include <cstdint>

namespace Pinetime {
  namespace Controllers {
    class Settings {
      public:
        void Init();

        void SetClockFace( uint8_t face );
        uint8_t GetClockFace();

        void SetAppMenu( uint8_t menu );
        uint8_t GetAppMenu();

        void SetHeartRate( uint8_t hr ) const { heartRate = hr; };
        uint8_t GetHeartRate() const { return heartRate };

      private:
        uint8_t clockFace = 0;
        uint8_t appMenu = 0;
        uint8_t heartRate = 0;
    };
  }
}