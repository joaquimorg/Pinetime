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

        void SetHeartRate( uint8_t hr ) { heartRate = hr; };
        uint8_t GetHeartRate() const { return heartRate; };
        uint32_t GetStepsGoal() const { return stepsGoal; };

      private:
        uint8_t clockFace = 0;
        uint8_t appMenu = 0;
        uint8_t heartRate = 0;
        uint32_t stepsGoal = 0;
    };
  }
}