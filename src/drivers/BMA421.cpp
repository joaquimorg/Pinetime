#include "BMA421.h"
#include <FreeRTOS.h>
#include <legacy/nrf_drv_gpiote.h>
#include <nrfx_log.h>
#include <task.h>
#include "board_config.h"

using namespace Pinetime::Drivers;


/* Earth's gravity in m/s^2 */
#define GRAVITY_EARTH       (9.80665f)


int8_t user_i2c_read(uint8_t reg_addr, uint8_t *reg_data, uint32_t length, void *intf_ptr)
{
    TwiMaster *twiMaster = (TwiMaster *)intf_ptr;
    twiMaster->Read(BMA421_TWI_ADDR, reg_addr, reg_data, length);
    return 0;
}


int8_t user_i2c_write(uint8_t reg_addr, const uint8_t *reg_data, uint32_t length, void *intf_ptr)
{
    TwiMaster *twiMaster = (TwiMaster *)intf_ptr;
    twiMaster->Write(BMA421_TWI_ADDR, reg_addr, reg_data, length);
    return 0;
}

void user_delay(uint32_t period_us, void *intf_ptr)
{
    //TwiMaster *twiMaster = (TwiMaster *)intf_ptr;
    //uint32_t i;
    //for(i = 0; i < (period_us * 64); i++) {}
}

/*!
 *  @brief Prints the execution status of the APIs.
 */
void bma4_error_codes_print_result(const char api_name[], uint16_t rslt)
{
    if (rslt != BMA4_OK)
    {
        //printf("%s\t", api_name);
        NRF_LOG_INFO("[BMA421] %s\t", api_name);
        if (rslt & BMA4_E_NULL_PTR)
        {
            //printf("Error [%d] : Null pointer\r\n", rslt);
            NRF_LOG_INFO("[BMA421] Error [%d] : Null pointer\r\n", rslt);
        }
        else if (rslt & BMA4_E_CONFIG_STREAM_ERROR)
        {
            //printf("Error [%d] : Invalid configuration stream\r\n", rslt);
            NRF_LOG_INFO("[BMA421] Error [%d] : Invalid configuration stream\r\n", rslt);
        }
        else if (rslt & BMA4_E_SELF_TEST_FAIL)
        {
            //printf("Error [%d] : Self test failed\r\n", rslt);
            NRF_LOG_INFO("[BMA421] Error [%d] : Self test failed\r\n", rslt);
        }
        else if (rslt & BMA4_E_INVALID_SENSOR)
        {
            //printf("Error [%d] : Device not found\r\n", rslt);
            NRF_LOG_INFO("[BMA421] Error [%d] : Device not found\r\n", rslt);
        }
        else
        {
            /* For more error codes refer "*_defs.h" */
            //printf("Error [%d] : Unknown error code\r\n", rslt);
            NRF_LOG_INFO("[BMA421] Error [%d] : Unknown error code\r\n", rslt);
        }
    }
}


BMA421::BMA421(TwiMaster &twiMaster) : twiMaster{twiMaster} {}


void BMA421::Init() {
    
    uint16_t rslt;

    struct bma4_int_pin_config pinConfig;

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
    bma4_error_codes_print_result("bma421_init", rslt);
    vTaskDelay(50);

    /* Soft reset */
    rslt = bma4_soft_reset(&bma);
    bma4_error_codes_print_result("bma4_soft_reset", rslt);
    vTaskDelay(50);

    /* Upload the configuration file to enable the features of the sensor. */
    rslt = bma421_write_config_file(&bma);
    bma4_error_codes_print_result("bma421_write_config", rslt);    

    /* Sets the electrical behaviour of interrupt
    */
    pinConfig.edge_ctrl = BMA4_LEVEL_TRIGGER;
    pinConfig.lvl = BMA4_ACTIVE_LOW;
    pinConfig.od = BMA4_OPEN_DRAIN;
    pinConfig.output_en = BMA4_OUTPUT_ENABLE;
    pinConfig.input_en = BMA4_INPUT_DISABLE;
    rslt = bma4_set_int_pin_config(&pinConfig, BMA4_INTR1_MAP, &bma);
    bma4_error_codes_print_result("bma4_set_int_pin_config status", rslt);  

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
    accel_conf.perf_mode = BMA4_CONTINUOUS_MODE;

    /* Set the accel configurations */
    rslt = bma4_set_accel_config(&accel_conf, &bma);
    bma4_error_codes_print_result("bma4_set_accel_config status", rslt);

    /* Enable the accelerometer */
    rslt = bma4_set_accel_enable(BMA4_ENABLE, &bma);
    bma4_error_codes_print_result("bma4_set_accel_enable status", rslt);

    /* Enable step counter */
    rslt = bma421_feature_enable(BMA421_STEP_CNTR, 1, &bma);
    bma4_error_codes_print_result("bma421_feature_enable status", rslt);

    rslt = bma421_step_detector_enable(BMA4_ENABLE, &bma);
    bma4_error_codes_print_result("bma421_step_detector_enable status", rslt);

    /* Set water-mark level 1 to get interrupt after 20 steps.
     * Range of step counter interrupt is 0 to 20460(resolution of 20 steps).
     */
    rslt = bma421_step_counter_set_watermark(1, &bma);
    bma4_error_codes_print_result("bma421_step_counter status", rslt);

    /* Set the interrupt mode in the sensor.
    */
    //rslt = bma4_set_interrupt_mode(BMA4_LATCH_MODE, &bma);
    //bma4_error_codes_print_result("bma4_set_interrupt_mode status", rslt);


    // pinetime has 90° rotated Accl
    struct bma421_axes_remap remap_data;
    remap_data.x_axis = 0;
    remap_data.x_axis_sign = 1;
    remap_data.y_axis = 1;
    remap_data.y_axis_sign = 1;
    remap_data.z_axis  = 2;
    remap_data.z_axis_sign  = 0;

    rslt = bma421_set_remap_axes(&remap_data, &bma);
    bma4_error_codes_print_result("bma421_set_remap_axes status", rslt);

    /* Interrupt Mapping
     */
    //rslt = bma421_map_interrupt(BMA4_INTR1_MAP, (BMA421_STEP_CNTR_INT | BMA421_ACTIVITY_INT | BMA421_DOUBLE_TAP_INT | BMA421_WRIST_WEAR_INT), 1, &bma);
    rslt = bma421_map_interrupt(BMA4_INTR1_MAP, BMA421_STEP_CNTR_INT, BMA4_ENABLE, &bma);
    bma4_error_codes_print_result("bma421_map_interrupt status", rslt);


}

