#include "FileService.h"

#include <../components/fs/FS.h>
#include "systemtask/SystemTask.h"
#include <string>

using namespace Pinetime::Controllers;

// provide linktime space for constants
constexpr ble_uuid128_t FileService::serviceUuid;
constexpr ble_uuid128_t FileService::fileCharacteristicUuid;
constexpr ble_uuid128_t FileService::fileControlCharacteristicUuid;


namespace {
  int FileServiceCallback(uint16_t conn_handle, uint16_t attr_handle,
                        struct ble_gatt_access_ctxt *ctxt, void *arg) {
    auto fileService = static_cast<FileService *>(arg);
    return fileService->OnServiceData(conn_handle, attr_handle, ctxt);
  }

  void TimeoutTimerCallback(TimerHandle_t xTimer) {
      auto fileService = static_cast<FileService *>(pvTimerGetTimerID(xTimer));
      fileService->OnTimeout();
  }
}

FileService::FileService(Pinetime::System::SystemTask &systemTask,
                       Pinetime::Controllers::Ble &bleController,
                       Pinetime::Controllers::FS &fs)

: mSystemTask{systemTask}, bleController{bleController}, fs{fs},

  mCharacteristicDefinitions{
      { .uuid = reinterpret_cast<const ble_uuid_t*>(&fileCharacteristicUuid),
        .access_cb = FileServiceCallback, 
        .arg = this,
        .flags = (BLE_GATT_CHR_F_WRITE_NO_RSP | BLE_GATT_CHR_F_READ), 
        .val_handle = nullptr },

      { .uuid = reinterpret_cast<const ble_uuid_t*>(&fileControlCharacteristicUuid),
        .access_cb = FileServiceCallback, 
        .arg = this,
        .flags = (BLE_GATT_CHR_F_WRITE | BLE_GATT_CHR_F_NOTIFY), 
        .val_handle = &eventHandle },

      { 0 /* null terminator */ }
  },

  mServiceDefinitions{
      { .type = BLE_GATT_SVC_TYPE_PRIMARY,
        .uuid = reinterpret_cast<const ble_uuid_t*>(&serviceUuid),
        .characteristics = mCharacteristicDefinitions
      },

      { 0 /* null terminator */ }
  } {
  timeoutTimer = xTimerCreate ("notificationTimer", 10000, pdFALSE, this, TimeoutTimerCallback);
}

void FileService::Init() {
  int res;
  res = ble_gatts_count_cfg(mServiceDefinitions);
  ASSERT(res == 0);

  res = ble_gatts_add_svcs(mServiceDefinitions);
  ASSERT(res == 0);

  bleController.FWType(Pinetime::Controllers::Ble::FirmwareType::RES);
}

int FileService::OnServiceData(uint16_t connectionHandle, uint16_t attributeHandle, ble_gatt_access_ctxt *context) {
  

  { uint16_t fileControlCharacteristicHandle;
    ble_gatts_find_chr(reinterpret_cast<const ble_uuid_t*>(&serviceUuid),
                       reinterpret_cast<const ble_uuid_t*>(&fileControlCharacteristicUuid),
                       nullptr /* don't get defintion handle */,
                       &fileControlCharacteristicHandle);
    if(attributeHandle == fileControlCharacteristicHandle) {

      const auto opcode = static_cast<Opcodes>(context->om->om_data[0]);

      switch(opcode) {
        case Opcodes::COMMAND_SEND_FIRMWARE_INFO: {
          
          uint8_t data[2];

          // Validation of size and erase flash
          // ...

          fileSize = context->om->om_data[1] + (context->om->om_data[2] << 8) + (context->om->om_data[3] << 16);

          if (fileSize == 0) {
            data[0] = 0x00;
          } else {
          
            data[0] = 0x01;
            bleController.StartFirmwareUpdate();
            bleController.State(Pinetime::Controllers::Ble::FirmwareUpdateStates::Running);
            bleController.FirmwareUpdateTotalBytes(fileSize);
            bleController.FirmwareUpdateCurrentBytes(0);
            // Send task to open app
            mSystemTask.PushMessage(Pinetime::System::SystemTask::Messages::OnResourceUpdateStart);
          }

          // Notify to send Firmware !          
          data[1] = (uint8_t)Opcodes::COMMAND_FIRMWARE_INIT;
          auto *om = ble_hs_mbuf_from_flat(&data, sizeof(data));
          ble_gattc_notify_custom(connectionHandle, eventHandle, om);

          return 0;
        }

        case Opcodes::COMMAND_FIRMWARE_CHECKSUM: {
          

          // verify Checksum
          bleController.State(Pinetime::Controllers::Ble::FirmwareUpdateStates::Validated);
          bleController.StopFirmwareUpdate();

          // Notify result of checksum
          uint8_t data[2];
          data[0] = 0x01;
          data[1] = (uint8_t)Opcodes::COMMAND_FIRMWARE_CHECKSUM;
          auto *om = ble_hs_mbuf_from_flat(&data, sizeof(data));
          ble_gattc_notify_custom(connectionHandle, eventHandle, om);

          mSystemTask.PushMessage(Pinetime::System::SystemTask::Messages::OnResourceUpdateEnd);

          return 0;
        }

        case Opcodes::COMMAND_FIRMWARE_START_DATA: {
          

          // update download

          //mSystemTask.PushMessage(Pinetime::System::SystemTask::Messages::OnResourceUpdateEnd);

          return 0;
        }

        case Opcodes::COMMAND_FIRMWARE_UPDATE_SYNC: {
          
          if(bleController.IsFirmwareUpdating()) {
            xTimerStart(timeoutTimer, 0);
          }

          return 0;
        }
       
        default:
          return BLE_ATT_ERR_REQ_NOT_SUPPORTED;
      }
    }
  }

  { uint16_t fileCharacteristicHandle;
    ble_gatts_find_chr(reinterpret_cast<const ble_uuid_t*>(&serviceUuid),
                      reinterpret_cast<const ble_uuid_t*>(&fileCharacteristicUuid),
                      nullptr /* don't get defintion handle */,
                      &fileCharacteristicHandle);
    if(attributeHandle == fileCharacteristicHandle) {
      switch(context->op) {
        case BLE_GATT_ACCESS_OP_WRITE_CHR: {
          return handleWrite(context);
        }
        case BLE_GATT_ACCESS_OP_READ_CHR: {
          return handleRead(context);
        }
        default:
          return BLE_ATT_ERR_UNLIKELY;
      }
    }
  }

  return BLE_ATT_ERR_UNLIKELY;
}

void FileService::closeActive() {

}

int FileService::handleWrite(ble_gatt_access_ctxt* context) {

  bytesReceived += context->om->om_len;
  bleController.FirmwareUpdateCurrentBytes(bytesReceived);

  /*if(mFile != nullptr) {
    // handle file write
    const int rtn = mFile->write(context->om->om_data, context->om->om_len);
    return (rtn == 0) ? 0 : BLE_ATT_ERR_INSUFFICIENT_RES;
  }*/
  return 0;
  //return BLE_ATT_ERR_WRITE_NOT_PERMITTED;
}

int FileService::handleRead(ble_gatt_access_ctxt* context) {

  //return 0;

  return BLE_ATT_ERR_READ_NOT_PERMITTED;
}

void FileService::OnTimeout() {
  bytesReceived = 0;
  bleController.State(Pinetime::Controllers::Ble::FirmwareUpdateStates::Error);
  bleController.StopFirmwareUpdate();
  mSystemTask.PushMessage(Pinetime::System::SystemTask::Messages::OnResourceUpdateEnd);
}
