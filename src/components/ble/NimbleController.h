#pragma once

#include <cstdint>
#include <nrf_log.h>
#define min // workaround: nimble's min/max macros conflict with libstdc++
#define max
#include <host/ble_gap.h>
#undef max
#undef min
#include "AlertNotificationClient.h"
#include "AlertNotificationService.h"
#include "BatteryInformationService.h"
#include "CurrentTimeClient.h"
#include "CurrentTimeService.h"
#include "DeviceInformationService.h"
#include "DfuService.h"
#include "FileService.h"
#include "ImmediateAlertService.h"
#include "MusicService.h"
#include "ServiceDiscovery.h"

namespace Pinetime {
  namespace Drivers {
    class SpiNorFlash;
  }

  namespace System {
    class SystemTask;
  }

  namespace Controllers {
    class Ble;
    class DateTime;
    class NotificationManager;
    class CallNotificationManager;
    class FS;

    class NimbleController {

      public:
        NimbleController(Pinetime::System::SystemTask& systemTask, Pinetime::Controllers::Ble& bleController,
                DateTime& dateTimeController, 
                Pinetime::Controllers::NotificationManager& notificationManager,
                Pinetime::Controllers::CallNotificationManager &callNotificationManager,
                Controllers::Battery& batteryController, Pinetime::Drivers::SpiNorFlash& spiNorFlash, Pinetime::Controllers::FS& fs);
        void Init();
        void StartAdvertising();
        int OnGAPEvent(ble_gap_event *event);

        int OnDiscoveryEvent(uint16_t i, const ble_gatt_error *pError, const ble_gatt_svc *pSvc);
        int OnCTSCharacteristicDiscoveryEvent(uint16_t connectionHandle, const ble_gatt_error *error,
                                              const ble_gatt_chr *characteristic);
        int OnANSCharacteristicDiscoveryEvent(uint16_t connectionHandle, const ble_gatt_error *error,
                                              const ble_gatt_chr *characteristic);
        int OnCurrentTimeReadResult(uint16_t connectionHandle, const ble_gatt_error *error, ble_gatt_attr *attribute);
        int OnANSDescriptorDiscoveryEventCallback(uint16_t connectionHandle, const ble_gatt_error *error,
                                                  uint16_t characteristicValueHandle, const ble_gatt_dsc *descriptor);

        void StartDiscovery();

        //Pinetime::Controllers::MusicService& music() {return musicService;};
        Pinetime::Controllers::AlertNotificationService& alertService() {return alertNotificationService;};

        uint16_t connHandle();

      private:
        static constexpr const char* deviceName = "Pinetime-Lite";
        Pinetime::System::SystemTask& systemTask;
        Pinetime::Controllers::Ble& bleController;
        DateTime& dateTimeController;
        Pinetime::Controllers::NotificationManager& notificationManager;
        Pinetime::Controllers::CallNotificationManager& callNotificationManager;
        Pinetime::Drivers::SpiNorFlash& spiNorFlash;
        Pinetime::Controllers::FS& fs;

        FileService fileService;
        Pinetime::Controllers::DfuService dfuService;

        DeviceInformationService deviceInformationService;
        CurrentTimeClient currentTimeClient;
        AlertNotificationService alertNotificationService;
        AlertNotificationClient alertNotificationClient;
        CurrentTimeService currentTimeService;
        //MusicService musicService;
        BatteryInformationService batteryInformationService;
        ImmediateAlertService immediateAlertService;


        uint8_t addrType; // 1 = Random, 0 = PUBLIC
        uint16_t connectionHandle = 0;

        ble_uuid128_t dfuServiceUuid {
                .u { .type = BLE_UUID_TYPE_128},
                .value = {0x23, 0xD1, 0xBC, 0xEA, 0x5F, 0x78, 0x23, 0x15, 0xDE, 0xEF, 0x12, 0x12, 0x30, 0x15, 0x00, 0xFF}
        };

        ServiceDiscovery serviceDiscovery;
    };
  }
}
