#include "HRS3300.h"
#include <FreeRTOS.h>
#include <legacy/nrf_drv_gpiote.h>
#include <nrfx_log.h>
#include <task.h>
#include <math.h>

#include "hrs3300/hrs3300.h"

/*
*
* Code from : https://github.com/sethitow/mbed-pinetime/blob/master/drivers/HRS3300_HeartRateSensor/HRS3300_HeartRateSensor.cpp
* and : https://github.com/atc1441/HRS3300-Arduino-Library
*
*/

extern "C" { int __hardfp_sqrt(int f){ return sqrt(f); } }


const uint8_t Hrs3300_init_register_array[][2] = {
    {0x01, 0xd0}, //11010000  bit[7]=1,HRS enable;bit[6:4]=101,wait time=50ms,bit[3]=0,LED DRIVE=22 mA
    //{0x01, 0xf0},   //11010000  bit[7]=1,HRS enable;bit[6:4]=101,wait time=50ms,bit[3]=0,LED DRIVE=22 mA v13.05
    {0x0c, 0x4e}, //00001110  bit[6]=0,LED DRIVE=22mA;bit[5]=0,sleep mode;p_pulse=1110,duty=50%
    {0x16, 0x78}, //01111000  bits[7:4]=0111,HRS 15bits
    {0x17, 0x0d}, //00001101  bits[7:5]=011,HRS gain 16*;bit[1]=0,HRS data=AD0
    {0x02, 0x80},
    {0x03, 0x00},
    {0x04, 0x00},
    {0x05, 0x00},
    {0x06, 0x00},
    {0x07, 0x00},
    {0x08, 0x74},
    {0x09, 0x00},
    {0x0a, 0x08},
    {0x0b, 0x00},
    {0x0c, 0x6e},
    {0x0d, 0x02},
    {0x0e, 0x07},
    {0x0f, 0x0f},
};
#define HRS3300_INIT_ARRAY_SIZE (sizeof(Hrs3300_init_register_array) / sizeof(Hrs3300_init_register_array[0]))

using namespace Pinetime::Drivers;

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

HRS3300::HRS3300(TwiMaster &twiMaster) : twiMaster{twiMaster} {}

void HRS3300::Init()
{
    uint8_t charbuf = 0;
    uint8_t _reg_0x7f, _reg_0x80, _reg_0x81, _reg_0x82;

    i2c_reg_read(HRS3300_REG_ID, &charbuf, 1);
    if (charbuf != HRS3300_DEVICE_ID)
    {
        return;
    }

    i2c_reg_read(HRS3300_REG_RESOLUTION, &charbuf, 1);
    //tr_info("res: 0x%hu", charbuf);
    i2c_reg_read(HRS3300_REG_HGAIN, &charbuf, 1);
    //tr_info("gain: 0x%hu", charbuf);
        
	for (int i = 0; i < HRS3300_INIT_ARRAY_SIZE; i++)
	{		
        i2c_reg_write(Hrs3300_init_register_array[i][0], (uint8_t *)&Hrs3300_init_register_array[i][1], 1);
	}

	//if (hrs3300_power_up_flg == 0)
	//{
        i2c_reg_read(0x7f, &_reg_0x7f, 1);
        i2c_reg_read(0x80, &_reg_0x80, 1);
        i2c_reg_read(0x81, &_reg_0x81, 1);
        i2c_reg_read(0x82, &_reg_0x82, 1);

        Hrs3300_set( _reg_0x7f, _reg_0x80, _reg_0x81, _reg_0x82 );

		//reg_0x7f = Hrs3300_read_reg(0x7f);
		//reg_0x80 = Hrs3300_read_reg(0x80);
		//reg_0x81 = Hrs3300_read_reg(0x81);
		//reg_0x82 = Hrs3300_read_reg(0x82);
		//hrs3300_power_up_flg = 1;
	//}
	
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
    
    ret_val = ((databuf[0] <<8 ) | ((databuf[1] & 0x0F) << 4) | (databuf[2] & 0x0F));

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

uint8_t HRS3300::GetHeartRate() {

    int16_t new_raw_data, als_raw_data;
    hrs3300_results_t alg_results;    
    hrs3300_bp_results_t bp_alg_results;

    static uint16_t timer_index = 0;

    new_raw_data = ReadHeartRateSensor();
    als_raw_data = ReadAmbientLightSensor();

    Hrs3300_alg_send_data(new_raw_data, als_raw_data, 0, 0, 0, 0);

    timer_index++;
    if (timer_index >= 25)
    {
        timer_index = 0;
        alg_results = Hrs3300_alg_get_results();
        if (alg_results.alg_status == MSG_NO_TOUCH)
        {
            return 254;
        }
        else if (alg_results.alg_status == MSG_PPG_LEN_TOO_SHORT)
        {
            return 253;
        }
        else
        {
            bp_alg_results = Hrs3300_alg_get_bp_results();
            return alg_results.hr_result;
        }
    }
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