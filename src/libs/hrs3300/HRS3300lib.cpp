#include "HRS3300lib.h"
#include <math.h>

extern "C" { int __hardfp_sqrt(int f){ return sqrt(f); } }

i2c_read_type _i2c_read;
i2c_write_type _i2c_write;
	
void HRS3300_begin(i2c_read_type i2c_read,i2c_write_type i2c_write)
{
  _i2c_read = i2c_read;
  _i2c_write = i2c_write;
  Hrs3300_chip_init();
  Hrs3300_chip_enable();
  Hrs3300_set_exinf(0, 0, 0, 0, 0, 0);
  Hrs3300_alg_open();
}

void HRS3300_enable(void)
{
  Hrs3300_chip_enable();
}

void HRS3300_disable(void)
{
  Hrs3300_chip_disable();
}

uint8_t HRS3300_getHR(void)
{
  return get_heart_rate();
}

void Hrs3300_write_reg(uint8_t addr, uint8_t data) 
{
   _i2c_write(0x44, addr, &data, 1);
}

uint8_t Hrs3300_read_reg(uint8_t addr) 
{
   uint8_t reg_temp;
   _i2c_read(0x44, addr, &reg_temp, 1);
   return reg_temp;
}

