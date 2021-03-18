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
  namespace Controllers {
    class Ble;
    class FS;


    class FileService {
      public:
        FileService(Pinetime::System::SystemTask &systemTask,
                   Pinetime::Controllers::Ble &bleController,
                   Pinetime::Controllers::FS &fs);
        
        void Init();

        int OnServiceData(uint16_t connectionHandle, uint16_t attributeHandle, ble_gatt_access_ctxt *context);

      private:
        static constexpr ble_uuid128_t serviceUuid{
                .u {.type = BLE_UUID_TYPE_128},
                .value = FILE_SERVICE_UUID_BASE
        };

        static constexpr ble_uuid128_t fileControlCharacteristicUuid{
                .u {.type = BLE_UUID_TYPE_128},
                .value = FILE_SERVICE_UUID_CHAR_CONTROL
        };

        static constexpr ble_uuid128_t fileCharacteristicUuid{
                .u {.type = BLE_UUID_TYPE_128},
                .value = FILE_SERVICE_UUID_CHAR
        };

        Pinetime::System::SystemTask &mSystemTask;
        Pinetime::Controllers::Ble &mBleController;
        Pinetime::Controllers::FS &fs;

        const struct ble_gatt_chr_def mCharacteristicDefinitions[3];    ///< number of characteristics (plus one null)
        const struct ble_gatt_svc_def mServiceDefinitions[2];           ///< number of services (plus one null)

        enum class Opcodes : uint8_t {
            START = 0xA1,
            END   = 0xA9,
        };

        void closeActive();
        int handleWrite(ble_gatt_access_ctxt*);
        int handleRead(ble_gatt_access_ctxt*);

  
    };
  }
}