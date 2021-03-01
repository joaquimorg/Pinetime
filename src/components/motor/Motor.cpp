#include "Motor.h"
#include <hal/nrf_gpio.h>
#include "board_config.h"
#include "main.h"

using namespace Pinetime::Controllers;

namespace {
    void vrTimerCallback(TimerHandle_t xTimer) {

    auto motor = static_cast<Motor *>(pvTimerGetTimerID(xTimer));
    motor->EndVibration();
    }
}

Motor::Motor( Controllers::Settings &settingsController ) :
    settingsController{settingsController} {}

void Motor::Init() {
    nrf_gpio_cfg_output(VIBRATOR_CTRL);
    nrf_gpio_pin_set(VIBRATOR_CTRL);

    Vibrate(25);
}

void Motor::Vibrate(uint8_t durationMs) {

    if ( settingsController.GetVibrationStatus() == Controllers::Settings::Vibration::OFF ) return;

    nrf_gpio_pin_clear(VIBRATOR_CTRL);    

    vrTimer = xTimerCreate ("vrTimer", pdMS_TO_TICKS( durationMs ), pdFALSE, this, vrTimerCallback);
    xTimerStart(vrTimer, 0);
    
}


void Motor::EndVibration() {
    xTimerStop(vrTimer, 0);
    nrf_gpio_pin_set(VIBRATOR_CTRL);
}