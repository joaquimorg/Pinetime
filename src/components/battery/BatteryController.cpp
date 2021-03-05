#include <math.h>
#include "BatteryController.h"
#include <hal/nrf_gpio.h>
#include <libraries/gpiote/app_gpiote.h>
#include <libraries/log/nrf_log.h>
#include <algorithm>
#include "board_config.h"
#include "main.h"

using namespace Pinetime::Controllers;

#define SAMPLES_IN_BUFFER 1
static nrf_saadc_value_t m_buffer_pool[2][SAMPLES_IN_BUFFER];

float voltage = 0.0f;
int8_t percentRemaining = -1;

void Battery::Init() {

  //nrf_gpio_cfg_input(CHARGE_BASE_IRQ, (nrf_gpio_pin_pull_t)GPIO_PIN_CNF_PULL_Pullup);

  //nrfx_gpiote_in_config_t pinConfig;

  // POWER PRESENCE INDICATION IRQ
  nrf_gpio_cfg_sense_input(CHARGE_BASE_IRQ, NRF_GPIO_PIN_NOPULL, NRF_GPIO_PIN_SENSE_LOW);

  static nrfx_gpiote_in_config_t const pinConfig = {
      .sense = NRF_GPIOTE_POLARITY_TOGGLE,
      .pull = NRF_GPIO_PIN_NOPULL,
      .is_watcher = false,
      .hi_accuracy = false,
      .skip_gpio_setup = true,
    };
  
  nrfx_gpiote_in_init(CHARGE_BASE_IRQ, &pinConfig, nrfx_gpiote_evt_handler);
  //

  // CHARGE INDICATION IRQ
  // Need to find how to handle this
  // TO DO
  //nrf_gpio_cfg_input(CHARGE_IRQ, (nrf_gpio_pin_pull_t)GPIO_PIN_CNF_PULL_Pullup);
  /*nrf_gpio_cfg_sense_input(CHARGE_IRQ, (nrf_gpio_pin_pull_t)GPIO_PIN_CNF_PULL_Pullup, (nrf_gpio_pin_sense_t)GPIO_PIN_CNF_SENSE_Low);

  pinConfig.skip_gpio_setup = true;
  pinConfig.hi_accuracy = false;
  pinConfig.is_watcher = false;
  pinConfig.sense = (nrf_gpiote_polarity_t)NRF_GPIOTE_POLARITY_HITOLO;
  pinConfig.pull = (nrf_gpio_pin_pull_t)GPIO_PIN_CNF_PULL_Pullup;

  nrfx_gpiote_in_init(CHARGE_IRQ, &pinConfig, nrfx_gpiote_evt_handler);*/
  //
  
}


void Battery::SaadcInit() {
  nrfx_saadc_config_t adcConfig = NRFX_SAADC_DEFAULT_CONFIG;
  APP_ERROR_CHECK(nrfx_saadc_init(&adcConfig, SaadcEventHandler));

  nrf_saadc_channel_config_t adcChannelConfig = {
          .resistor_p = NRF_SAADC_RESISTOR_DISABLED,
          .resistor_n = NRF_SAADC_RESISTOR_DISABLED,
          .gain       = NRF_SAADC_GAIN1_5,
          .reference  = NRF_SAADC_REFERENCE_INTERNAL,
          .acq_time   = NRF_SAADC_ACQTIME_3US,
          .mode       = NRF_SAADC_MODE_SINGLE_ENDED,
          .burst      = NRF_SAADC_BURST_ENABLED,
          .pin_p      = BATTERY_VOL,
          .pin_n      = NRF_SAADC_INPUT_DISABLED
  };
  APP_ERROR_CHECK(nrfx_saadc_channel_init(0, &adcChannelConfig));

  APP_ERROR_CHECK(nrfx_saadc_buffer_convert(m_buffer_pool[0],SAMPLES_IN_BUFFER));

  APP_ERROR_CHECK(nrfx_saadc_buffer_convert(m_buffer_pool[1],SAMPLES_IN_BUFFER));
}

void Battery::Update() {

  //isCharging = 1;//!nrf_gpio_pin_read(CHARGE_IRQ);
  isCharging = nrf_gpio_pin_read(CHARGE_BASE_IRQ);
  
  //isPowerPresent = !nrf_gpio_pin_read(CHARGE_BASE_IRQ);

  // Non blocking read
  SaadcInit();
  
  APP_ERROR_CHECK(nrfx_saadc_sample());
  
}

void Battery::SaadcEventHandler(nrfx_saadc_evt_t const * p_event) {

  
  int avg_sample = 0;
  int i;

  const float battery_max = 4.05; //maximum voltage of battery
  const float battery_min = 3.00;  //minimum voltage of battery before shutdown

  if (p_event->type == NRFX_SAADC_EVT_DONE) {
    
    APP_ERROR_CHECK(nrfx_saadc_buffer_convert(p_event->data.done.p_buffer, SAMPLES_IN_BUFFER));

    for (i = 0; i < SAMPLES_IN_BUFFER; i++) {
      avg_sample += p_event->data.done.p_buffer[i]; // take N samples in a row
    }
    avg_sample /= i; // average all the samples out

    voltage = (avg_sample * 2.0f) / (1024 / 3.0f);
   
    voltage = roundf(voltage * 100) / 100;

    percentRemaining = ((voltage - battery_min) / (battery_max - battery_min)) * 100;

    if (percentRemaining > 100) {
        percentRemaining = 100;    
    }
    if (percentRemaining < 0) {
        percentRemaining = 0;    
    }

    nrfx_saadc_uninit();

  } else {
    percentRemaining = -1;
  }
}


int8_t Battery::PercentRemaining() {
  return percentRemaining;
}

float Battery::Voltage() {
  return voltage;
}
