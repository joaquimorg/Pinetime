#include "FileService.h"

#include <../components/fs/FS.h>
#include "systemtask/SystemTask.h"
#include <string>

using namespace Pinetime::Controllers;

// provide linktime space for constants
constexpr ble_uuid128_t FileService::serviceUuid;
constexpr ble_uuid128_t FileService::fileCharacteristicUuid;
constexpr ble_uuid128_t FileService::fileControlCharacteristicUuid;

/// C level hook to call class methods
int FileServiceCallback(uint16_t conn_handle, uint16_t attr_handle,
                       struct ble_gatt_access_ctxt *ctxt, void *arg) {
  auto fileService = static_cast<FileService *>(arg);
  return fileService->OnServiceData(conn_handle, attr_handle, ctxt);
}

FileService::FileService(Pinetime::System::SystemTask &systemTask,
                       Pinetime::Controllers::Ble &bleController,
                       Pinetime::Controllers::FS &fs)

: mSystemTask{systemTask}, mBleController{bleController}, fs{fs},

  mCharacteristicDefinitions{
      { .uuid = reinterpret_cast<const ble_uuid_t*>(&fileCharacteristicUuid),
        .access_cb = FileServiceCallback, .arg = this,
        .flags = (BLE_GATT_CHR_F_WRITE_NO_RSP | BLE_GATT_CHR_F_READ), .val_handle = nullptr },

      { .uuid = reinterpret_cast<const ble_uuid_t*>(&fileControlCharacteristicUuid),
        .access_cb = FileServiceCallback, .arg = this,
        .flags = (BLE_GATT_CHR_F_WRITE | BLE_GATT_CHR_F_NOTIFY), .val_handle = nullptr },

      { 0 /* null terminator */ }
  },

  mServiceDefinitions{
      { .type = BLE_GATT_SVC_TYPE_PRIMARY,
        .uuid = reinterpret_cast<const ble_uuid_t*>(&serviceUuid),
        .characteristics = mCharacteristicDefinitions
      },

      { 0 /* null terminator */ }
  }
{

}

void FileService::Init() {
  int res;
  res = ble_gatts_count_cfg(mServiceDefinitions);
  ASSERT(res == 0);

  res = ble_gatts_add_svcs(mServiceDefinitions);
  ASSERT(res == 0);
}

int FileService::OnServiceData(uint16_t connectionHandle, uint16_t attributeHandle, ble_gatt_access_ctxt *context) {

  { uint16_t fileControlCharacteristicHandle;
    ble_gatts_find_chr(reinterpret_cast<const ble_uuid_t*>(&serviceUuid),
                       reinterpret_cast<const ble_uuid_t*>(&fileControlCharacteristicUuid),
                       nullptr /* don't get defintion handle */,
                       &fileControlCharacteristicHandle);
    if(attributeHandle == fileControlCharacteristicHandle) {

      const auto opcode = static_cast<Opcodes>(context->om->om_data[1]);

      switch(opcode) {
        case Opcodes::START: {
          closeActive();
          int rtn = 0;
          uint8_t data[1];

          data[0] = 0x01;

          rtn = os_mbuf_append(context->om, &data, sizeof(data));

          mSystemTask.PushMessage(Pinetime::System::SystemTask::Messages::OnResourceUpdateStart);

          return (rtn == 0) ? 0 : BLE_ATT_ERR_INSUFFICIENT_RES;
        }
        case Opcodes::END: {
          closeActive();
          int rtn = 0;
          uint8_t data[1];

          data[0] = 0x01;

          rtn = os_mbuf_append(context->om, &data, sizeof(data));

          mSystemTask.PushMessage(Pinetime::System::SystemTask::Messages::OnResourceUpdateEnd);
          return (rtn == 0) ? 0 : BLE_ATT_ERR_INSUFFICIENT_RES;
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

  /*if(mFile != nullptr) {
    // handle file write
    const int rtn = mFile->write(context->om->om_data, context->om->om_len);
    return (rtn == 0) ? 0 : BLE_ATT_ERR_INSUFFICIENT_RES;
  }*/

  return BLE_ATT_ERR_WRITE_NOT_PERMITTED;
}

int FileService::handleRead(ble_gatt_access_ctxt* context) {
  
  //constexpr size_t GATT_MAX = 512;  
  // handle file read
  uint8_t buffer[2];
  buffer[0] = 0xff;
  buffer[1] = 0x01;
  os_mbuf_append(context->om, buffer, sizeof(buffer));

  return 0;

  //return BLE_ATT_ERR_READ_NOT_PERMITTED;
}


