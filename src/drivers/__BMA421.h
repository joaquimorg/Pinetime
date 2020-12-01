#pragma once

#include "TwiMaster.h"

#define BMA421_CHIP_ID_ADDR                     UINT8_C(0X00)
#define BMA421_CHIP_ID_VALUE                    (0x11)

namespace Pinetime {
  namespace Drivers {

    struct accl_data_struct {
      int16_t x;
      int16_t y;
      int16_t z;      
    };
    class BMA421 {
        public :
            
          BMA421(TwiMaster& twiMaster);
          BMA421(const BMA421&) = delete;
          BMA421& operator=(const BMA421&) = delete;
          BMA421(BMA421&&) = delete;
          BMA421& operator=(BMA421&&) = delete;

          void Init();
          void Update();

          void Wakeup();
          void Sleep();

          uint32_t GetSteps() const { return step_count; };
          uint8_t GetActivity() const { return activity; };
          uint8_t GetTapStatus() const { return 0; };
          float GetTemp() const { return tempC; };
          accl_data_struct GetAccel() const { return accelData; };

        private:

          TwiMaster& twiMaster;
          uint8_t twiAddress;

          uint8_t deviceReady = 0;

          struct accl_data_struct accelData;
          uint32_t step_count = 0;
          uint8_t activity = 0;
          //uint8_t tapStatus = 0;
          float tempC = 0;

          
          //void accl_write_reg(uint8_t reg, uint8_t *reg_data);
          uint8_t accl_read_reg(uint8_t reg);

          void accl_config_read_write(bool rw, uint8_t addr, uint8_t *data, uint32_t len, uint32_t offset);
          void reset_step_counter();
          uint32_t read_step_data();
          void update_accl_data();

          void reg_write(uint8_t reg_addr, uint8_t reg_data);
          void i2c_reg_write(uint8_t reg_addr, uint8_t *reg_data, uint16_t length);
          void i2c_reg_read(uint8_t reg_addr, uint8_t *reg_data, uint16_t length);

            
    };
  }
}