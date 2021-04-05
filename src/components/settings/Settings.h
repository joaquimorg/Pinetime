#pragma once

#include <cstdint>
#include "components/datetime/DateTimeController.h"
#include "components/accelerometer/Accelerometer.h"
#include "components/brightness/BrightnessController.h"
#include "drivers/SpiNorFlash.h"

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
        enum class WakeUpMode {None, SingleTap, DoubleTap, RaiseWrist};

        Settings( Pinetime::Drivers::SpiNorFlash &spiNorFlash );

        void Init();

        void SaveSettings();

        void SetClockFace( uint8_t face ) {
          if ( face != settings.clockFace ) settingsChanged = true;
          settings.clockFace = face; 
        };
        uint8_t GetClockFace() const { return settings.clockFace; };

        void SetAppMenu( uint8_t menu ) { appMenu = menu; };
        uint8_t GetAppMenu() const { return appMenu; };

        void SetSettingsMenu( uint8_t menu ) { settingsMenu = menu; };
        uint8_t GetSettingsMenu() const { return settingsMenu; };

        void SetClockType( ClockType clocktype ) { 
          if ( clocktype != settings.clockType ) settingsChanged = true;
          settings.clockType = clocktype; 
        };
        ClockType GetClockType() const { return settings.clockType; };

        void SetStepsGoal( uint32_t goal ) { 
          if ( goal != settings.stepsGoal ) settingsChanged = true;
          settings.stepsGoal = goal; 
        };
        uint32_t GetStepsGoal() const { return settings.stepsGoal; };

        void SetVibrationStatus( Vibration status ) { 
          if ( status != settings.vibrationStatus ) settingsChanged = true;
          settings.vibrationStatus = status; 
        };
        Vibration GetVibrationStatus() const { return settings.vibrationStatus; };

        void SetScreenTimeOut( uint32_t timeout ) { 
          if ( timeout != settings.screenTimeOut ) settingsChanged = true;
          settings.screenTimeOut = timeout; 
        };
        uint32_t GetScreenTimeOut() const { return settings.screenTimeOut; };

        void SetHistorySteps( Accelerometer steps, DateTime date );

        void setWakeUpMode( WakeUpMode wakeUp ) { 
          if ( wakeUp != settings.wakeUpMode ) settingsChanged = true;
          settings.wakeUpMode = wakeUp; 
        };
        WakeUpMode getWakeUpMode() const { return settings.wakeUpMode; };

        void SetBrightness( Controllers::BrightnessController::Levels level ) { 
          if ( level != settings.brightLevel ) settingsChanged = true;
          settings.brightLevel = level; 
        };
        Controllers::BrightnessController::Levels GetBrightness() const { return settings.brightLevel; };

        uint8_t getSettingsBlock() const { return settingsFlashBlock; };
      private:

        Pinetime::Drivers::SpiNorFlash& spiNorFlash;

        struct SettingsData {

          ClockType clockType = ClockType::H24;
          Vibration vibrationStatus = Vibration::ON;

          uint8_t clockFace = 0;

          uint32_t stepsGoal = 1000;
          uint32_t screenTimeOut = 15000;

          WakeUpMode wakeUpMode = WakeUpMode::None;

          Controllers::BrightnessController::Levels brightLevel = Controllers::BrightnessController::Levels::Medium;

        };

        history_step_struct stepHistory[8];
        uint8_t stepHistoryPos = 0;

        uint8_t appMenu = 0;
        uint8_t settingsMenu = 0;

        SettingsData settings;

        // There are 10 blocks of reserved flash to save settings
        // to minimize wear, the recording is done in a rotating way by the 10 blocks
        uint8_t settingsFlashBlock = 99; // default to indicate it needs to find the active block

        bool settingsChanged = false;

        bool FindHeader();
        void ReadSettingsData();
        void EraseBlock();
        void SetHeader( bool state );
        void SaveSettingsData();
        void LoadSettingsFromFlash();
        void SaveSettingsToFlash();

    };
  }
}