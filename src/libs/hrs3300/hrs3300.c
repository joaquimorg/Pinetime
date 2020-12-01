#include <stdio.h>
#include <stdbool.h>
#include "hrs3300.h"
#include "hrs3300_reg_init.h"

const uint8_t hrs3300_bp_timeout_grade = 0;
const uint8_t hrs3300_agc_init_stage = 0x04;
const uint8_t hrs3300_bp_power_grade = 1;
const uint8_t hrs3300_accurate_first_shot = 0;
const uint8_t hrs3300_up_factor = 3;
const uint8_t hrs3300_up_shift = 2;
const uint16_t hrs3300_AMP_LTH = 120;
const uint16_t hrs3300_hr_AMP_LTH = 150;
const uint16_t hrs3300_hr_PVAR_LTH = 10;

static bool hrs3300_power_up_flg = 0;
uint8_t reg_0x7f;
uint8_t reg_0x80;
uint8_t reg_0x81;
uint8_t reg_0x82;

uint16_t Hrs3300_read_hrs(void)
{
	uint8_t databuf[3];
	uint16_t data;

	databuf[0] = Hrs3300_read_reg(0x09);
	databuf[1] = Hrs3300_read_reg(0x0a);
	databuf[2] = Hrs3300_read_reg(0x0f);

	data = ((databuf[0] << 8) | ((databuf[1] & 0x0F) << 4) | (databuf[2] & 0x0F));

	return data;
}

uint16_t Hrs3300_read_als(void)
{
	uint8_t databuf[3];
	uint16_t data;

	databuf[0] = Hrs3300_read_reg(0x08);
	databuf[1] = Hrs3300_read_reg(0x0d);
	databuf[2] = Hrs3300_read_reg(0x0e);

	data = ((databuf[0] << 3) | ((databuf[1] & 0x3F) << 11) | (databuf[2] & 0x07));

	if (data > 32767)
		data = 32767;

	return data;
}

void Hrs3300_set( uint8_t _reg_0x7f, uint8_t _reg_0x80, uint8_t _reg_0x81, uint8_t _reg_0x82 ) {

	reg_0x7f = _reg_0x7f;
	reg_0x80 = _reg_0x80;
	reg_0x81 = _reg_0x81;
	reg_0x82 = _reg_0x82;
	hrs3300_power_up_flg = 1;

}

bool Hrs3300_chip_init()
{
	int i = 0;
	for (i = 0; i < INIT_ARRAY_SIZE; i++)
	{
		Hrs3300_write_reg(init_register_array[i][0], init_register_array[i][1]);
	}

	if (hrs3300_power_up_flg == 0)
	{
		reg_0x7f = Hrs3300_read_reg(0x7f);
		reg_0x80 = Hrs3300_read_reg(0x80);
		reg_0x81 = Hrs3300_read_reg(0x81);
		reg_0x82 = Hrs3300_read_reg(0x82);
		hrs3300_power_up_flg = 1;
	}
	return true;
}

void Hrs3300_chip_enable()
{
	Hrs3300_write_reg(0x16, 0x78);
	Hrs3300_write_reg(0x01, 0xd0);
	Hrs3300_write_reg(0x0c, 0x2e);

	return;
}

void Hrs3300_chip_disable()
{
	Hrs3300_write_reg(0x01, 0x08);
	Hrs3300_write_reg(0x02, 0x80);
	Hrs3300_write_reg(0x0c, 0x4e);

	Hrs3300_write_reg(0x16, 0x88);

	Hrs3300_write_reg(0x0c, 0x22);
	Hrs3300_write_reg(0x01, 0xf0);
	Hrs3300_write_reg(0x0c, 0x02);

	Hrs3300_write_reg(0x0c, 0x22);
	Hrs3300_write_reg(0x01, 0xf0);
	Hrs3300_write_reg(0x0c, 0x02);

	Hrs3300_write_reg(0x0c, 0x22);
	Hrs3300_write_reg(0x01, 0xf0);
	Hrs3300_write_reg(0x0c, 0x02);

	Hrs3300_write_reg(0x0c, 0x22);
	Hrs3300_write_reg(0x01, 0xf0);
	Hrs3300_write_reg(0x0c, 0x02);

	return;
}

uint8_t get_heart_rate()
{
	int16_t new_raw_data, als_raw_data;
	hrs3300_results_t alg_results;
	uint8_t n;
	hrs3300_bp_results_t bp_alg_results;
	static uint16_t timer_index = 0;
	new_raw_data = Hrs3300_read_hrs();
	als_raw_data = Hrs3300_read_als();
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