#pragma once

#include <FreeRTOS.h>
#include <timers.h>
#include "TwiMaster.h"
#include "board_config.h"
#include "hrs3300/hrs3300.h"

#define HRS3300_I2C_ADDRESS HR_TWI_ADDR
#define HRS3300_DEVICE_ID 0x21

#define HRS3300_REG_ID 0x00
#define HRS3300_REG_ENABLE 0x01
#define HRS3300_REG_PDRIVER 0x0C

#define HRS3300_REG_C0DATAL 0x0F
#define HRS3300_REG_C0DATAM 0x09
#define HRS3300_REG_C0DATAH 0x0A

#define HRS3300_REG_C1DATAL 0x0E
#define HRS3300_REG_C1DATAM 0x08
#define HRS3300_REG_C1DATAH 0x0D

#define HRS3300_REG_RESOLUTION 0x16
#define HRS3300_REG_HGAIN 0x17

namespace Pinetime {
  namespace Drivers {
    class HRS3300 {
        public :

          enum HRS_WAIT_TIME
          {
              HRS_WAIT_TIME_800ms = 0,
              HRS_WAIT_TIME_400ms = 1,
              HRS_WAIT_TIME_200ms = 2,
              HRS_WAIT_TIME_100ms = 3,
              HRS_WAIT_TIME_75ms = 4,
              HRS_WAIT_TIME_50ms = 5,
              HRS_WAIT_TIME_12_5ms = 6,
              HRS_WAIT_TIME_0ms = 7
          };

          enum HRS_ENABLE_DISABLE
          {
              HRS_DISABLE = 0,
              HRS_ENABLE = 1

          };

          enum PDRIVE_CURRENT
          {
              PDRIVE_CURRENT_12_5mA = 0,
              PDRIVE_CURRENT_20mA = 1,
              PDRIVE_CURRENT_30mA = 2,
              PDRIVE_CURRENT_40mA = 3
          };

          enum ADC_RESOLUTION
          {
              ADC_RESOLUTION_8b = 0,
              ADC_RESOLUTION_9b = 1,
              ADC_RESOLUTION_10b = 2,
              ADC_RESOLUTION_11b = 3,
              ADC_RESOLUTION_12b = 4,
              ADC_RESOLUTION_13b = 5,
              ADC_RESOLUTION_14b = 6,
              ADC_RESOLUTION_15b = 7,
              ADC_RESOLUTION_16b = 8,
              ADC_RESOLUTION_17b = 9,
              ADC_RESOLUTION_18b = 10,
          };

          enum HRS_GAIN
          {
              HRS_GAIN_1x = 0,
              HRS_GAIN_2x = 1,
              HRS_GAIN_4x = 2,
              HRS_GAIN_8x = 3,
              HRS_GAIN_64x = 4
          };

          HRS3300(TwiMaster& twiMaster);
          HRS3300(const HRS3300&) = delete;
          HRS3300& operator=(const HRS3300&) = delete;
          HRS3300(HRS3300&&) = delete;
          HRS3300& operator=(HRS3300&&) = delete;

          void Init();
          void SetEnable(enum HRS_ENABLE_DISABLE enable = HRS_DISABLE,
                  enum HRS_WAIT_TIME wait_time = HRS_WAIT_TIME_12_5ms,
                  enum PDRIVE_CURRENT current = PDRIVE_CURRENT_40mA,
                  enum HRS_ENABLE_DISABLE p_on = HRS_ENABLE);
          void SetAdcResolution(enum ADC_RESOLUTION resolution = ADC_RESOLUTION_14b);
          void SetHrsGain(enum HRS_GAIN gain = HRS_GAIN_64x);
          uint16_t ReadHeartRateSensor();
          uint16_t ReadAmbientLightSensor();
          
          void HRReading();

          uint8_t GetHeartRate() const { return heartRate; };
          uint8_t GetBPHigh() const { return bpHigh; };
          uint8_t GetBPLow() const { return bpLow; };

          uint8_t GetStatus() const { return heartRateStatus; };
          uint8_t GetHistory( uint8_t pos ) const { return heartRateHistory[pos]; };


        private:

          TwiMaster& twiMaster;

          uint8_t heartRate = 0;
          uint8_t bpHigh = 0;
          uint8_t bpLow = 0;
          uint8_t heartRateStatus = 255;
          uint8_t timerIsRunning = 0;

          uint8_t heartRateHistory[6] = {0};

          uint16_t timer_index = 0;
          TimerHandle_t hrTimer;

          uint8_t ReadHeartRate();

          void i2c_reg_write(uint8_t reg_addr, uint8_t *reg_data, uint16_t length);
          void i2c_reg_read(uint8_t reg_addr, uint8_t *reg_data, uint16_t length);
            
    };
  }
}