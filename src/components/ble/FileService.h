#pragma once

#include <cstdint>
#include <array>

#define min // workaround: nimble's min/max macros conflict with libstdc++
#define max
#include <host/ble_gap.h>
#undef max
#undef min

//00021000-78fc-48fe-8e23-433b3a1942d0
#define FILE_SERVICE_UUID_BASE {0xd0, 0x42, 0x19, 0x3a, 0x3b, 0x43, 0x23, 0x8e, 0xfe, 0x48, 0xfc, 0x78, 0x00, 0x10, 0x02, 0x00}
//00021000-78fc-48fe-8e23-433b3a1942d0
#define FILE_SERVICE_UUID_CHAR {0xd0, 0x42, 0x19, 0x3a, 0x3b, 0x43, 0x23, 0x8e, 0xfe, 0x48, 0xfc, 0x78, 0x00, 0x10, 0x02, 0x00}
//00021001-78fc-48fe-8e23-433b3a1942d0
#define FILE_SERVICE_UUID_CHAR_CONTROL {0xd0, 0x42, 0x19, 0x3a, 0x3b, 0x43, 0x23, 0x8e, 0xfe, 0x48, 0xfc, 0x78, 0x01, 0x10, 0x02, 0x00}

namespace Pinetime {
  namespace System {
    class SystemTask;
  }
  
  namespace Drivers {
    class SpiNorFlash;
  }

  namespace Controllers {
    class Ble;

    class FileService {
      public:
        FileService(Pinetime::System::SystemTask &systemTask,
                   Pinetime::Controllers::Ble &bleController,
                   Pinetime::Drivers::SpiNorFlash &spiNorFlash);
        
        void Init();

        int OnServiceData(uint16_t connectionHandle, uint16_t attributeHandle, ble_gatt_access_ctxt *context);
        void OnTimeout();


        class SpiFlash {
          public:
            enum class FlashType : uint8_t {
                RES, FW, BOT
            };
            
            SpiFlash(Pinetime::Drivers::SpiNorFlash& spiNorFlash) : spiNorFlash{spiNorFlash} {}
            void Init(size_t chunkSize, size_t totalSize, FlashType flashType);
            void Erase();
            void Append(uint8_t* data, size_t size);
            uint16_t CalculateCrc();
            bool IsComplete();

          private:
            Pinetime::Drivers::SpiNorFlash& spiNorFlash;
            static constexpr size_t bufferSize = 240;
            bool ready = false;
            size_t chunkSize = 0;
            size_t totalSize = 0;
            
            FlashType flashType;
            size_t writeOffset = 0;
            
            // Firmware
            size_t maxSize = 475136;

            size_t bufferWriteIndex = 0;
            size_t totalWriteIndex = 0;
            uint8_t tempBuffer[bufferSize];
            
            uint16_t ComputeCrc(uint8_t const *p_data, uint32_t size, uint16_t const *p_crc);
            void WriteMagicNumber();
            
            // Don't write to flash... simulate mode for testing...
            bool demoMode = false;

        };

      private:
        static constexpr ble_uuid128_t serviceUuid{
                .u {.type = BLE_UUID_TYPE_128},
                .value = FILE_SERVICE_UUID_BASE
        };

        static constexpr ble_uuid128_t fileControlCharacteristicUuid{
                .u {.type = BLE_UUID_TYPE_128},
                .value = FILE_SERVICE_UUID_CHAR_CONTROL
        };

        static constexpr ble_uuid128_t fileDataCharacteristicUuid{
                .u {.type = BLE_UUID_TYPE_128},
                .value = FILE_SERVICE_UUID_CHAR
        };

        Pinetime::System::SystemTask &mSystemTask;
        Pinetime::Controllers::Ble &bleController;

        SpiFlash spiFlash;

        const struct ble_gatt_chr_def mCharacteristicDefinitions[3];    ///< number of characteristics (plus one null)
        const struct ble_gatt_svc_def mServiceDefinitions[2];           ///< number of services (plus one null)

        enum class Opcodes : uint8_t {
            COMMAND_FIRMWARE_UPDATE_SYNC  = 0x00,
            COMMAND_SEND_FIRMWARE_INFO    = 0x02,
            COMMAND_FIRMWARE_INIT         = 0x01,
            COMMAND_FIRMWARE_START_DATA   = 0x03,
            COMMAND_FIRMWARE_CHECKSUM     = 0x04,
            COMMAND_FIRMWARE_END_DATA     = 0x06,
            COMMAND_FIRMWARE_OK           = 0x07,
            COMMAND_FIRMWARE_ERROR        = 0x08,
            COMMAND_FIRMWARE_CHECKSUM_ERR = 0x09,
        };

        enum class States : uint8_t {
            Idle, Init, Start, Data, Validate, Validated
        };
        States state = States::Idle;

        uint16_t mtuSize = 240;
        uint32_t fileSize = 0;
        uint32_t bytesReceived = 0;
        TimerHandle_t timeoutTimer;

        uint16_t fileControlCharacteristicHandle;
        uint16_t fileDataCharacteristicHandle;

        int ControlPointHandler(uint16_t connectionHandle, os_mbuf *om);
        int WritePacketHandler(uint16_t connectionHandle, os_mbuf *om);
        void Reset();

        void NotificationSend(uint16_t connection, uint16_t charactHandle, const uint8_t *data, const size_t s);

  
    };
  }
}