/*! @brief Converts raw sensor values(LSB) to meters per seconds square.
 *
 *  @param[in] val      : Raw sensor value.
 *  @param[in] g_range  : Accel Range selected (2G, 4G, 8G, 16G).
 *  @param[in] bit_width    : Resolution of the sensor.
 *
 *  @return Accel values in meters per second square.
 *
 */
static float lsb_to_ms2(int16_t val, float g_range, uint8_t bit_width)
{
    float half_scale = (float)(1 << bit_width) / 2.0f;

    return GRAVITY_EARTH * val * g_range / half_scale;
}

void BMA421::Wakeup() {
    uint8_t rslt;
    /* Sets the advance power save mode */
    rslt = bma4_set_advance_power_save(BMA4_DISABLE, &bma);
    bma4_error_codes_print_result("bma4_set_advance_power_save", rslt);
}

void BMA421::Sleep() {
    uint8_t rslt;
    /* Sets the advance power save mode */
    rslt = bma4_set_advance_power_save(0x03, &bma);
    bma4_error_codes_print_result("bma4_set_advance_power_save", rslt);
}

void BMA421::Update() {
    uint16_t rslt;

    uint32_t step_out = 0;

    uint8_t activity_out = 0;

    int32_t get_temp_C = 0;

    struct bma4_accel sens_data;

    /* Variable to get the interrupt status */
    uint16_t int_status = 0;

    /* Get temperature in degree C */
    rslt = bma4_get_temperature(&get_temp_C, BMA4_DEG, &bma);
    bma4_error_codes_print_result("bma4_get_temperature in degree C status", rslt);

    tempC = (float)get_temp_C / (float)BMA4_SCALE_TEMP;

    /* Read the accel data */
    rslt = bma4_read_accel_xyz(&sens_data, &bma);

    /* Converting lsb to meters per seconds square for 12 bit accelerometer at 2G range */    
    /*accelData.x = lsb_to_ms2(sens_data.x, 2, bma.resolution);
    accelData.y = lsb_to_ms2(sens_data.y, 2, bma.resolution);
    accelData.z = lsb_to_ms2(sens_data.z, 2, bma.resolution);*/

    accelData.x = (sens_data.x / 0x10);
    accelData.y = (sens_data.y / 0x10);
    accelData.z = (sens_data.z / 0x10);

    //accelData = sens_data;

    /* Read the interrupt register */
    rslt = bma421_read_int_status(&int_status, &bma);
    bma4_error_codes_print_result("bma421_read_int_status status", rslt);    

    /* Check if step counter interrupt is triggered */
    //if (int_status & BMA421_STEP_CNTR_INT)
    //{
        rslt = bma421_step_counter_output(&step_out, &bma);
        bma4_error_codes_print_result("bma421_step_counter_output status", rslt);

        step_count = step_out;
    //}

    /* An interrupt is set if any activity is recognized  */
    //if (int_status & BMA421_ACTIVITY_INT)
    //{
        /* Read activity output register for recognizing specific activity */
        rslt = bma421_activity_output(&activity_out, &bma);
        bma4_error_codes_print_result("bma421_activity_output status", rslt);
        switch (activity_out)
        {
            case BMA421_USER_STATIONARY:
                //printf("User is stationary\n");
                activity = 1;
                break;
            case BMA421_USER_WALKING:
                //printf("User is walking\n");
                activity = 2;
                break;
            case BMA421_USER_RUNNING:
                //printf("User is running\n");
                activity = 3;
                break;
            case BMA421_STATE_INVALID:
                //printf("Invalid activity recognized\n");
                activity = 0;
                break;
        }

    //}

    if (int_status & BMA421_SINGLE_TAP_INT)
    {
        //printf("Single tap received\n");
        tapStatus = 1;
    }
    else if (int_status & BMA421_DOUBLE_TAP_INT)
    {
        //printf("Double tap received\n");
        tapStatus = 2;
    } else if (int_status & BMA421_WRIST_WEAR_INT)
    {
        //printf("WRIST Tilt\n");
        tapStatus = 3;
    } else {
        tapStatus = int_status;
    }

}
