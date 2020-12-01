#include "Vibration.h"
#include <hal/nrf_gpio.h>
#include "board_config.h"
#include "systemtask/SystemTask.h"

using namespace Pinetime::Drivers;


void Vibration::Init() {
    nrf_gpio_cfg_output(VIBRATOR_CTRL);
    nrf_gpio_pin_set(VIBRATOR_CTRL);

    Vibrate(25);
}

void Vibration::Vibrate(uint8_t durationMs) {
    nrf_gpio_pin_clear(VIBRATOR_CTRL);
    vTaskDelay(durationMs);
    nrf_gpio_pin_set(VIBRATOR_CTRL);
}