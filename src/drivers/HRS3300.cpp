#include "HRS3300.h"
#include <FreeRTOS.h>
#include <legacy/nrf_drv_gpiote.h>
#include <nrfx_log.h>
#include <task.h>
#include "board_config.h"

using namespace Pinetime::Drivers;

HRS3300::HRS3300(TwiMaster &twiMaster, uint8_t twiAddress) : twiMaster{twiMaster}, twiAddress{twiAddress} {}

void HRS3300::Init() {
}