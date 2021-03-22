#pragma once

#include <cstdint>
#include <lvgl/lvgl.h>
#include "Screen.h"

namespace Pinetime {

  namespace Drivers {
    class SpiNorFlash;
  }

  namespace Applications {
    namespace Screens {

      class Weather : public Screen{
        public:
          Weather(DisplayApp* app, Pinetime::Drivers::SpiNorFlash& spiNorFlash);
          ~Weather() override;

          bool Refresh() override;
         
        private:          

          Pinetime::Drivers::SpiNorFlash& spiNorFlash;
          
          lv_obj_t* llabel;

          uint16_t CalculateCrc();
          uint16_t ComputeCrc(uint8_t const *p_data, uint32_t size, uint16_t const *p_crc);

      };
    }
  }
}
