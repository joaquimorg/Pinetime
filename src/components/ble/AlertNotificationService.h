#pragma once
#include <cstdint>
#include <array>
#define min // workaround: nimble's min/max macros conflict with libstdc++
#define max
#include <host/ble_gap.h>
#undef max
#undef min


namespace Pinetime {

  namespace System {
    class SystemTask;
  }
  namespace Controllers {
    class NotificationManager;

    class AlertNotificationService {
      public:
        AlertNotificationService(Pinetime::System::SystemTask &systemTask,
                                         Pinetime::Controllers::NotificationManager &notificationManager);
        void Init();

        int OnAlert(uint16_t conn_handle, uint16_t attr_handle,
                                    struct ble_gatt_access_ctxt *ctxt);

        void event(char event);

        static const uint8_t EVENT_HANG_UP_CALL = 0x00;
        static const uint8_t EVENT_ANSWER_CALL = 0x01;


      private:
        static const uint8_t ALERT_UNKNOWN = 0x01;
        static const uint8_t ALERT_SIMPLE_ALERT = 0x02;
        static const uint8_t ALERT_EMAIL = 0x03;
        static const uint8_t ALERT_NEWS = 0x04;
        static const uint8_t ALERT_INCOMING_CALL = 0x05;
        static const uint8_t ALERT_MISSED_CALL = 0x06;
        static const uint8_t ALERT_SMS = 0x07;
        static const uint8_t ALERT_VOICE_MAIL = 0x08;
        static const uint8_t ALERT_SCHEDULE = 0x09;
        static const uint8_t ALERT_HIGH_PRIORITY_ALERT = 0x0a;
        static const uint8_t ALERT_INSTANT_MESSAGE = 0x0b;

        static constexpr uint16_t ansId {0x1811};
        static constexpr uint16_t ansCharId {0x2a46};
        static constexpr uint16_t ansEventCharId = {0x2a47};

        static constexpr ble_uuid16_t ansUuid {
                .u { .type = BLE_UUID_TYPE_16 },
                .value = ansId
        };

        static constexpr ble_uuid16_t ansCharUuid {
                .u { .type = BLE_UUID_TYPE_16 },
                .value = ansCharId
        };

        static constexpr ble_uuid16_t ansEventUuid {
                .u { .type = BLE_UUID_TYPE_16 },
                .value = ansEventCharId
        };

        struct ble_gatt_chr_def characteristicDefinition[3];
        struct ble_gatt_svc_def serviceDefinition[2];

        Pinetime::System::SystemTask &systemTask;
        NotificationManager &notificationManager;

        uint16_t eventHandle;
    };
  }
}
