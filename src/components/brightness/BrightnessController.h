#pragma once

#include <cstdint>

namespace Pinetime {
  namespace Controllers {
    class BrightnessController {
    public:
      enum class Levels {Off, Low, LowMedium, Medium, MediumHigh, High};
      void Init();

      void Set(Levels level);
      Levels Level() const;
      void Lower();
      void Higher();

      void Backup();
      void Restore();

    private:
      /*static constexpr uint8_t pinLcdBacklight1 = 14;
      static constexpr uint8_t pinLcdBacklight2 = 22;
      static constexpr uint8_t pinLcdBacklight3 = 23;*/
      Levels level = Levels::Medium;
      Levels backupLevel = Levels::Medium;
    };
  }
}
