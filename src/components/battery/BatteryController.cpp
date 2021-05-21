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
	nrf_gpio_cfg_input(CHARGE_IRQ, NRF_GPIO_PIN_NOPULL);

	if ( nrf_gpio_pin_read(CHARGE_IRQ) ) {
		nrf_gpio_cfg_sense_input(CHARGE_IRQ, NRF_GPIO_PIN_NOPULL, NRF_GPIO_PIN_SENSE_LOW);
	} else {
		nrf_gpio_cfg_sense_input(CHARGE_IRQ, NRF_GPIO_PIN_NOPULL, NRF_GPIO_PIN_SENSE_HIGH);
	}
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
	
	if ( isReading ) return;

	SaadcInit();
	samples = 0;
	isReading = true;
	nrfx_saadc_sample();
}

void Battery::SaadcInit() {
	nrfx_saadc_config_t adcConfig = NRFX_SAADC_DEFAULT_CONFIG;
	APP_ERROR_CHECK(nrfx_saadc_init(&adcConfig, adcCallbackStatic));

  	nrf_saadc_channel_config_t adcChannelConfig = {
		.resistor_p = NRF_SAADC_RESISTOR_DISABLED,
		.resistor_n = NRF_SAADC_RESISTOR_DISABLED,
		.gain       = NRF_SAADC_GAIN1_6,
		.reference  = NRF_SAADC_REFERENCE_INTERNAL,
		.acq_time   = NRF_SAADC_ACQTIME_20US,
		.mode       = NRF_SAADC_MODE_SINGLE_ENDED,
		.burst      = NRF_SAADC_BURST_ENABLED,
		.pin_p      = BATTERY_VOL,
		.pin_n      = NRF_SAADC_INPUT_DISABLED
  	};
  	APP_ERROR_CHECK(nrfx_saadc_channel_init(0, &adcChannelConfig));
	APP_ERROR_CHECK(nrfx_saadc_buffer_convert(&adc_buf[0], 1));
    APP_ERROR_CHECK(nrfx_saadc_buffer_convert(&adc_buf[1], 1));

}

/**
 * Symmetric sigmoidal approximation
 * https://www.desmos.com/calculator/7m9lu26vpy
 *
 * c - c / (1 + k*x/v)^3
 */
uint8_t Battery::sigmoidal(uint16_t voltage, uint16_t minVoltage, uint16_t maxVoltage) {
	// slow
	// uint8_t result = 110 - (110 / (1 + pow(1.468 * (voltage - minVoltage)/(maxVoltage - minVoltage), 6)));

	// steep
	// uint8_t result = 102 - (102 / (1 + pow(1.621 * (voltage - minVoltage)/(maxVoltage - minVoltage), 8.1)));

	// normal
	uint8_t result = 105 - (105 / (1 + pow(1.724 * (voltage - minVoltage)/(maxVoltage - minVoltage), 5.5)));
	return result >= 100 ? 100 : result;
}

void Battery::SaadcEventHandler(nrfx_saadc_evt_t const * p_event) {

	const uint16_t battery_max = 4150; // maximum voltage of battery ( max charging voltage is 4.21 )
	const uint16_t battery_min = 3200; // minimum voltage of battery before shutdown ( depends on the battery )

	const uint16_t VIN_MEAS_R26 = 1000 + 50; // 1000kOhm +- 5% + error correction
	const uint16_t VIN_MEAS_R35 = 1000; // 1000kOhm +- 5%

	if (p_event->type == NRFX_SAADC_EVT_DONE) {

		nrf_saadc_value_t adc_result;

		adc_result = p_event->data.done.p_buffer[0];

		APP_ERROR_CHECK(nrfx_saadc_buffer_convert(p_event->data.done.p_buffer, 1));

		// Voltage divider ratio
		//(R26 + R35) / R35
		auto value = (VIN_MEAS_R26 + VIN_MEAS_R35) * adc_result / VIN_MEAS_R35;		

		voltageBuffer.insert(value);

		// ADC * 0.6V reference / 10-bit ADC * prescale (1/6)
		auto miliVolts = ((voltageBuffer.GetAverage() * 600) / 1024) * 6;

		voltage = (float)miliVolts / 1000;

		if (miliVolts <= battery_min) {
			percentRemaining = 0;
		} else if (miliVolts >= battery_max) {
			percentRemaining = 100;
		} else {
			percentRemaining = sigmoidal(miliVolts, battery_min, battery_max);
			percentRemaining = std::max(percentRemaining, 0);
			percentRemaining = std::min(percentRemaining, 100);
		}
		percentRemainingBuffer.insert(percentRemaining);

		samples++;
		if ( samples > percentSamples ) {
      		nrfx_saadc_uninit();
			isReading = false;
		} else {
			nrfx_saadc_sample();
		}
    }
}

void Battery::adcCallbackStatic(nrfx_saadc_evt_t const *event) {
    instance->SaadcEventHandler(event);
}