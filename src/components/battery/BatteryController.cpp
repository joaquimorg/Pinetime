#include <algorithm>
#include <math.h>
#include "BatteryController.h"
#include <hal/nrf_gpio.h>
#include <libraries/gpiote/app_gpiote.h>
#include <nrfx_saadc.h>
#include <libraries/log/nrf_log.h>
#include "board_config.h"
#include "main.h"

using namespace Pinetime::Controllers;

Battery *Battery::instance = nullptr;

Battery::Battery() {
	instance = this;
}

void Battery::Init() {

	nrf_gpio_cfg_input(CHARGE_BASE_IRQ, NRF_GPIO_PIN_NOPULL);
	//nrf_gpio_cfg_input(CHARGE_IRQ, (nrf_gpio_pin_pull_t)GPIO_PIN_CNF_PULL_Pullup);

	// POWER PRESENCE INDICATION IRQ
	//nrf_gpio_cfg_sense_input(CHARGE_BASE_IRQ, NRF_GPIO_PIN_NOPULL, NRF_GPIO_PIN_SENSE_LOW);

	/*static nrfx_gpiote_in_config_t const pinConfigBase = {
		.sense = NRF_GPIOTE_POLARITY_TOGGLE,
		.pull = NRF_GPIO_PIN_NOPULL,
		.is_watcher = false,
		.hi_accuracy = false,
		.skip_gpio_setup = false,
		};
	
	nrfx_gpiote_in_init(CHARGE_BASE_IRQ, &pinConfigBase, nrfx_gpiote_evt_handler);*/

	//

	// CHARGE INDICATION IRQ
	//nrf_gpio_cfg_input(CHARGE_IRQ, NRF_GPIO_PIN_NOPULL);	
	
	nrf_gpio_cfg_sense_input(CHARGE_IRQ, NRF_GPIO_PIN_NOPULL, NRF_GPIO_PIN_SENSE_LOW);
	static nrfx_gpiote_in_config_t const pinConfigCharge = {
		.sense = NRF_GPIOTE_POLARITY_TOGGLE,
		.pull = NRF_GPIO_PIN_NOPULL,
		.is_watcher = false,
		.hi_accuracy = false,
		.skip_gpio_setup = true,
		};
	
	nrfx_gpiote_in_init(CHARGE_IRQ, &pinConfigCharge, nrfx_gpiote_evt_handler);	
		
}

void Battery::Update() {

  	isCharging = !nrf_gpio_pin_read(CHARGE_IRQ);
  	isPowerPresent = !nrf_gpio_pin_read(CHARGE_BASE_IRQ);	
	SaadcInit();
	nrfx_saadc_sample();
}

void Battery::SaadcInit() {
	nrfx_saadc_config_t adcConfig = NRFX_SAADC_DEFAULT_CONFIG;
	APP_ERROR_CHECK(nrfx_saadc_init(&adcConfig, adcCallbackStatic));

  	nrf_saadc_channel_config_t adcChannelConfig = {
		.resistor_p = NRF_SAADC_RESISTOR_DISABLED,
		.resistor_n = NRF_SAADC_RESISTOR_DISABLED,
		.gain       = NRF_SAADC_GAIN1_5,
		.reference  = NRF_SAADC_REFERENCE_INTERNAL,
		.acq_time   = NRF_SAADC_ACQTIME_5US,
		.mode       = NRF_SAADC_MODE_SINGLE_ENDED,
		.burst      = NRF_SAADC_BURST_ENABLED,
		.pin_p      = BATTERY_VOL,
		.pin_n      = NRF_SAADC_INPUT_DISABLED
  	};
  	APP_ERROR_CHECK(nrfx_saadc_channel_init(0, &adcChannelConfig));
	APP_ERROR_CHECK(nrfx_saadc_buffer_convert(m_buffer_pool[0], 1));
    APP_ERROR_CHECK(nrfx_saadc_buffer_convert(m_buffer_pool[1], 1));

}

void Battery::SaadcEventHandler(nrfx_saadc_evt_t const * p_event) {

	const float battery_max = 4.18; // maximum voltage of battery ( max charging voltage is 4.21 )
	const float battery_min = 3.20; // minimum voltage of battery before shutdown ( depends on the battery )

	if (p_event->type == NRFX_SAADC_EVT_DONE) {
	
		APP_ERROR_CHECK(nrfx_saadc_buffer_convert(p_event->data.done.p_buffer, 1));

		int i, batt_sum=0;

		for (i = 0; i < 1; i++) {
			batt_sum+=p_event->data.done.p_buffer[i];		
		}
		batt_sum /= 1;

      	voltage = (batt_sum * 2.04f) / (1024 / 3.0f);
      	voltage = roundf(voltage * 100) / 100;		
		
      	percentRemaining = static_cast<int>(((voltage - battery_min) / (battery_max - battery_min)) * 100);

      	percentRemaining = std::max(percentRemaining, 0);
      	percentRemaining = std::min(percentRemaining, 100);

		percentRemainingBuffer.insert(percentRemaining);

      	nrfx_saadc_uninit();
    }
}

void Battery::adcCallbackStatic(nrfx_saadc_evt_t const *event) {
    instance->SaadcEventHandler(event);
}