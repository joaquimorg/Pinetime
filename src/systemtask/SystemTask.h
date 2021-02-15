#pragma once

#include <memory>

#include <FreeRTOS.h>
#include <task.h>
#include <timers.h>

#include "SystemMonitor.h"
#include "components/settings/Settings.h"
#include "components/battery/BatteryController.h"
#include "components/ble/NimbleController.h"
#include "components/ble/NotificationManager.h"
#include "displayapp/DisplayApp.h"
#include "drivers/Watchdog.h"
#include "drivers/Vibration.h"

namespace Pinetime {
  namespace Drivers {
    class BMA421;
    class Cst816S;
    class SpiMaster;
    class SpiNorFlash;
    class St7789;
    class TwiMaster;
    class Vibration;
  }
  namespace System {
    class SystemTask {
      public:
        enum class Messages {WakeUp, GoToSleep, OnNewNotification, OnNewCall, BleConnected,
            BleFirmwareUpdateStarted, BleFirmwareUpdateFinished, OnTouchEvent, OnStepEvent, OnButtonEvent, OnDisplayTaskSleeping,
            ReloadIdleTimer, EnableSleeping, DisableSleeping
        };

        SystemTask(Drivers::SpiMaster &spi, Drivers::St7789 &lcd,
                   Pinetime::Drivers::SpiNorFlash& spiNorFlash,                   
                   Drivers::TwiMaster& twiMaster, Drivers::Cst816S &touchPanel, Drivers::BMA421& stepCounter,
                   Components::LittleVgl &lvgl,
                   Controllers::Battery &batteryController, Controllers::Ble &bleController,
                   Controllers::DateTime &dateTimeController,
                   Controllers::Settings &settingsController);


        void Start();
        void PushMessage(Messages msg);

        void OnButtonPushed();
        void OnTouchEvent();
        void OnStepEvent();
        void OnChargingEvent();
        void OnPowerPresentEvent();

        void OnIdle();
        void HardwareStatus();

        Pinetime::Controllers::NimbleController& nimble() {return nimbleController;};

      private:
        TaskHandle_t taskHandle;
        Pinetime::Drivers::Vibration vibration;

        Pinetime::Drivers::SpiMaster& spi;
        Pinetime::Drivers::St7789& lcd;
        Pinetime::Drivers::SpiNorFlash& spiNorFlash;
        Pinetime::Drivers::TwiMaster& twiMaster;
        Pinetime::Drivers::Cst816S& touchPanel;
        Pinetime::Drivers::BMA421& stepCounter;
        Pinetime::Components::LittleVgl& lvgl;
        Pinetime::Controllers::Battery& batteryController;
        std::unique_ptr<Pinetime::Applications::DisplayApp> displayApp;
        Pinetime::Controllers::Ble& bleController;
        Pinetime::Controllers::DateTime& dateTimeController;
        QueueHandle_t systemTasksMsgQueue;
        std::atomic<bool> isSleeping{false};
        std::atomic<bool> isGoingToSleep{false};
        std::atomic<bool> isWakingUp{false};
        Pinetime::Controllers::Settings& settingsController;
        Pinetime::Drivers::Watchdog watchdog;
        Pinetime::Drivers::WatchdogView watchdogView;
        Pinetime::Controllers::NotificationManager notificationManager;
        Pinetime::Controllers::NimbleController nimbleController;

        static void Process(void* instance);
        void Work();
        void ReloadIdleTimer() const;
        bool isBleDiscoveryTimerRunning = false;
        uint8_t bleDiscoveryTimer = 0;
        static constexpr uint32_t idleTime = 15000;

        static constexpr uint32_t hardwareTime = 15000;
        static constexpr uint32_t hardwareIdleTime = 120000;

        TimerHandle_t idleTimer;
        TimerHandle_t hardwareTimer;
        bool doNotGoToSleep = false;

        void WakeUp();

#if configUSE_TRACE_FACILITY == 1
        SystemMonitor<FreeRtosMonitor> monitor;
#else
        SystemMonitor<DummyMonitor> monitor;
#endif
    };
  }
}
