#include "HRS3300.h"
#include <FreeRTOS.h>
#include <legacy/nrf_drv_gpiote.h>
#include <nrfx_log.h>
#include <task.h>

#include "hrs3300/hrs3300.h"
#include <math.h>

extern "C" { int __hardfp_sqrt(int f){ return sqrt(f); } }

/*
*
* Code from : https://github.com/sethitow/mbed-pinetime/blob/master/drivers/HRS3300_HeartRateSensor/HRS3300_HeartRateSensor.cpp
* and : https://github.com/atc1441/HRS3300-Arduino-Library
*
*/

using namespace Pinetime::Drivers;


HRS3300::HRS3300(TwiMaster &twiMaster) : twiMaster{twiMaster} {}

void HRS3300::Init()
{
    uint8_t charbuf = 0;
    
    i2c_reg_read(HRS3300_REG_ID, &charbuf, 1);
    if (charbuf != HRS3300_DEVICE_ID)
    {
        return;
    }

    i2c_reg_read(HRS3300_REG_RESOLUTION, &charbuf, 1);
    //tr_info("res: 0x%hu", charbuf);
    i2c_reg_read(HRS3300_REG_HGAIN, &charbuf, 1);
    //tr_info("gain: 0x%hu", charbuf);
	
    SetEnable();
    SetAdcResolution();
    SetHrsGain();

}

void HRS3300::SetEnable(enum HRS_ENABLE_DISABLE enable,
                                         enum HRS_WAIT_TIME wait_time, enum PDRIVE_CURRENT current,
                                         enum HRS_ENABLE_DISABLE p_on)
{
    uint8_t charbuf = 0;

    charbuf = charbuf | enable << 7;
    charbuf = charbuf | wait_time << 4;
    charbuf = charbuf | (current & (1 << 1)) << 2;
    i2c_reg_write(HRS3300_REG_ENABLE, &charbuf, 1);

    charbuf = 0;
    charbuf = charbuf | (current & 1) << 6;
    charbuf = charbuf | p_on << 5;
    charbuf = charbuf | 1 << 3; // This bit is set by default. Not sure if it needs to be set.
    i2c_reg_write(HRS3300_REG_PDRIVER, &charbuf, 1);

    if ( enable == HRS_ENABLE)
        Hrs3300_alg_open();
    else
        Hrs3300_alg_close();

}

void HRS3300::SetAdcResolution(enum ADC_RESOLUTION resolution)
{
    uint8_t charbuf = 0x60; // Default value is 0x66, not sure what the other bits do.
    charbuf = charbuf | resolution;
    i2c_reg_write(HRS3300_REG_RESOLUTION, &charbuf, 1);
}

void HRS3300::SetHrsGain(enum HRS_GAIN gain)
{
    uint8_t charbuf = 0;
    charbuf = charbuf | gain << 2;
    i2c_reg_write(HRS3300_REG_HGAIN, &charbuf, 1);
}

uint16_t HRS3300::ReadHeartRateSensor()
{
    uint16_t ret_val = 0;
    uint8_t  databuf[3];

    i2c_reg_read(HRS3300_REG_C0DATAM, &databuf[0], 1);
    i2c_reg_read(HRS3300_REG_C0DATAH, &databuf[1], 1);
    i2c_reg_read(HRS3300_REG_C0DATAL, &databuf[2], 1);    
    
    ret_val = ((databuf[0] << 8 ) | ((databuf[1] & 0x0F) << 4) | (databuf[2] & 0x0F));

    return ret_val;
}

uint16_t  HRS3300::ReadAmbientLightSensor()
{
    uint16_t ret_val = 0;
    uint8_t  databuf[3];

    i2c_reg_read(HRS3300_REG_C1DATAM, &databuf[0], 1);
    i2c_reg_read(HRS3300_REG_C1DATAH, &databuf[1], 1);
    i2c_reg_read(HRS3300_REG_C1DATAL, &databuf[2], 1);        

    ret_val = ((databuf[0] << 3) | ((databuf[1] & 0x3F) << 11) | (databuf[2] & 0x07));
	
	if (ret_val > 32767) ret_val = 32767; 

    return ret_val;
}

uint8_t HRS3300::ReadHeartRate() {

    int16_t new_raw_data, als_raw_data;
    hrs3300_results_t alg_results;    
    
    static uint16_t timer_index = 0;

    new_raw_data = ReadHeartRateSensor();
    als_raw_data = ReadAmbientLightSensor();

    Hrs3300_alg_send_data(new_raw_data, als_raw_data, 0, 0, 0, 0);

    //timer_index++;
    //if (timer_index >= 25)
    //{
        timer_index = 0;
        alg_results = Hrs3300_alg_get_results();

        switch (alg_results.alg_status)
        {
            case MSG_ALG_NOT_OPEN:
                return 254;
                break;
            case MSG_NO_TOUCH:
                return 253;
                break;
            case MSG_PPG_LEN_TOO_SHORT:
                return 252;
                break;
            case MSG_HR_READY:
                heartRate = alg_results;
                return 251;
                break;
            case MSG_ALG_TIMEOUT:
                return 250;
                break;
            case MSG_SETTLE:
                return 249;
                break;
            
            default:
                break;
        }

    //}


    return 255;
}

/* --------------------------------------------------------------- */
uint16_t HRS3300::i2c_reg_write(uint8_t reg_addr,
                                uint8_t *reg_data, uint16_t length)
{

    /* Write to registers using I2C. Return 0 for a successful execution. */

    twiMaster.Write(HRS3300_I2C_ADDRESS, reg_addr, (const uint8_t *)reg_data, length);
    return 0;
   
}

uint16_t HRS3300::i2c_reg_read(uint8_t reg_addr,
                               uint8_t *reg_data, uint16_t length)
{

    /* Read from registers using I2C. Return 0 for a successful execution. */

    twiMaster.Read(HRS3300_I2C_ADDRESS, reg_addr, (uint8_t *)reg_data, length);
    return 0;    
}


void Hrs3300_write_reg(uint8_t addr, uint8_t data) 
{
   //_i2c_write(0x44, addr, &data, 1);
}

uint8_t Hrs3300_read_reg(uint8_t addr) 
{
   uint8_t reg_temp;
   //_i2c_read(0x44, addr, &reg_temp, 1);
   return reg_temp;
}
