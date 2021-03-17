#pragma once
#include <cstddef>
#include <cstdint>

namespace Pinetime {
  namespace Drivers {
    class Spi;
    class St7789 {
      public:
        explicit St7789(Spi& spi, uint8_t pinDataCommand);
        St7789(const St7789&) = delete;
        St7789& operator=(const St7789&) = delete;
        St7789(St7789&&) = delete;
        St7789& operator=(St7789&&) = delete;

        void Init();
        void Uninit();
        void DrawPixel(uint16_t x, uint16_t y, uint32_t color);

        void PartialDisplay(uint8_t mode);
        void setPartArea(uint16_t sr, uint16_t er);

        void VerticalScrollDefinition(uint16_t topFixedLines, uint16_t scrollLines, uint16_t bottomFixedLines);
        void VerticalScrollStartAddress(uint16_t line);


        void BeginDrawBuffer(uint16_t x, uint16_t y, uint16_t width, uint16_t height);
        void NextDrawBuffer(const uint8_t* data, size_t size);

        void DrawBuffer(uint16_t x1, uint16_t x2, uint16_t y1, uint16_t y2, const uint8_t *data);

        void DisplayOn();
        void DisplayOff();

        void Sleep();
        void Wakeup();
      private:
        Spi& spi;
        uint8_t pinDataCommand;
        uint16_t verticalScrollingStartAddress = 0;

        void HardwareReset();
        void SoftwareReset();
        void SleepOut();
        void SleepIn();
        void ColMod();
        void MemoryDataAccessControl();
        void DisplayInversionOn();
        void NormalModeOn();
        void WriteToRam();
        void SetAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
        void WriteCommand(uint8_t cmd);
        void WriteSpi(const uint8_t* data, size_t size);

        void WriteData(uint8_t data);
        void ColumnAddressSet();

        static constexpr uint16_t Width = 240;
        static constexpr uint16_t Height = 320;
        void RowAddressSet();
        bool isLocked = false;
    };
  }
}


