#include "TwiMaster.h"
#include <cstring>
#include <hal/nrf_gpio.h>
#include <nrfx_log.h>
#include "board_config.h"
#include "app_error.h"
#include "app_timer.h"
#include <nrf_drv_twi.h>

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
       .frequency          = NRF_DRV_TWI_FREQ_250K,
       .interrupt_priority = APP_IRQ_PRIORITY_LOWEST,
       .hold_bus_uninit    = false
    };

    err_code = nrf_drv_twi_init(&m_twi, &config, NULL, NULL);
    
    APP_ERROR_CHECK(err_code);
    nrf_drv_twi_enable(&m_twi);

    xSemaphoreGive(mutex);
}


TwiMaster::ErrorCodes TwiMaster::Read(uint8_t deviceAddress, uint8_t registerAddress, uint8_t *data, size_t size) {	
	xSemaphoreTake(mutex, portMAX_DELAY);
	nrf_drv_twi_tx(&m_twi, deviceAddress, &registerAddress, 1, false);
	nrf_drv_twi_rx(&m_twi, deviceAddress, data, size);	
	xSemaphoreGive(mutex);
	return ErrorCodes::NoError;
}

TwiMaster::ErrorCodes TwiMaster::Write(uint8_t deviceAddress, uint8_t registerAddress, const uint8_t *data, size_t size) {	
	ASSERT(size <= maxDataSize);
	xSemaphoreTake(mutex, portMAX_DELAY);
	internalBuffer[0] = registerAddress;
	std::memcpy(internalBuffer+1, data, size);
	nrf_drv_twi_tx(&m_twi, deviceAddress, internalBuffer, size + 1, true);
	xSemaphoreGive(mutex);
	return ErrorCodes::NoError;
}

void TwiMaster::Disable() {

	nrf_drv_twi_disable(&m_twi);
	nrf_drv_twi_uninit(&m_twi);

	nrf_gpio_cfg_default(TWI_SCL);
	nrf_gpio_cfg_default(TWI_SDA);
}

void TwiMaster::Sleep() {
	nrf_drv_twi_disable(&m_twi);
}

void TwiMaster::Wakeup() {
  	nrf_drv_twi_enable(&m_twi);
}

