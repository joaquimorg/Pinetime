#pragma once

#include <array>
#include <cstdint>
#include <nrf_log.h>

namespace Pinetime {
  namespace Controllers {
    class Ble {
      public:
        using BleAddress = std::array<uint8_t, 6>;
        enum class FirmwareUpdateStates {Idle, Running, Validated, Error, FormatFlash};
        enum class AddressTypes { Public, Random };
        enum class FirmwareType { FW, RES, BOT };

        Ble() = default;
        bool IsConnected() const {return isConnected;}
        void Connect();
        void Disconnect();

        void StartFirmwareUpdate();
        void StopFirmwareUpdate();
        void FirmwareUpdateTotalBytes(uint32_t totalBytes);
        void FirmwareUpdateCurrentBytes(uint32_t currentBytes);
        void State(FirmwareUpdateStates state) { firmwareUpdateState = state; }

        bool IsFirmwareUpdating() const { return isFirmwareUpdating; }
        uint32_t FirmwareUpdateTotalBytes() const { return firmwareUpdateTotalBytes; }
        uint32_t FirmwareUpdateCurrentBytes() const { return firmwareUpdateCurrentBytes; }
        FirmwareUpdateStates State() const { return firmwareUpdateState; }

        void Address(BleAddress&& addr) { address = addr; }
        const BleAddress& Address() const { return address; }
        void AddressType(AddressTypes t) { addressType = t;}

        void FWType(FirmwareType type) { firmwareType = type; }
        FirmwareType FWType() const { return firmwareType; }
      private:
        bool isConnected = false;
        bool isFirmwareUpdating = false;
        uint32_t firmwareUpdateTotalBytes = 0;
        uint32_t firmwareUpdateCurrentBytes = 0;
        FirmwareUpdateStates firmwareUpdateState = FirmwareUpdateStates::Idle;
        BleAddress address;
        AddressTypes addressType;
        FirmwareType firmwareType;

    };
  }
}