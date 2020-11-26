#include "BrightnessController.h"
#include <hal/nrf_gpio.h>
#include "board_config.h"

using namespace Pinetime::Controllers;


void BrightnessController::Init() {
  nrf_gpio_cfg_output(LCD_LIGHT_1);
  nrf_gpio_cfg_output(LCD_LIGHT_2);
  nrf_gpio_cfg_output(LCD_LIGHT_3);
  Set(level);
}

void BrightnessController::Set(BrightnessController::Levels level) {
  this->level = level;
  switch(level) {
    default:
    case Levels::High:
      nrf_gpio_pin_clear(LCD_LIGHT_1);
      nrf_gpio_pin_clear(LCD_LIGHT_2);
      nrf_gpio_pin_clear(LCD_LIGHT_3);
      break;
    case Levels::Medium:
      nrf_gpio_pin_clear(LCD_LIGHT_1);
      nrf_gpio_pin_clear(LCD_LIGHT_2);
      nrf_gpio_pin_set(LCD_LIGHT_3);
      break;
    case Levels::Low:
      nrf_gpio_pin_clear(LCD_LIGHT_1);
      nrf_gpio_pin_set(LCD_LIGHT_2);
      nrf_gpio_pin_set(LCD_LIGHT_3);
      break;
    case Levels::Off:
      nrf_gpio_pin_set(LCD_LIGHT_1);
      nrf_gpio_pin_set(LCD_LIGHT_2);
      nrf_gpio_pin_set(LCD_LIGHT_3);
      break;
  }
}

void BrightnessController::Lower() {
  switch(level) {
    case Levels::High: Set(Levels::Medium); break;
    case Levels::Medium: Set(Levels::Low); break;
    case Levels::Low: Set(Levels::Off); break;
    default: break;
  }
}

void BrightnessController::Higher() {
  switch(level) {
    case Levels::Off: Set(Levels::Low); break;
    case Levels::Low: Set(Levels::Medium); break;
    case Levels::Medium: Set(Levels::High); break;
    default: break;
  }
}

BrightnessController::Levels BrightnessController::Level() const {
  return level;
}

void BrightnessController::Backup() {
  backupLevel = level;
}

void BrightnessController::Restore() {
  Set(backupLevel);
}

