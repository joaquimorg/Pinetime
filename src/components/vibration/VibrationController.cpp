#include "VibrationController.h"
#include <hal/nrf_gpio.h>
#include "board_config.h"
#include "systemtask/SystemTask.h"

using namespace Pinetime::Controllers;


void VibrationController::Init() {
    nrf_gpio_cfg_output(VIBRATOR_CTRL);
    nrf_gpio_pin_clear(VIBRATOR_CTRL);

    Vibrate(100);
}

void VibrationController::Vibrate(uint8_t durationMs) {
    nrf_gpio_pin_set(VIBRATOR_CTRL);
    vTaskDelay(durationMs);
    nrf_gpio_pin_clear(VIBRATOR_CTRL);
}