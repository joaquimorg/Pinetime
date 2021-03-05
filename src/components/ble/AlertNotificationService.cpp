#include "AlertNotificationService.h"
#include <hal/nrf_rtc.h>
#include <cstring>
#include <algorithm>
#include "NotificationManager.h"
#include "CallNotificationManager.h"
#include "systemtask/SystemTask.h"

using namespace Pinetime::Controllers;

constexpr ble_uuid16_t AlertNotificationService::ansUuid;
constexpr ble_uuid16_t AlertNotificationService::ansCharUuid;
//constexpr ble_uuid16_t AlertNotificationService::ansEventUuid;
constexpr ble_uuid128_t AlertNotificationService::notificationEventUuid;


int AlertNotificationCallback(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg) {
  auto anService = static_cast<AlertNotificationService*>(arg);
  return anService->OnAlert(conn_handle, attr_handle, ctxt);
}

void AlertNotificationService::Init() {
  int res;
  res = ble_gatts_count_cfg(serviceDefinition);
  ASSERT(res == 0);

  res = ble_gatts_add_svcs(serviceDefinition);
  ASSERT(res == 0);
}

AlertNotificationService::AlertNotificationService ( 
    System::SystemTask& systemTask, 
    NotificationManager& notificationManager,
    CallNotificationManager& callNotificationManager )
  : characteristicDefinition{
            {
                    .uuid = (ble_uuid_t *) &ansCharUuid,
                    .access_cb = AlertNotificationCallback,
                    .arg = this,
                    .flags = BLE_GATT_CHR_F_WRITE
            },
            /*{
                    .uuid = (ble_uuid_t *) &ansEventUuid,
                    .access_cb = AlertNotificationCallback,
                    .arg = this,
                    .flags = BLE_GATT_CHR_F_NOTIFY,
                    .val_handle = &eventHandle
            },*/
            {
                    .uuid = (ble_uuid_t *) &notificationEventUuid,
                    .access_cb = AlertNotificationCallback,
                    .arg = this,
                    .flags = BLE_GATT_CHR_F_NOTIFY,
                    .val_handle = &eventHandle
            },
            {
              0
            }
    },
    serviceDefinition{
            {
                    /* Device Information Service */
                    .type = BLE_GATT_SVC_TYPE_PRIMARY,
                    .uuid = (ble_uuid_t *) &ansUuid,
                    .characteristics = characteristicDefinition
            },
            {
                    0
            },
    }, 
    systemTask{systemTask}, 
    notificationManager{notificationManager},
    callNotificationManager{callNotificationManager} {
}

