#pragma once
#include <FreeRTOS.h>
#include <date/date.h>
#include <queue.h>
#include <task.h>
#include <memory>
#include "Apps.h"
#include "LittleVgl.h"
#include "TouchEvents.h"
#include "components/brightness/BrightnessController.h"
#include "components/firmwarevalidator/FirmwareValidator.h"
#include "displayapp/screens/Modal.h"
#include "components/settings/Settings.h"

namespace Pinetime {

  namespace Drivers {
    class St7789;
    class Cst816S;
    class WatchdogView;
    class BMA421;
    class HRS3300;
  }
  
  namespace Controllers {
    class Settings;
    class Battery;
    class Ble;
    class DateTime;
    class NotificationManager;
  }

  namespace System {
    class SystemTask;
  };
  namespace Applications {
    class DisplayApp {
      public:
        enum class States {Idle, Running};
        enum class Messages : uint8_t {GoToSleep, GoToRunning, UpdateDateTime, UpdateBleConnection, UpdateBatteryLevel, TouchEvent, StepEvent, ButtonPushed,
            NewNotification, BleFirmwareUpdateStarted };

        enum class FullRefreshDirections { None, Up, Down };
        enum class TouchModes { Gestures, Polling };

        DisplayApp(Drivers::St7789 &lcd, Components::LittleVgl &lvgl, Drivers::Cst816S &touchPanel,
                   Controllers::Battery &batteryController, Controllers::Ble &bleController,
                   Controllers::DateTime &dateTimeController, Drivers::WatchdogView &watchdog,                   
                   Controllers::Settings &settingsController,
                   Drivers::BMA421& stepCounter,
                   Drivers::HRS3300& hrs,
                   System::SystemTask &systemTask,
                   Pinetime::Controllers::NotificationManager& notificationManager);
        void Start();
        void PushMessage(Messages msg);

        void StartApp(Apps app);

        void SetFullRefresh(FullRefreshDirections direction);
        void SetTouchMode(TouchModes mode);

      private:

        Pinetime::Drivers::St7789& lcd;
        Pinetime::Components::LittleVgl& lvgl;
        Pinetime::Drivers::Cst816S& touchPanel;
        Pinetime::Controllers::Battery &batteryController;
        Pinetime::Controllers::Ble &bleController;
        Pinetime::Controllers::DateTime& dateTimeController;
        Pinetime::Drivers::WatchdogView& watchdog;
        Pinetime::Controllers::Settings& settingsController;
        Pinetime::Drivers::BMA421& stepCounter;
        Pinetime::Drivers::HRS3300& hrs;
        Pinetime::System::SystemTask& systemTask;
        Pinetime::Controllers::NotificationManager& notificationManager;
        std::unique_ptr<Screens::Screen> currentScreen;

        Pinetime::Controllers::FirmwareValidator validator;
        Controllers::BrightnessController brightnessController;


        TaskHandle_t taskHandle;
        States state = States::Running;
        QueueHandle_t msgQueue;
        static constexpr uint8_t queueSize = 10;
        static constexpr uint8_t itemSize = 1;
        
        Apps nextApp = Apps::None;
        bool onClockApp = false; // TODO find a better way to know that we should handle gestures and button differently for the Clock app.
        TouchModes touchMode = TouchModes::Gestures;

        static void Process(void* instance);
        void InitHw();
        void Refresh();
        void RunningState();
        void IdleState();
        TouchEvents OnTouchEvent();        
                
    };
  }
}


