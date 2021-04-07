#pragma once
#include <FreeRTOS.h>
#include <semphr.h>
#include <cstdint>

namespace Pinetime {
  namespace Drivers {
    class TwiMaster {
      public:
        enum class Modules { TWIM1 };
        enum class Frequencies {Khz100, Khz250, Khz400};
        enum class ErrorCodes {NoError, TransactionFailed};
        struct Parameters {
          uint32_t frequency;
          uint8_t pinSda;
          uint8_t pinScl;
        };

        uint8_t value;
        TwiMaster(const Modules module, const Parameters& params);

        void Init();
        ErrorCodes Read(uint8_t deviceAddress, uint8_t registerAddress, uint8_t* buffer, size_t size);
        ErrorCodes Write(uint8_t deviceAddress, uint8_t registerAddress, const uint8_t* data, size_t size);

        void Sleep();
        void Wakeup();
        void Disable();


      private:

        SemaphoreHandle_t mutex;
        const Modules module;
        const Parameters params;
        static constexpr uint8_t maxDataSize{64};
        static constexpr uint8_t registerSize{1};
        uint8_t internalBuffer[maxDataSize + registerSize];

    };
  }
}