int AlertNotificationService::OnAlert(uint16_t conn_handle, uint16_t attr_handle,
                                                    struct ble_gatt_access_ctxt *ctxt) {
  if (ctxt->op == BLE_GATT_ACCESS_OP_WRITE_CHR) {

    constexpr size_t headerSize = 9;

    uint8_t* notification = new uint8_t[headerSize];
    
    os_mbuf_copydata(ctxt->om, 0, headerSize, notification);

    uint8_t subject_length = notification[7];
    uint8_t body_length = notification[8];    
    uint8_t category = notification[5];

    Pinetime::System::SystemTask::Messages event = Pinetime::System::SystemTask::Messages::OnNewNotification;

    if ( category == NOTIFICATION_INCOME_CALL || category == NOTIFICATION_CALL_OFF ) {
      
      //notifCall.category = Pinetime::Controllers::CallNotificationManager::Categories::Unknown;

      event = Pinetime::System::SystemTask::Messages::OnNewCall;
      
      if ( category == NOTIFICATION_INCOME_CALL ) {

        CallNotificationManager::Notification notifCall;

        // number
        os_mbuf_copydata(ctxt->om, headerSize, subject_length + 1, notifCall.number.data());
        notifCall.number[subject_length] = '\0';

        // callername
        os_mbuf_copydata(ctxt->om, headerSize + subject_length, body_length + 1, notifCall.callername.data());
        notifCall.callername[body_length] = '\0';

        notifCall.category = Pinetime::Controllers::CallNotificationManager::Categories::IncomingCall;

        callNotificationManager.Set(notifCall);

      } else if ( category == NOTIFICATION_CALL_OFF ) {

        //notifCall.category = Pinetime::Controllers::CallNotificationManager::Categories::EndCall;

        callNotificationManager.TerminateCall();

      } 

      
      //notif.category = Pinetime::Controllers::NotificationManager::Categories::IncomingCall;
      //notificationManager.Push(std::move(notif));

    } else {

      NotificationManager::Notification notif;

      notif.alertID = notification[1];    

      // Subject
      os_mbuf_copydata(ctxt->om, headerSize, subject_length + 1, notif.subject.data());
      notif.subject[subject_length] = '\0';

      // Body
      os_mbuf_copydata(ctxt->om, headerSize + subject_length, body_length + 1, notif.message.data());
      notif.message[body_length] = '\0';

      // Date Time
      os_mbuf_copydata(ctxt->om, headerSize + subject_length + body_length, 5, notif.year.data());
      notif.year[4] = '\0';
      os_mbuf_copydata(ctxt->om, headerSize + subject_length + body_length + 4, 3, notif.month.data());
      notif.month[2] = '\0';
      os_mbuf_copydata(ctxt->om, headerSize + subject_length + body_length + 6, 3, notif.day.data());
      notif.day[2] = '\0';
      os_mbuf_copydata(ctxt->om, headerSize + subject_length + body_length + 9, 3, notif.hour.data());
      notif.hour[2] = '\0';
      os_mbuf_copydata(ctxt->om, headerSize + subject_length + body_length + 11, 3, notif.minute.data());
      notif.minute[2] = '\0';
      os_mbuf_copydata(ctxt->om, headerSize + subject_length + body_length + 13, 3, notif.second.data());
      notif.second[2] = '\0';


      switch(category) {
        
        case NOTIFICATION_EMAIL:
          notif.category = Pinetime::Controllers::NotificationManager::Categories::Email;
          break;
        case NOTIFICATION_MISSED_CALL:
          notif.category = Pinetime::Controllers::NotificationManager::Categories::MissedCall;
          break;
        case NOTIFICATION_SMS:
          notif.category = Pinetime::Controllers::NotificationManager::Categories::Sms;
          break;
        case NOTIFICATION_CALENDAR:
          notif.category = Pinetime::Controllers::NotificationManager::Categories::Schedule;
          break;
        case NOTIFICATION_WECHAT:
        case NOTIFICATION_VIBER:
        case NOTIFICATION_SNAPCHAT:
        case NOTIFICATION_WHATSAPP:
        case NOTIFICATION_FACEBOOK:
        case NOTIFICATION_MESSENGER:
        case NOTIFICATION_INSTAGRAM:
        case NOTIFICATION_TWITTER:
        case NOTIFICATION_LINKEDIN:
        case NOTIFICATION_LINE:
        case NOTIFICATION_SKYPE:
          notif.category = Pinetime::Controllers::NotificationManager::Categories::InstantMessage;
          break;
        default:
          notif.category = Pinetime::Controllers::NotificationManager::Categories::Unknown;
          break;
      }
      
      notificationManager.Push(std::move(notif));
    }
    systemTask.PushMessage(event);
  }
  return 0;
}

void AlertNotificationService::event(char event) {
  auto *om = ble_hs_mbuf_from_flat(&event, 1);

  uint16_t connectionHandle = systemTask.nimble().connHandle();

  if (connectionHandle == 0 || connectionHandle == BLE_HS_CONN_HANDLE_NONE) {
    return;
  }

  ble_gattc_notify_custom(connectionHandle, eventHandle, om);
}

void AlertNotificationService::AcceptIncomingCall() {
  auto response = IncomingCallResponses::Answer;
  auto *om = ble_hs_mbuf_from_flat(&response, 1);

  uint16_t connectionHandle = systemTask.nimble().connHandle();

  if (connectionHandle == 0 || connectionHandle == BLE_HS_CONN_HANDLE_NONE) {
    return;
  }

  ble_gattc_notify_custom(connectionHandle, eventHandle, om);
}

void AlertNotificationService::RejectIncomingCall() {
  auto response = IncomingCallResponses::Reject;
  auto *om = ble_hs_mbuf_from_flat(&response, 1);

  uint16_t connectionHandle = systemTask.nimble().connHandle();

  if (connectionHandle == 0 || connectionHandle == BLE_HS_CONN_HANDLE_NONE) {
    return;
  }

  ble_gattc_notify_custom(connectionHandle, eventHandle, om);
}

void AlertNotificationService::MuteIncomingCall() {
  auto response = IncomingCallResponses::Mute;
  auto *om = ble_hs_mbuf_from_flat(&response, 1);

  uint16_t connectionHandle = systemTask.nimble().connHandle();

  if (connectionHandle == 0 || connectionHandle == BLE_HS_CONN_HANDLE_NONE) {
    return;
  }

  ble_gattc_notify_custom(connectionHandle, eventHandle, om);
} 