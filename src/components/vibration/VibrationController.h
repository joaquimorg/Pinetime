#pragma once

#include <cstdint>

namespace Pinetime {
  namespace Controllers {
    class VibrationController {
    public:
        void Init();
        void Vibrate(uint8_t durationMs);

    private:

    };
  }
}
