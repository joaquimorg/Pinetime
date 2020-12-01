#ifndef _HRS3300_H_
#define _HRS3300_H_

#include <stdint.h>

#ifndef bool
#define bool unsigned char
#endif
#define WHO_AM_3300 0x21
typedef enum
{
	MSG_ALG_NOT_OPEN = 0x01,
	MSG_NO_TOUCH = 0x02,
	MSG_PPG_LEN_TOO_SHORT = 0x03,
	MSG_HR_READY = 0x04,
	MSG_ALG_TIMEOUT = 0x05,
	MSG_SETTLE = 0x06
} hrs3300_msg_code_t;

typedef enum
{
	MSG_BP_ALG_NOT_OPEN = 0x01,
	MSG_BP_NO_TOUCH = 0x02,
	MSG_BP_PPG_LEN_TOO_SHORT = 0x03,
	MSG_BP_READY = 0x04,
	MSG_BP_ALG_TIMEOUT = 0x05,
	MSG_BP_SETTLE = 0x06
} hrs3300_bp_msg_code_t;
typedef struct
{
	hrs3300_msg_code_t alg_status;
	uint32_t data_cnt;
	uint8_t hr_result;
	uint8_t hr_result_qual;
	bool object_flg;
} hrs3300_results_t;

typedef struct
{
	hrs3300_bp_msg_code_t bp_alg_status;
	uint8_t sbp;
	uint8_t dbp;
	uint32_t data_cnt;
	uint8_t hr_result;
	bool object_flg;
} hrs3300_bp_results_t;

#ifdef __cplusplus
extern "C"
{
#endif

	void Hrs3300_set( uint8_t _reg_0x7f, uint8_t _reg_0x80, uint8_t _reg_0x81, uint8_t _reg_0x82 );

	bool Hrs3300_chip_init(void);
	void Hrs3300_chip_enable(void);
	void Hrs3300_chip_disable(void);
	uint16_t Hrs3300_read_hrs(void);
	uint16_t Hrs3300_read_als(void);
	void Hrs3300_set_exinf(uint8_t age, uint8_t height, uint8_t weight, uint8_t gender, uint8_t ref_sbp, uint8_t ref_dbp);
	void Hrs3300_write_reg(uint8_t addr, uint8_t data);
	uint8_t Hrs3300_read_reg(uint8_t addr);
	hrs3300_results_t Hrs3300_alg_get_results(void);
	hrs3300_bp_results_t Hrs3300_alg_get_bp_results(void);
	extern bool Hrs3300_alg_open(void);
	extern void Hrs3300_bp_alg_open(void);
	extern bool Hrs3300_alg_send_data(int16_t new_raw_data, int16_t als_raw_data, int16_t gsen_data_x, int16_t gsen_data_y, int16_t gsen_data_z, uint16_t timer_time);
	extern bool Hrs3300_bp_alg_send_data(int16_t new_raw_data);
	extern void Hrs3300_alg_close(void);
	uint8_t get_heart_rate(void);

#ifdef __cplusplus
}
#endif
#endif // _HRS3300_H_
