#pragma once
#include <cstdint>
#include <array>
#include <nrf_log.h>
#define min // workaround: nimble's min/max macros conflict with libstdc++
#define max
#include <host/ble_gap.h>
#undef max
#undef min

//00020001-78fc-48fe-8e23-433b3a1942d0
#define NOTIFICATION_EVENT_SERVICE_UUID_BASE {0xd0, 0x42, 0x19, 0x3a, 0x3b, 0x43, 0x23, 0x8e, 0xfe, 0x48, 0xfc, 0x78, 0x01, 0x00, 0x02, 0x00}

//00020000-78fc-48fe-8e23-433b3a1942d0
#define MSG_EVENT_SERVICE_UUID_BASE {0xd0, 0x42, 0x19, 0x3a, 0x3b, 0x43, 0x23, 0x8e, 0xfe, 0x48, 0xfc, 0x78, 0x00, 0x00, 0x02, 0x00}

//00020002-78fc-48fe-8e23-433b3a1942d0
#define MSG_EVENT_SERVICE_UUID_CHAR {0xd0, 0x42, 0x19, 0x3a, 0x3b, 0x43, 0x23, 0x8e, 0xfe, 0x48, 0xfc, 0x78, 0x02, 0x00, 0x02, 0x00}

namespace Pinetime {

  namespace System {
    class SystemTask;
  }
  namespace Controllers {
    class NotificationManager;
    class CallNotificationManager;

    class AlertNotificationService {
      public:
        AlertNotificationService(Pinetime::System::SystemTask &systemTask,
                                 Pinetime::Controllers::NotificationManager &notificationManager,
                                 Pinetime::Controllers::CallNotificationManager &callNotificationManager);
        void Init();

        int OnAlert(uint16_t conn_handle, uint16_t attr_handle,
                                    struct ble_gatt_access_ctxt *ctxt);

        void AcceptIncomingCall();
        void RejectIncomingCall();
        void MuteIncomingCall();

        enum class IncomingCallResponses : uint8_t {
          Reject = 0x00,
          Answer = 0x01,
          Mute = 0x02
        };

        void event(char event);


      private:

        // from PineTimeJFConstants in Gadgetbridge

        static const uint8_t NOTIFICATION_MISSED_CALL = 0x00;
        static const uint8_t NOTIFICATION_SMS         = 0x01;
        static const uint8_t NOTIFICATION_SOCIAL      = 0x02;
        static const uint8_t NOTIFICATION_EMAIL       = 0x03;
        static const uint8_t NOTIFICATION_CALENDAR    = 0x04;
        static const uint8_t NOTIFICATION_INCOME_CALL = 0x05;
        static const uint8_t NOTIFICATION_CALL_OFF    = 0x06;
        static const uint8_t NOTIFICATION_WECHAT      = 0x07;
        static const uint8_t NOTIFICATION_VIBER       = 0x08;
        static const uint8_t NOTIFICATION_SNAPCHAT    = 0x09;
        static const uint8_t NOTIFICATION_WHATSAPP    = 0x0A;
        static const uint8_t NOTIFICATION_FACEBOOK    = 0x0C;
        static const uint8_t NOTIFICATION_MESSENGER   = 0x0F;
        static const uint8_t NOTIFICATION_INSTAGRAM   = 0x10;
        static const uint8_t NOTIFICATION_TWITTER     = 0x11;
        static const uint8_t NOTIFICATION_LINKEDIN    = 0x12;
        static const uint8_t NOTIFICATION_LINE        = 0x14;
        static const uint8_t NOTIFICATION_SKYPE       = 0x15;


        static constexpr ble_uuid128_t ansUuid {
                .u { .type = BLE_UUID_TYPE_128 },
                .value = MSG_EVENT_SERVICE_UUID_BASE
        };

        static constexpr ble_uuid128_t ansCharUuid {
                .u { .type = BLE_UUID_TYPE_128 },
                .value = MSG_EVENT_SERVICE_UUID_CHAR
        };

        static constexpr ble_uuid128_t notificationEventUuid {
                .u { .type = BLE_UUID_TYPE_128 },
                .value = NOTIFICATION_EVENT_SERVICE_UUID_BASE
        };

        struct ble_gatt_chr_def characteristicDefinition[3];
        struct ble_gatt_svc_def serviceDefinition[2];

        Pinetime::System::SystemTask &systemTask;
        NotificationManager &notificationManager;
        CallNotificationManager &callNotificationManager;

        uint16_t eventHandle;
    };
  }
}
