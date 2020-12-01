#include "BMA421.h"
#include <FreeRTOS.h>
#include <legacy/nrf_drv_gpiote.h>
#include <nrfx_log.h>
#include <task.h>
#include "board_config.h"
#include "BMA421Config.h"

using namespace Pinetime::Drivers;

BMA421::BMA421(TwiMaster &twiMaster) : twiMaster{twiMaster} {}


/* --------------------------------------------------------------- */


void BMA421::accl_config_read_write(bool rw, uint8_t addr, uint8_t *data, uint32_t len, uint32_t offset) {
    uint8_t charbuf = 0;

    for (uint32_t i = 0; i < len; i += 16) {
        charbuf = (offset + (i / 2)) & 0x0F;
        reg_write(0x5B, charbuf);

        charbuf = (offset + (i / 2)) >> 4;    
        reg_write(0x5C, charbuf);

        if (rw)
            i2c_reg_read(0x5E, data + i, (len - i >= 16) ? 16 : (len - i));
        else
            i2c_reg_write(0x5E, data + i, (len - i >= 16) ? 16 : (len - i));
    }
}

void BMA421::reset_step_counter() {
    uint8_t feature_config[0x47] = { 0 };

    accl_config_read_write(1, 0x5E, feature_config, 0x46, 256);
    feature_config[0x3A + 1] = 0x34;
    accl_config_read_write(0, 0x5E, feature_config, 0x46, 256);

}

uint32_t BMA421::read_step_data() {
  uint32_t data;
  i2c_reg_read(0x1E, (uint8_t *)&data, 4);
  return data;
}

void BMA421::update_accl_data() {
  i2c_reg_read(0x12, (uint8_t *)&accelData.x, 6);
  short tempX = accelData.x;
  accelData.x = accelData.y;
  accelData.y = tempX;

  accelData.x = (accelData.x / 0x10);
  accelData.y = (accelData.y / 0x10);
  accelData.z = (accelData.z / 0x10);
}

void BMA421::Init() {
    
    uint8_t charbuf = 0;

    i2c_reg_read(BMA421_CHIP_ID_ADDR, &charbuf, 1);
    if (charbuf != BMA421_CHIP_ID_VALUE)
    {
        return;
    }


    // Reset
    reg_write(0x7E, 0xB6);
    //vTaskDelay(5);

    //Sleep disable    
    reg_write(0x7C, 0x00);
    //vTaskDelay(5);

    //Write Blob    
    //reg_write(0x59, 0x00);
    //accl_config_read_write(0, 0x5E, (uint8_t *)accl_config_file, sizeof(accl_config_file), 0);    
    //reg_write(0x59, 0x01);
    //vTaskDelay(5);

    //Accl Enable
    reg_write(0x7D, 0x04);
    //Acc Conf
    reg_write(0x40, 0b00101000);
    //Enable and Reset
    //reset_step_counter();
    //Sleep Enable
    //reg_write(0x7C, 0x03);

}

void BMA421::Wakeup() {
    
}

void BMA421::Sleep() {
   
}

void BMA421::Update() {
    uint8_t charbuf = 0;

    update_accl_data();
    step_count = read_step_data();
    //activity = accl_read_reg(0x27);
    i2c_reg_read(0x27, &activity, 1);
    i2c_reg_read(0x22, &charbuf, 1);
    tempC = (charbuf + 23) / 1000;
}


/* --------------------------------------------------------------- */

void BMA421::reg_write(uint8_t reg_addr, uint8_t reg_data)
{
     i2c_reg_write(reg_addr, &reg_data, 1);  
}

void BMA421::i2c_reg_write(uint8_t reg_addr,
                                uint8_t *reg_data, uint16_t length)
{
    /* Write to registers using I2C. Return 0 for a successful execution. */
    twiMaster.Write(BMA421_TWI_ADDR, reg_addr, (const uint8_t *)reg_data, length);
}

void BMA421::i2c_reg_read(uint8_t reg_addr,
                               uint8_t *reg_data, uint16_t length)
{
    /* Read from registers using I2C. Return 0 for a successful execution. */
    twiMaster.Read(BMA421_TWI_ADDR, reg_addr, (uint8_t *)reg_data, length);
}