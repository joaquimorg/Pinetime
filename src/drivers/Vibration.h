#pragma once

#include <cstdint>

namespace Pinetime {
  namespace Drivers {
    class Vibration {
    public:
        void Init();
        void Vibrate(uint8_t durationMs);

    private:

    };
  }
}
