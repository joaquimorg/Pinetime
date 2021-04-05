#include "TwiMaster.h"
#include <cstring>
#include <hal/nrf_gpio.h>
#include <nrfx_log.h>
#include "board_config.h"
#include "app_error.h"
#include "app_timer.h"

using namespace Pinetime::Drivers;

static const nrf_drv_twi_t m_twi = NRF_DRV_TWI_INSTANCE(1);

TwiMaster::TwiMaster(const Modules module, const Parameters& params) : module{module}, params{params} {
	mutex = xSemaphoreCreateBinary();
}

void TwiMaster::Init() {

	uint32_t err_code;

    nrf_drv_twi_config_t const config = {
       .scl                = params.pinScl,
       .sda                = params.pinSda,
       .frequency          = NRF_DRV_TWI_FREQ_400K,
       .interrupt_priority = APP_IRQ_PRIORITY_LOWEST,
       .clear_bus_init     = false
    };

    err_code = nrf_drv_twi_init(&m_twi, &config, NULL, NULL);
    APP_ERROR_CHECK(err_code);
    nrf_drv_twi_enable(&m_twi);

    xSemaphoreGive(mutex);
}


TwiMaster::ErrorCodes TwiMaster::Read(uint8_t deviceAddress, uint8_t registerAddress, uint8_t *data, size_t size) {
	ErrorCodes ret = ErrorCodes::NoError;
	
	ret_code_t err_code;
	xSemaphoreTake(mutex, portMAX_DELAY);
	err_code = nrf_drv_twi_tx(&m_twi, deviceAddress, &registerAddress, 1, false);
	err_code = nrf_drv_twi_rx(&m_twi, deviceAddress, data, size);
	xSemaphoreGive(mutex);

	return ret;
}

TwiMaster::ErrorCodes TwiMaster::Write(uint8_t deviceAddress, uint8_t registerAddress, const uint8_t *data, size_t size) {
	ErrorCodes ret = ErrorCodes::NoError;
	ASSERT(size <= maxDataSize);
	ret_code_t err_code;
	xSemaphoreTake(mutex, portMAX_DELAY);
	internalBuffer[0] = registerAddress;
	std::memcpy(internalBuffer+1, data, size);
	err_code = nrf_drv_twi_tx(&m_twi, deviceAddress, internalBuffer, size + 1, true);
	xSemaphoreGive(mutex);
	return ret;
}

void TwiMaster::Sleep() {

	/*nrf_gpio_cfg_default(6);
	nrf_gpio_cfg_default(7);*/
	NRF_LOG_INFO("[TWIMASTER] Sleep");
	}

void TwiMaster::Wakeup() {
	//Init();
	NRF_LOG_INFO("[TWIMASTER] Wakeup");
}

