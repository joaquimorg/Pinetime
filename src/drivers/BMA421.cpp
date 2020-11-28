#include "BMA421.h"
#include <FreeRTOS.h>
#include <legacy/nrf_drv_gpiote.h>
#include <nrfx_log.h>
#include <task.h>
#include "board_config.h"

using namespace Pinetime::Drivers;

int8_t user_i2c_read(uint8_t reg_addr, uint8_t *reg_data, uint32_t length, void *intf_ptr)
{

    TwiMaster *twiMaster = (TwiMaster *)intf_ptr;
    twiMaster->Read(BMA421_TWI_ADDR, reg_addr, reg_data, length);
    return 0;
}


int8_t user_i2c_write(uint8_t reg_addr, const uint8_t *reg_data, uint32_t length, void *intf_ptr)
{

    TwiMaster *twiMaster = (TwiMaster *)intf_ptr;
    twiMaster->Write(BMA421_TWI_ADDR, 0xA5, reg_data, length);
    return 0;
}

void user_delay(uint32_t period_us, void *intf_ptr)
{
    TwiMaster *twiMaster = (TwiMaster *)intf_ptr;
}



BMA421::BMA421(TwiMaster &twiMaster) : twiMaster{twiMaster} {}

void BMA421::Init() {
    
    uint16_t rslt;

    bma.intf = BMA4_I2C_INTF;
    //dev_addr = BMA4_I2C_ADDR_PRIMARY;
    bma.bus_read = user_i2c_read;
    bma.bus_write = user_i2c_write;
    bma.variant = BMA42X_VARIANT;
    bma.intf_ptr = &twiMaster;
    bma.delay_us = user_delay;
    bma.read_write_len = 8;

    /* Sensor initialization */
    rslt = bma421_init(&bma);

    /* Enable the accelerometer */
    rslt = bma4_set_accel_enable(1, &bma);

    /* Accelerometer Configuration Setting */
    /* Output data Rate */
    accel_conf.odr = BMA4_OUTPUT_DATA_RATE_100HZ;

    /* Gravity range of the sensor (+/- 2G, 4G, 8G, 16G) */
    accel_conf.range = BMA4_ACCEL_RANGE_2G;

    /* Bandwidth configure number of sensor samples required to average
     * if value = 2, then 4 samples are averaged
     * averaged samples = 2^(val(accel bandwidth))
     * Note1 : More info refer datasheets
     * Note2 : A higher number of averaged samples will result in a lower noise level of the signal, but since the
     * performance power mode phase is increased, the power consumption will also rise.
     */
    accel_conf.bandwidth = BMA4_ACCEL_NORMAL_AVG4;

    /* Enable the filter performance mode where averaging of samples
     * will be done based on above set bandwidth and ODR.
     * There are two modes
     *  0 -> Averaging samples (Default)
     *  1 -> No averaging
     * For more info on No Averaging mode refer datasheets.
     */
    accel_conf.perf_mode = BMA4_CIC_AVG_MODE;

    /* Set the accel configurations */
    rslt = bma4_set_accel_config(&accel_conf, &bma);

    /* Enable step counter */
    rslt = bma421_feature_enable(BMA421_STEP_CNTR, 1, &bma);

    /* Map the interrupt pin with that of step counter interrupts
     * Interrupt will  be generated when step activity is generated.
     */
    rslt = bma421_map_interrupt(BMA4_INTR1_MAP, BMA421_STEP_CNTR_INT, 1, &bma);

    /* Set water-mark level 1 to get interrupt after 20 steps.
     * Range of step counter interrupt is 0 to 20460(resolution of 20 steps).
     */
    rslt = bma421_step_counter_set_watermark(1, &bma);


}


void BMA421::Update() {
    uint16_t rslt;

    /* Variable to get the step counter output */
    uint32_t step_out = 0;

    rslt = bma421_step_counter_output(&step_out, &bma);

    step_count = step_out;

}
