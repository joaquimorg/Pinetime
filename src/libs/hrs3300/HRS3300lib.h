#ifndef HRS_CLASS_H
#define HRS_CLASS_H

#include "hrs3300.h"

typedef uint8_t (*i2c_read_type)(uint8_t addr, uint8_t reg_addr, uint8_t *reg_data, uint32_t length);

typedef uint8_t (*i2c_write_type)(uint8_t addr, uint8_t reg_addr, const uint8_t *reg_data, uint32_t length);

void HRS3300_begin(i2c_read_type i2c_read,i2c_write_type i2c_write);
	
void HRS3300_enable(void);

void HRS3300_disable(void);
	
uint8_t HRS3300_getHR(void);

#endif
