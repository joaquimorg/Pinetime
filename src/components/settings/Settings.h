#pragma once

#include <cstdint>
#include "components/datetime/DateTimeController.h"
#include "components/accelerometer/Accelerometer.h"

namespace Pinetime {
  namespace Controllers {

    struct history_step_struct {
      uint32_t steps;
      uint8_t day;
      DateTime::Months month;
      uint16_t year;
    };
    class Settings {
      public:
        
        enum class ClockType {H24, H12};
        enum class Vibration {ON, OFF};

        void Init();

        void SetClockFace( uint8_t face ) { clockFace = face; };
        uint8_t GetClockFace() const { return clockFace; };

        void SetAppMenu( uint8_t menu ) { appMenu = menu; };
        uint8_t GetAppMenu() const { return appMenu; };

        void SetSettingsMenu( uint8_t menu ) { settingsMenu = menu; };
        uint8_t GetSettingsMenu() const { return settingsMenu; };

        void SetClockType( ClockType clocktype ) { clockType = clocktype; };
        ClockType GetClockType() const { return clockType; };

        void SetStepsGoal( uint32_t goal ) { stepsGoal = goal; };
        uint32_t GetStepsGoal() const { return stepsGoal; };

        void SetVibrationStatus( Vibration status ) { vibrationStatus = status; };
        Vibration GetVibrationStatus() const { return vibrationStatus; };

        void SetScreenTimeOut( uint32_t timeout ) { screenTimeOut = timeout; };
        uint32_t GetScreenTimeOut() const { return screenTimeOut; };

        void SetHistorySteps( Accelerometer steps, DateTime date );

      private:
        ClockType clockType = ClockType::H24;
        Vibration vibrationStatus = Vibration::ON;

        uint8_t clockFace = 0;
        uint8_t appMenu = 0;
        uint8_t settingsMenu = 0;

        uint32_t stepsGoal = 0;
        uint32_t screenTimeOut = 15000;

        history_step_struct stepHistory[8];
        uint8_t stepHistoryPos = 0;

    };
  }
}