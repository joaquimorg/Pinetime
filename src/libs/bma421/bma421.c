/**
 * Copyright (c) 2020 Bosch Sensortec GmbH. All rights reserved.
 *
 * BSD-3-Clause
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * @file       bma421.c
 * @date       2020-05-08
 * @version    V2.14.13
 *
 */

/*! \file bma421.c
 * \brief Sensor Driver for BMA421 sensor
 */

#include "bma421.h"

#include "bma421_config.h"

/***************************************************************************/

/*! Static Function Declarations
 ****************************************************************************/

/*!
 * @brief This API enables the features of sensor.
 *
 * @param[in] feature : Variable to specify the features which are to be set
 * in the sensor.
 * @param[in] len : Length for read and write
 * @param[in] feature_config : Array address which stores the feature
 * configuration data
 * @param[in] dev : Structure instance of bma4_dev.
 *
 *  @return Result of API execution status
 *  @retval 0 -> Success
 *  @retval < 0 -> Fail
 */
static int8_t feature_enable(uint8_t feature, uint8_t len, uint8_t *feature_config, struct bma4_dev *dev);

/*!
 * @brief This API disables the features of sensor.
 *
 * @param[in] feature : Variable to specify the features which are to be unset
 * in the sensor.
 * @param[in] len : Length for read and write
 * @param[in] feature_config : Array address which stores the feature
 * configuration data
 * @param[in] dev : Structure instance of bma4_dev.
 *
 *  @return Result of API execution status
 *  @retval 0 -> Success
 *  @retval < 0 -> Fail
 */
static int8_t feature_disable(uint8_t feature, uint8_t len, uint8_t *feature_config, struct bma4_dev *dev);

/*!
 * @brief This API update the settings of step counter into write array.
 *
 * @param[in] setting : Pointer to structure variable which stores the
 * settings parameter1 to parameter25.
 * @param[in] index : value for array traversing.
 * @param[out] feature_config   : Pointer to store the settings
 *
 * @return none
 */
static void update_stepcounter_parameter(const struct bma421_stepcounter_settings *setting,
                                         uint8_t index,
                                         uint8_t *feature_config);

/*!
 *  @brief This API copy the settings of step counter into the
 *  structure of bma421_stepcounter_settings, which is read from sensor.
 *
 * @param[out] setting : Pointer to structure variable which stores the
 * settings parameter1 to parameter25 read from sensor.
 * @param[in] data_p : Pointer of array which stores the parameters.
 *
 * @return none
 */
static void extract_stepcounter_parameter(struct bma421_stepcounter_settings *setting, const uint16_t *data_p);

/***************************************************************************/

/**\name        Function definitions
 ****************************************************************************/

/*!
 * @brief This API is the entry point.
 * Call this API before using all other APIs.
 * This API reads the chip-id of the sensor and sets the resolution.
 */
int8_t bma421_init(struct bma4_dev *dev)
{
    int8_t rslt;

    rslt = bma4_init(dev);
    if (rslt == BMA4_OK)
    {
        if (dev->chip_id == BMA421_CHIP_ID)
        {
            /* Resolution of BMA421 sensor is 12 bit */
            dev->resolution = 12;

            dev->feature_len = BMA421_FEATURE_SIZE;

            dev->config_size = sizeof(bma421_config_file);
        }
        else
        {
            rslt = BMA4_E_INVALID_SENSOR;
        }
    }

    return rslt;
}

/*!
 * @brief This API is used to upload the configuration file to enable the
 * features of the sensor.
 */
int8_t bma421_write_config_file(struct bma4_dev *dev)
{
    int8_t rslt = BMA4_OK;

    if (dev != NULL)
    {
        if (dev->chip_id == BMA421_CHIP_ID)
        {
            /* Configuration stream read/write length boundary
             * check
             */
            if ((dev->read_write_len >= BMA421_RD_WR_MIN_LEN) && (dev->read_write_len <= BMA421_FEATURE_SIZE))
            {
                /* Even or odd check */
                if ((dev->read_write_len % 2) != 0)
                {
                    dev->read_write_len = dev->read_write_len - 1;
                }

                /*Assign stream data */
                dev->config_file_ptr = bma421_config_file;
                rslt = bma4_write_config_file(dev);
            }
            else
            {
                rslt = BMA4_E_RD_WR_LENGTH_INVALID;
            }
        }
        else
        {
            rslt = BMA4_E_INVALID_SENSOR;
        }
    }
    else
    {
        rslt = BMA4_E_NULL_PTR;
    }

    return rslt;
}

/*!
 * @brief This API is used to get the configuration id of the sensor.
 */
int8_t bma421_get_config_id(uint16_t *config_id, struct bma4_dev *dev)
{
    uint8_t feature_config[BMA421_FEATURE_SIZE] = { 0 };
    uint8_t index = BMA421_CONFIG_ID_OFFSET;
    int8_t rslt = BMA4_OK;
    uint16_t config_id_lsb = 0;
    uint16_t config_id_msb = 0;

    if (dev != NULL)
    {
        if (dev->chip_id == BMA421_CHIP_ID)
        {
            rslt = bma4_read_regs(BMA4_FEATURE_CONFIG_ADDR, feature_config, BMA421_FEATURE_SIZE, dev);
            if (rslt == BMA4_OK)
            {
                config_id_lsb = (uint16_t)feature_config[index];
                config_id_msb = ((uint16_t)feature_config[index + 1]) << 8;
                *config_id = config_id_lsb | config_id_msb;
            }
        }
        else
        {
            rslt = BMA4_E_INVALID_SENSOR;
        }
    }
    else
    {
        rslt = BMA4_E_NULL_PTR;
    }

    return rslt;
}

/*!
 * @brief This API sets/un-sets the user provided interrupt to either interrupt
 * pin1 or pin2 in the sensor.
 */
int8_t bma421_map_interrupt(uint8_t int_line, uint16_t int_map, uint8_t enable, struct bma4_dev *dev)
{
    int8_t rslt = BMA4_OK;

    if (dev != NULL)
    {
        if (dev->chip_id == BMA421_CHIP_ID)
        {
            if (int_line <= 1)
            {
                /* Map/Unmap the interrupt */
                rslt = bma4_map_interrupt(int_line, int_map, enable, dev);
            }
            else
            {
                rslt = BMA4_E_INT_LINE_INVALID;
            }
        }
        else
        {
            rslt = BMA4_E_INVALID_SENSOR;
        }
    }
    else
    {
        rslt = BMA4_E_NULL_PTR;
    }

    return rslt;
}

/*!
 * @brief This API reads the bma421 interrupt status from the sensor.
 */
int8_t bma421_read_int_status(uint16_t *int_status, struct bma4_dev *dev)
{
    int8_t rslt = BMA4_OK;

    if (dev != NULL)
    {
        if (dev->chip_id == BMA421_CHIP_ID)
        {
            /* Read the interrupt status */
            rslt = bma4_read_int_status(int_status, dev);
        }
        else
        {
            rslt = BMA4_E_INVALID_SENSOR;
        }
    }
    else
    {
        rslt = BMA4_E_NULL_PTR;
    }

    return rslt;
}

/*!
 * @brief This API enables/disables the features of the sensor.
 */
int8_t bma421_feature_enable(uint8_t feature, uint8_t enable, struct bma4_dev *dev)
{
    uint8_t feature_config[BMA421_FEATURE_SIZE] = { 0 };
    int8_t rslt = BMA4_OK;
    uint8_t len = BMA421_FEATURE_SIZE;

    if (dev != NULL)
    {
        if (dev->chip_id == BMA421_CHIP_ID)
        {
            /* Read feature configuration data */
            rslt = bma4_read_regs(BMA4_FEATURE_CONFIG_ADDR, feature_config, len, dev);
            if (rslt == BMA4_OK)
            {
                if (enable == TRUE)
                {
                    /* Enables the feature */
                    rslt = feature_enable(feature, len, feature_config, dev);
                }
                else
                {
                    /* Disables the feature */
                    rslt = feature_disable(feature, len, feature_config, dev);
                }
            }
        }
        else
        {
            rslt = BMA4_E_INVALID_SENSOR;
        }
    }
    else
    {
        rslt = BMA4_E_NULL_PTR;
    }

    return rslt;
}

/*!
 * @brief This API performs x, y and z axis remapping in the sensor.
 */
int8_t bma421_set_remap_axes(const struct bma421_axes_remap *remap_data, struct bma4_dev *dev)
{
    uint8_t feature_config[BMA421_FEATURE_SIZE] = { 0 };
    uint8_t index = BMA421_AXES_REMAP_OFFSET;
    int8_t rslt = BMA4_OK;
    uint8_t x_axis = 0;
    uint8_t x_axis_sign = 0;
    uint8_t y_axis = 0;
    uint8_t y_axis_sign = 0;
    uint8_t z_axis = 0;

    if (dev != NULL)
    {
        if (dev->chip_id == BMA421_CHIP_ID)
        {
            rslt = bma4_read_regs(BMA4_FEATURE_CONFIG_ADDR, feature_config, BMA421_FEATURE_SIZE, dev);
            if (rslt == BMA4_OK)
            {
                x_axis = remap_data->x_axis & BMA421_X_AXIS_MASK;
                x_axis_sign = (remap_data->x_axis_sign << 2) & BMA421_X_AXIS_SIGN_MASK;
                y_axis = (remap_data->y_axis << 3) & BMA421_Y_AXIS_MASK;
                y_axis_sign = (remap_data->y_axis_sign << 5) & BMA421_Y_AXIS_SIGN_MASK;
                z_axis = (remap_data->z_axis << 6) & BMA421_Z_AXIS_MASK;
                feature_config[index] = x_axis | x_axis_sign | y_axis | y_axis_sign | z_axis;
                feature_config[index + 1] = remap_data->z_axis_sign & BMA421_Z_AXIS_SIGN_MASK;
                rslt = bma4_write_regs(BMA4_FEATURE_CONFIG_ADDR, feature_config, BMA421_FEATURE_SIZE, dev);
            }
        }
        else
        {
            rslt = BMA4_E_INVALID_SENSOR;
        }
    }
    else
    {
        rslt = BMA4_E_NULL_PTR;
    }

    return rslt;
}

/*!
 * @brief This API reads the x, y and z axis remap data from the sensor.
 */
int8_t bma421_get_remap_axes(struct bma421_axes_remap *remap_data, struct bma4_dev *dev)
{
    uint8_t feature_config[BMA421_FEATURE_SIZE] = { 0 };
    uint8_t index = BMA421_AXES_REMAP_OFFSET;
    int8_t rslt = BMA4_OK;

    if (dev != NULL)
    {
        if (dev->chip_id == BMA421_CHIP_ID)
        {
            rslt = bma4_read_regs(BMA4_FEATURE_CONFIG_ADDR, feature_config, BMA421_FEATURE_SIZE, dev);
            if (rslt == BMA4_OK)
            {
                remap_data->x_axis = feature_config[index] & BMA421_X_AXIS_MASK;
                remap_data->x_axis_sign = (feature_config[index] & BMA421_X_AXIS_SIGN_MASK) >> 2;
                remap_data->y_axis = (feature_config[index] & BMA421_Y_AXIS_MASK) >> 3;
                remap_data->y_axis_sign = (feature_config[index] & BMA421_Y_AXIS_SIGN_MASK) >> 5;
                remap_data->z_axis = (feature_config[index] & BMA421_Z_AXIS_MASK) >> 6;
                remap_data->z_axis_sign = (feature_config[index + 1] & BMA421_Z_AXIS_SIGN_MASK);
            }
        }
        else
        {
            rslt = BMA4_E_INVALID_SENSOR;
        }
    }
    else
    {
        rslt = BMA4_E_NULL_PTR;
    }

    return rslt;
}

/*!
 * @brief This API sets the configuration of any-motion feature in the sensor.
 * This API enables/disables the any-motion feature according to the axis set.
 */
int8_t bma421_set_any_mot_config(const struct bma421_any_no_mot_config *any_mot, struct bma4_dev *dev)
{
    /* Variable to define error */
    int8_t rslt = BMA4_OK;

    /* Initialize configuration file */
    uint8_t feature_config[BMA421_FEATURE_SIZE] = { 0 };

    /* Update index to configure any-motion axes */
    uint8_t index = BMA421_ANY_MOT_OFFSET;

    /* Variable to define LSB */
    uint16_t lsb = 0;

    /* Variable to define MSB */
    uint16_t msb = 0;

    /* Variable to define LSB and MSB */
    uint16_t lsb_msb = 0;

    if ((dev != NULL) && (any_mot != NULL))
    {
        /* Get any-motion configuration from the sensor */
        rslt = bma4_read_regs(BMA4_FEATURE_CONFIG_ADDR, feature_config, BMA421_ANY_MOT_LEN, dev);
        if (rslt == BMA4_OK)
        {
            /* Set threshold value in feature configuration array */
            feature_config[index++] = BMA4_GET_LSB(any_mot->threshold);
            feature_config[index++] = BMA4_GET_MSB(any_mot->threshold);

            /* Extract the word where duration and axes enable
             * resides
             */
            lsb = feature_config[index];
            msb = feature_config[index + 1] << 8;
            lsb_msb = lsb | msb;

            /* Set the duration in the same word */
            lsb_msb = BMA4_SET_BITS_POS_0(lsb_msb, BMA421_ANY_NO_MOT_DUR, any_mot->duration);

            /* Set the axes in the same word */
            lsb_msb = BMA4_SET_BITSLICE(lsb_msb, BMA421_ANY_NO_MOT_AXIS_EN, any_mot->axes_en);

            /* Assign the word with set duration and axes enable
             * value back to feature configuration array
             */
            feature_config[index++] = BMA4_GET_LSB(lsb_msb);
            feature_config[index] = BMA4_GET_MSB(lsb_msb);

            /* Set any-motion configuration to the sensor */
            rslt = bma4_write_regs(BMA4_FEATURE_CONFIG_ADDR, feature_config, BMA421_ANY_MOT_LEN, dev);
        }
    }
    else
    {
        rslt = BMA4_E_NULL_PTR;
    }

    return rslt;
}

/*!
 * @brief This API gets the configuration of any-motion feature from the
 * sensor.
 */
int8_t bma421_get_any_mot_config(struct bma421_any_no_mot_config *any_mot, struct bma4_dev *dev)
{
    /* Variable to define error */
    int8_t rslt = BMA4_OK;

    /* Initialize configuration file */
    uint8_t feature_config[BMA421_FEATURE_SIZE] = { 0 };

    /* Update index to configure any-motion axes */
    uint8_t index = BMA421_ANY_MOT_OFFSET;

    /* Variable to define LSB */
    uint16_t lsb = 0;

    /* Variable to define MSB */
    uint16_t msb = 0;

    /* Variable to define LSB and MSB */
    uint16_t lsb_msb = 0;

    if ((dev != NULL) && (any_mot != NULL))
    {
        /* Get any-motion configuration from the sensor */
        rslt = bma4_read_regs(BMA4_FEATURE_CONFIG_ADDR, feature_config, BMA421_ANY_MOT_LEN, dev);
        if (rslt == BMA4_OK)
        {
            /* Get word to calculate threshold and any-motion
             * select
             */
            lsb = (uint16_t)feature_config[index++];
            msb = ((uint16_t)feature_config[index++] << 8);
            lsb_msb = lsb | msb;

            /* Extract threshold value */
            any_mot->threshold = lsb_msb & BMA421_ANY_NO_MOT_THRES_MSK;

            /* Get word to calculate duration and axes enable */
            lsb = (uint16_t)feature_config[index++];
            msb = ((uint16_t)feature_config[index] << 8);
            lsb_msb = lsb | msb;

            /* Extract duration value */
            any_mot->duration = lsb_msb & BMA421_ANY_NO_MOT_DUR_MSK;

            /* Extract axes enable value */
            any_mot->axes_en = (uint8_t)((lsb_msb & BMA421_ANY_NO_MOT_AXIS_EN_MSK) >> BMA421_ANY_NO_MOT_AXIS_EN_POS);
        }
    }
    else
    {
        rslt = BMA4_E_NULL_PTR;
    }

    return rslt;
}

/*!
 * @brief This API sets the configuration of no-motion feature in the sensor.
 * This API enables/disables the no-motion feature according to the axis set.
 */
int8_t bma421_set_no_mot_config(const struct bma421_any_no_mot_config *no_mot, struct bma4_dev *dev)
{
    /* Variable to define error */
    int8_t rslt = BMA4_OK;

    /* Initialize configuration file */
    uint8_t feature_config[BMA421_FEATURE_SIZE] = { 0 };

    /* Update index to configure no-motion axes */
    uint8_t index = BMA421_NO_MOT_OFFSET;

    /* Variable to define LSB */
    uint16_t lsb = 0;

    /* Variable to define MSB */
    uint16_t msb = 0;

    /* Variable to define LSB and MSB */
    uint16_t lsb_msb = 0;

    if ((dev != NULL) && (no_mot != NULL))
    {
        /* Get no-motion configuration from the sensor */
        rslt = bma4_read_regs(BMA4_FEATURE_CONFIG_ADDR, feature_config, BMA421_NO_MOT_RD_WR_LEN, dev);
        if (rslt == BMA4_OK)
        {
            /* Set threshold value in feature configuration array */
            feature_config[index++] = BMA4_GET_LSB(no_mot->threshold);
            feature_config[index++] = BMA4_GET_MSB(no_mot->threshold);

            /* Extract the word where duration and axes enable
             * resides
             */
            lsb = feature_config[index];
            msb = feature_config[index + 1] << 8;
            lsb_msb = lsb | msb;

            /* Set the duration in the same word */
            lsb_msb = BMA4_SET_BITS_POS_0(lsb_msb, BMA421_ANY_NO_MOT_DUR, no_mot->duration);

            /* Set the axes in the same word */
            lsb_msb = BMA4_SET_BITSLICE(lsb_msb, BMA421_ANY_NO_MOT_AXIS_EN, no_mot->axes_en);

            /* Assign the word with set duration and axes enable
             * value back to feature configuration array
             */
            feature_config[index++] = BMA4_GET_LSB(lsb_msb);
            feature_config[index] = BMA4_GET_MSB(lsb_msb);

            /* Set no-motion configuration to the sensor */
            rslt = bma4_write_regs(BMA4_FEATURE_CONFIG_ADDR, feature_config, BMA421_NO_MOT_RD_WR_LEN, dev);
        }
    }
    else
    {
        rslt = BMA4_E_NULL_PTR;
    }

    return rslt;
}

/*!
 * @brief This API gets the configuration of no-motion feature from the
 * sensor.
 */
int8_t bma421_get_no_mot_config(struct bma421_any_no_mot_config *no_mot, struct bma4_dev *dev)
{
    /* Variable to define error */
    int8_t rslt = BMA4_OK;

    /* Initialize configuration file */
    uint8_t feature_config[BMA421_FEATURE_SIZE] = { 0 };

    /* Update index to configure no-motion axes */
    uint8_t index = BMA421_NO_MOT_OFFSET;

    /* Variable to define LSB */
    uint16_t lsb = 0;

    /* Variable to define MSB */
    uint16_t msb = 0;

    /* Variable to define LSB and MSB */
    uint16_t lsb_msb = 0;

    if ((dev != NULL) && (no_mot != NULL))
    {
        /* Get no-motion configuration from the sensor */
        rslt = bma4_read_regs(BMA4_FEATURE_CONFIG_ADDR, feature_config, BMA421_NO_MOT_RD_WR_LEN, dev);
        if (rslt == BMA4_OK)
        {
            /* Get word to calculate threshold and no-motion
             * select
             */
            lsb = (uint16_t)feature_config[index++];
            msb = ((uint16_t)feature_config[index++] << 8);
            lsb_msb = lsb | msb;

            /* Extract threshold value */
            no_mot->threshold = lsb_msb & BMA421_ANY_NO_MOT_THRES_MSK;

            /* Get word to calculate duration and axes enable */
            lsb = (uint16_t)feature_config[index++];
            msb = ((uint16_t)feature_config[index] << 8);
            lsb_msb = lsb | msb;

            /* Extract duration value */
            no_mot->duration = lsb_msb & BMA421_ANY_NO_MOT_DUR_MSK;

            /* Extract axes enable value */
            no_mot->axes_en = (uint8_t)((lsb_msb & BMA421_ANY_NO_MOT_AXIS_EN_MSK) >> BMA421_ANY_NO_MOT_AXIS_EN_POS);
        }
    }
    else
    {
        rslt = BMA4_E_NULL_PTR;
    }

    return rslt;
}

/*!
 * @brief This API enables or disables the step detector feature in the sensor.
 */
int8_t bma421_step_detector_enable(uint8_t enable, struct bma4_dev *dev)
{
    uint8_t feature_config[BMA421_FEATURE_SIZE] = { 0 };
    int8_t rslt = BMA4_OK;

    /* Step detector enable bit position is 1 byte ahead of the base address */
    uint8_t index = BMA421_STEP_CNTR_OFFSET + 1;

    if (dev != NULL)
    {
        if (dev->chip_id == BMA421_CHIP_ID)
        {
            rslt = bma4_read_regs(BMA4_FEATURE_CONFIG_ADDR, feature_config, BMA421_FEATURE_SIZE, dev);
            if (rslt == BMA4_OK)
            {
                feature_config[index] = BMA4_SET_BITSLICE(feature_config[index], BMA421_STEP_DETECTOR_EN, enable);
                rslt = bma4_write_regs(BMA4_FEATURE_CONFIG_ADDR, feature_config, BMA421_FEATURE_SIZE, dev);
            }
        }
        else
        {
            rslt = BMA4_E_INVALID_SENSOR;
        }
    }
    else
    {
        rslt = BMA4_E_NULL_PTR;
    }

    return rslt;
}

/*!
 * @brief This API sets the watermark level for step counter interrupt in the
 * sensor.
 */
int8_t bma421_step_counter_set_watermark(uint16_t step_counter_wm, struct bma4_dev *dev)
{
    uint8_t feature_config[BMA421_FEATURE_SIZE] = { 0 };
    uint8_t index = BMA421_STEP_CNTR_OFFSET;
    uint16_t wm_lsb = 0;
    uint16_t wm_msb = 0;
    int8_t rslt = BMA4_OK;
    uint16_t data = 0;

    if (dev != NULL)
    {
        if (dev->chip_id == BMA421_CHIP_ID)
        {
            rslt = bma4_read_regs(BMA4_FEATURE_CONFIG_ADDR, feature_config, BMA421_FEATURE_SIZE, dev);
            if (rslt == BMA4_OK)
            {
                wm_lsb = feature_config[index];
                wm_msb = feature_config[index + 1] << 8;
                data = wm_lsb | wm_msb;

                /* Sets only watermark bits in the complete
                 * 16 bits of data
                 */
                data = BMA4_SET_BITS_POS_0(data, BMA421_STEP_CNTR_WM, step_counter_wm);

                /* Splits 16 bits of data to individual
                 * 8 bits data
                 */
                feature_config[index] = BMA4_GET_LSB(data);
                feature_config[index + 1] = BMA4_GET_MSB(data);

                /* Writes stepcounter watermark settings
                 * in the sensor
                 */
                rslt = bma4_write_regs(BMA4_FEATURE_CONFIG_ADDR, feature_config, BMA421_FEATURE_SIZE, dev);
            }
        }
        else
        {
            rslt = BMA4_E_INVALID_SENSOR;
        }
    }
    else
    {
        rslt = BMA4_E_NULL_PTR;
    }

    return rslt;
}

/*!
 * @brief This API gets the water mark level set for step counter interrupt
 * in the sensor.
 */
int8_t bma421_step_counter_get_watermark(uint16_t *step_counter_wm, struct bma4_dev *dev)
{
    uint8_t feature_config[BMA421_FEATURE_SIZE] = { 0 };
    uint8_t index = BMA421_STEP_CNTR_OFFSET;
    uint16_t wm_lsb = 0;
    uint16_t wm_msb = 0;
    int8_t rslt = BMA4_OK;
    uint16_t data = 0;

    if (dev != NULL)
    {
        if (dev->chip_id == BMA421_CHIP_ID)
        {
            rslt = bma4_read_regs(BMA4_FEATURE_CONFIG_ADDR, feature_config, BMA421_FEATURE_SIZE, dev);
            if (rslt == BMA4_OK)
            {
                wm_lsb = feature_config[index];
                wm_msb = feature_config[index + 1] << 8;
                data = wm_lsb | wm_msb;
                *step_counter_wm = BMA4_GET_BITS_POS_0(data, BMA421_STEP_CNTR_WM);
            }
        }
        else
        {
            rslt = BMA4_E_INVALID_SENSOR;
        }
    }
    else
    {
        rslt = BMA4_E_NULL_PTR;
    }

    return rslt;
}

/*!
 * @brief This API resets the counted steps of step counter.
 */
int8_t bma421_reset_step_counter(struct bma4_dev *dev)
{
    uint8_t feature_config[BMA421_FEATURE_SIZE] = { 0 };

    /* Reset bit is 1 byte ahead of base address */
    uint8_t index = BMA421_STEP_CNTR_OFFSET + 1;
    int8_t rslt = BMA4_OK;

    if (dev != NULL)
    {
        if (dev->chip_id == BMA421_CHIP_ID)
        {
            rslt = bma4_read_regs(BMA4_FEATURE_CONFIG_ADDR, feature_config, BMA421_FEATURE_SIZE, dev);
            if (rslt == BMA4_OK)
            {
                feature_config[index] = BMA4_SET_BITSLICE(feature_config[index], BMA421_STEP_CNTR_RST, 1);
                rslt = bma4_write_regs(BMA4_FEATURE_CONFIG_ADDR, feature_config, BMA421_FEATURE_SIZE, dev);
            }
        }
        else
        {
            rslt = BMA4_E_INVALID_SENSOR;
        }
    }
    else
    {
        rslt = BMA4_E_NULL_PTR;
    }

    return rslt;
}

/*!
 * @brief This API gets the number of counted steps of the step counter
 * feature from the sensor.
 */
int8_t bma421_step_counter_output(uint32_t *step_count, struct bma4_dev *dev)
{
    uint8_t data[BMA421_STEP_CNTR_DATA_SIZE] = { 0 };
    int8_t rslt = BMA4_OK;
    uint32_t step_count_0 = 0;
    uint32_t step_count_1 = 0;
    uint32_t step_count_2 = 0;
    uint32_t step_count_3 = 0;

    if (dev != NULL)
    {
        if (dev->chip_id == BMA421_CHIP_ID)
        {
            /* Reads the step counter output data from the
             * gpio register
             */
            rslt = bma4_read_regs(BMA4_STEP_CNT_OUT_0_ADDR, data, BMA421_STEP_CNTR_DATA_SIZE, dev);
            if (rslt == BMA4_OK)
            {
                step_count_0 = (uint32_t)data[0];
                step_count_1 = (uint32_t)data[1] << 8;
                step_count_2 = (uint32_t)data[2] << 16;
                step_count_3 = (uint32_t)data[3] << 24;
                *step_count = step_count_0 | step_count_1 | step_count_2 | step_count_3;
            }
        }
        else
        {
            rslt = BMA4_E_INVALID_SENSOR;
        }
    }
    else
    {
        rslt = BMA4_E_NULL_PTR;
    }

    return rslt;
}

/*!
 * @brief This API gets the output for activity feature.
 */
int8_t bma421_activity_output(uint8_t *activity, struct bma4_dev *dev)
{
    uint8_t data = 0;
    int8_t rslt = BMA4_OK;

    if (dev != NULL)
    {
        if (dev->chip_id == BMA421_CHIP_ID)
        {
            /* Reads the activity output from the gpio register */
            rslt = bma4_read_regs(BMA4_ACTIVITY_OUT_ADDR, &data, 1, dev);
            if (rslt == BMA4_OK)
            {
                *activity = data;
            }
        }
        else
        {
            rslt = BMA4_E_INVALID_SENSOR;
        }
    }
    else
    {
        rslt = BMA4_E_NULL_PTR;
    }

    return rslt;
}

/*!
 * @brief This API gets the parameter1 to parameter7 settings of the step
 * counter feature.
 */
int8_t bma421_stepcounter_get_parameter(struct bma421_stepcounter_settings *setting, struct bma4_dev *dev)
{
    uint8_t feature_config[BMA421_FEATURE_SIZE] = { 0 };
    uint16_t *data_p = (uint16_t *)(void *)feature_config;
    int8_t rslt = BMA4_OK;

    if (dev != NULL)
    {
        if (dev->chip_id == BMA421_CHIP_ID)
        {
            rslt = bma4_read_regs(BMA4_FEATURE_CONFIG_ADDR, feature_config, BMA421_FEATURE_SIZE, dev);
            if (rslt == BMA4_OK)
            {
                /* To convert 8bit to 16 bit address */
                data_p = data_p + BMA421_STEP_CNTR_PARAM_OFFSET / 2;
                extract_stepcounter_parameter(setting, data_p);
            }
        }
        else
        {
            rslt = BMA4_E_INVALID_SENSOR;
        }
    }
    else
    {
        rslt = BMA4_E_NULL_PTR;
    }

    return rslt;
}

/*!
 * @brief This API sets the parameter1 to parameter7 settings of the step
 * counter feature in the sensor.
 */
int8_t bma421_stepcounter_set_parameter(const struct bma421_stepcounter_settings *setting, struct bma4_dev *dev)
{
    uint8_t feature_config[BMA421_FEATURE_SIZE] = { 0 };
    uint8_t index = BMA421_STEP_CNTR_PARAM_OFFSET;
    int8_t rslt = BMA4_OK;

    if (dev != NULL)
    {
        if (dev->chip_id == BMA421_CHIP_ID)
        {
            rslt = bma4_read_regs(BMA4_FEATURE_CONFIG_ADDR, feature_config, BMA421_FEATURE_SIZE, dev);
            if (rslt == BMA4_OK)
            {
                update_stepcounter_parameter(setting, index, feature_config);

                /* Writes step counter parameter settings
                 * in the sensor
                 */
                rslt = bma4_write_regs(BMA4_FEATURE_CONFIG_ADDR, feature_config, BMA421_FEATURE_SIZE, dev);
            }
        }
        else
        {
            rslt = BMA4_E_INVALID_SENSOR;
        }
    }
    else
    {
        rslt = BMA4_E_NULL_PTR;
    }

    return rslt;
}

/*!
 * @brief This API sets the sensitivity of single tap feature in the sensor.
 */
int8_t bma421_single_tap_set_sensitivity(uint8_t sensitivity, struct bma4_dev *dev)
{
    uint8_t feature_config[BMA421_FEATURE_SIZE] = { 0 };
    uint8_t index = BMA421_SINGLE_TAP_OFFSET;
    int8_t rslt = BMA4_OK;

    if (dev != NULL)
    {
        if (dev->chip_id == BMA421_CHIP_ID)
        {
            rslt = bma4_read_regs(BMA4_FEATURE_CONFIG_ADDR, feature_config, BMA421_FEATURE_SIZE, dev);
            if (rslt == BMA4_OK)
            {
                feature_config[index] = BMA4_SET_BITSLICE(feature_config[index], BMA421_TAP_SENS, sensitivity);

                /* Writes sensitivity settings in the sensor */
                rslt = bma4_write_regs(BMA4_FEATURE_CONFIG_ADDR, feature_config, BMA421_FEATURE_SIZE, dev);
            }
        }
        else
        {
            rslt = BMA4_E_INVALID_SENSOR;
        }
    }
    else
    {
        rslt = BMA4_E_NULL_PTR;
    }

    return rslt;
}

/*!
 * @brief This API sets the sensitivity of double tap feature in the sensor.
 */
int8_t bma421_double_tap_set_sensitivity(uint8_t sensitivity, struct bma4_dev *dev)
{
    uint8_t feature_config[BMA421_FEATURE_SIZE] = { 0 };
    uint8_t index = BMA421_DOUBLE_TAP_OFFSET;
    int8_t rslt = BMA4_OK;

    if (dev != NULL)
    {
        if (dev->chip_id == BMA421_CHIP_ID)
        {
            rslt = bma4_read_regs(BMA4_FEATURE_CONFIG_ADDR, feature_config, BMA421_FEATURE_SIZE, dev);
            if (rslt == BMA4_OK)
            {
                feature_config[index] = BMA4_SET_BITSLICE(feature_config[index], BMA421_TAP_SENS, sensitivity);

                /* Writes sensitivity settings in the sensor */
                rslt = bma4_write_regs(BMA4_FEATURE_CONFIG_ADDR, feature_config, BMA421_FEATURE_SIZE, dev);
            }
        }
        else
        {
            rslt = BMA4_E_INVALID_SENSOR;
        }
    }
    else
    {
        rslt = BMA4_E_NULL_PTR;
    }

    return rslt;
}

/*!
 *  @brief This API gets the sensitivity of single tap feature in the sensor
 */
int8_t bma421_single_tap_get_sensitivity(uint8_t *sensitivity, struct bma4_dev *dev)
{
    uint8_t feature_config[BMA421_FEATURE_SIZE] = { 0 };
    uint8_t index = BMA421_SINGLE_TAP_OFFSET;
    int8_t rslt = BMA4_OK;

    if (dev != NULL)
    {
        if (dev->chip_id == BMA421_CHIP_ID)
        {
            rslt = bma4_read_regs(BMA4_FEATURE_CONFIG_ADDR, feature_config, BMA421_FEATURE_SIZE, dev);
            if (rslt == BMA4_OK)
            {
                /* Extracts sensitivity data */
                *sensitivity = BMA4_GET_BITSLICE(feature_config[index], BMA421_TAP_SENS);
            }
        }
        else
        {
            rslt = BMA4_E_INVALID_SENSOR;
        }
    }
    else
    {
        rslt = BMA4_E_NULL_PTR;
    }

    return rslt;
}

/*!
 *  @brief This API gets the sensitivity of double tap feature in the sensor
 */
int8_t bma421_double_tap_get_sensitivity(uint8_t *sensitivity, struct bma4_dev *dev)
{
    uint8_t feature_config[BMA421_FEATURE_SIZE] = { 0 };
    uint8_t index = BMA421_DOUBLE_TAP_OFFSET;
    int8_t rslt = BMA4_OK;

    if (dev != NULL)
    {
        if (dev->chip_id == BMA421_CHIP_ID)
        {
            rslt = bma4_read_regs(BMA4_FEATURE_CONFIG_ADDR, feature_config, BMA421_FEATURE_SIZE, dev);
            if (rslt == BMA4_OK)
            {
                /* Extracts sensitivity data */
                *sensitivity = BMA4_GET_BITSLICE(feature_config[index], BMA421_TAP_SENS);
            }
        }
        else
        {
            rslt = BMA4_E_INVALID_SENSOR;
        }
    }
    else
    {
        rslt = BMA4_E_NULL_PTR;
    }

    return rslt;
}

/*! @cond DOXYGEN_SUPRESS */

/* Suppressing doxygen warnings triggered for same static function names present across various sensor variant
 * directories */

/*!
 * @brief This API enables the features of the sensor.
 */
static int8_t feature_enable(uint8_t feature, uint8_t len, uint8_t *feature_config, struct bma4_dev *dev)
{
    uint8_t index = 0;
    int8_t rslt;

    /* Enable step counter */
    if ((feature & BMA421_STEP_CNTR) > 0)
    {
        index = BMA421_STEP_CNTR_OFFSET + 1;
        feature_config[index] = feature_config[index] | BMA421_STEP_CNTR_EN_MSK;
    }

    /* Enable step activity */
    if ((feature & BMA421_STEP_ACT) > 0)
    {
        index = BMA421_STEP_CNTR_OFFSET + 1;
        feature_config[index] = feature_config[index] | BMA421_STEP_ACT_EN_MSK;
    }

    /* Enable wrist wear wakeup */
    if ((feature & BMA421_WRIST_WEAR) > 0)
    {
        index = BMA421_WRIST_WEAR_OFFSET;
        feature_config[index] = feature_config[index] | BMA421_WRIST_WEAR_EN_MSK;
    }

    /* Enable single - tap */
    if ((feature & BMA421_SINGLE_TAP) > 0)
    {
        index = BMA421_SINGLE_TAP_OFFSET;
        feature_config[index] = feature_config[index] | BMA421_SINGLE_TAP_EN_MSK;
    }

    /* Enable  double- tap */
    if ((feature & BMA421_DOUBLE_TAP) > 0)
    {
        index = BMA421_DOUBLE_TAP_OFFSET;
        feature_config[index] = feature_config[index] | BMA421_DOUBLE_TAP_EN_MSK;
    }

    /* Write the feature enable settings in the sensor */
    rslt = bma4_write_regs(BMA4_FEATURE_CONFIG_ADDR, feature_config, len, dev);

    return rslt;
}

/*!
 * @brief This API disables the features of the sensor.
 */
static int8_t feature_disable(uint8_t feature, uint8_t len, uint8_t *feature_config, struct bma4_dev *dev)
{
    uint8_t index = 0;
    int8_t rslt;

    /* Disable step counter */
    if ((feature & BMA421_STEP_CNTR) > 0)
    {
        index = BMA421_STEP_CNTR_OFFSET + 1;
        feature_config[index] = feature_config[index] & (~BMA421_STEP_CNTR_EN_MSK);
    }

    /* Disable step activity */
    if ((feature & BMA421_STEP_ACT) > 0)
    {
        index = BMA421_STEP_CNTR_OFFSET + 1;
        feature_config[index] = feature_config[index] & (~BMA421_STEP_ACT_EN_MSK);
    }

    /* Disable wrist wear wakeup */
    if ((feature & BMA421_WRIST_WEAR) > 0)
    {
        index = BMA421_WRIST_WEAR_OFFSET;
        feature_config[index] = feature_config[index] & (~BMA421_WRIST_WEAR_EN_MSK);
    }

    /* Disable single-tap */
    if ((feature & BMA421_SINGLE_TAP) > 0)
    {
        index = BMA421_SINGLE_TAP_OFFSET;
        feature_config[index] = feature_config[index] & (~BMA421_SINGLE_TAP_EN_MSK);
    }

    /* Disable double-tap */
    if ((feature & BMA421_DOUBLE_TAP) > 0)
    {
        index = BMA421_DOUBLE_TAP_OFFSET;
        feature_config[index] = feature_config[index] & (~BMA421_DOUBLE_TAP_EN_MSK);
    }

    /* Write the configured settings in the sensor */
    rslt = bma4_write_regs(BMA4_FEATURE_CONFIG_ADDR, feature_config, len, dev);

    return rslt;
}

/*!
 *  @brief This API update the settings of step counter.
 */
static void update_stepcounter_parameter(const struct bma421_stepcounter_settings *setting,
                                         uint8_t index,
                                         uint8_t *feature_config)
{
    feature_config[index++] = BMA4_GET_LSB(setting->param1);
    feature_config[index++] = BMA4_GET_MSB(setting->param1);
    feature_config[index++] = BMA4_GET_LSB(setting->param2);
    feature_config[index++] = BMA4_GET_MSB(setting->param2);
    feature_config[index++] = BMA4_GET_LSB(setting->param3);
    feature_config[index++] = BMA4_GET_MSB(setting->param3);
    feature_config[index++] = BMA4_GET_LSB(setting->param4);
    feature_config[index++] = BMA4_GET_MSB(setting->param4);
    feature_config[index++] = BMA4_GET_LSB(setting->param5);
    feature_config[index++] = BMA4_GET_MSB(setting->param5);
    feature_config[index++] = BMA4_GET_LSB(setting->param6);
    feature_config[index++] = BMA4_GET_MSB(setting->param6);
    feature_config[index++] = BMA4_GET_LSB(setting->param7);
    feature_config[index++] = BMA4_GET_MSB(setting->param7);
    feature_config[index++] = BMA4_GET_LSB(setting->param8);
    feature_config[index++] = BMA4_GET_MSB(setting->param8);
    feature_config[index++] = BMA4_GET_LSB(setting->param9);
    feature_config[index++] = BMA4_GET_MSB(setting->param9);
    feature_config[index++] = BMA4_GET_LSB(setting->param10);
    feature_config[index++] = BMA4_GET_MSB(setting->param10);
    feature_config[index++] = BMA4_GET_LSB(setting->param11);
    feature_config[index++] = BMA4_GET_MSB(setting->param11);
    feature_config[index++] = BMA4_GET_LSB(setting->param12);
    feature_config[index++] = BMA4_GET_MSB(setting->param12);
    feature_config[index++] = BMA4_GET_LSB(setting->param13);
    feature_config[index++] = BMA4_GET_MSB(setting->param13);
    feature_config[index++] = BMA4_GET_LSB(setting->param14);
    feature_config[index++] = BMA4_GET_MSB(setting->param14);
    feature_config[index++] = BMA4_GET_LSB(setting->param15);
    feature_config[index++] = BMA4_GET_MSB(setting->param15);
    feature_config[index++] = BMA4_GET_LSB(setting->param16);
    feature_config[index++] = BMA4_GET_MSB(setting->param16);
    feature_config[index++] = BMA4_GET_LSB(setting->param17);
    feature_config[index++] = BMA4_GET_MSB(setting->param17);
    feature_config[index++] = BMA4_GET_LSB(setting->param18);
    feature_config[index++] = BMA4_GET_MSB(setting->param18);
    feature_config[index++] = BMA4_GET_LSB(setting->param19);
    feature_config[index++] = BMA4_GET_MSB(setting->param19);
    feature_config[index++] = BMA4_GET_LSB(setting->param20);
    feature_config[index++] = BMA4_GET_MSB(setting->param20);
    feature_config[index++] = BMA4_GET_LSB(setting->param21);
    feature_config[index++] = BMA4_GET_MSB(setting->param21);
    feature_config[index++] = BMA4_GET_LSB(setting->param22);
    feature_config[index++] = BMA4_GET_MSB(setting->param22);
    feature_config[index++] = BMA4_GET_LSB(setting->param23);
    feature_config[index++] = BMA4_GET_MSB(setting->param23);
    feature_config[index++] = BMA4_GET_LSB(setting->param24);
    feature_config[index++] = BMA4_GET_MSB(setting->param24);
    feature_config[index++] = BMA4_GET_LSB(setting->param25);
    feature_config[index] = BMA4_GET_MSB(setting->param25);
}

/*!
 * @brief This API copy the settings of step counter into the structure of
 * bma421_stepcounter_settings, which is read from sensor.
 */
static void extract_stepcounter_parameter(struct bma421_stepcounter_settings *setting, const uint16_t *data_p)
{
    setting->param1 = *(data_p++);
    setting->param2 = *(data_p++);
    setting->param3 = *(data_p++);
    setting->param4 = *(data_p++);
    setting->param5 = *(data_p++);
    setting->param6 = *(data_p++);
    setting->param7 = *(data_p++);
    setting->param8 = *(data_p++);
    setting->param9 = *(data_p++);
    setting->param10 = *(data_p++);
    setting->param11 = *(data_p++);
    setting->param12 = *(data_p++);
    setting->param13 = *(data_p++);
    setting->param14 = *(data_p++);
    setting->param15 = *(data_p++);
    setting->param16 = *(data_p++);
    setting->param17 = *(data_p++);
    setting->param18 = *(data_p++);
    setting->param19 = *(data_p++);
    setting->param20 = *(data_p++);
    setting->param21 = *(data_p++);
    setting->param22 = *(data_p++);
    setting->param23 = *(data_p++);
    setting->param24 = *(data_p++);
    setting->param25 = *data_p;
}

/*!
 * @brief This API is used to get the config file major and minor information.
 */
int8_t bma421_get_version_config(uint16_t *config_major, uint16_t *config_minor, struct bma4_dev *dev)
{
    /* Initialize configuration file */
    uint8_t feature_config[BMA421_FEATURE_SIZE] = { 0 };

    /* Update index to config file version */
    uint8_t index = BMA421_CONFIG_ID_START_ADDR;

    /* Variable to define LSB */
    uint8_t lsb = 0;

    /* Variable to define MSB */
    uint8_t msb = 0;

    /* Variable to define LSB and MSB */
    uint16_t lsb_msb = 0;

    /* Result of api are returned to this variable */
    int8_t rslt = BMA4_OK;

    if ((dev != NULL) && (config_major != NULL) && (config_minor != NULL))
    {
        rslt = bma4_set_advance_power_save(BMA4_DISABLE, dev);

        /* Wait for sensor time synchronization. Refer the data-sheet for
         * more information
         */
        dev->delay_us(450, dev->intf_ptr);

        if (rslt == BMA4_OK)
        {
            /* Get config file identification from the sensor */
            rslt = bma4_read_regs(BMA4_FEATURE_CONFIG_ADDR, feature_config, BMA421_FEATURE_SIZE, dev);

            if (rslt == BMA4_OK)
            {
                /* Get word to calculate config file identification */
                lsb = feature_config[index++];
                msb = feature_config[index++];

                lsb_msb = (uint16_t)(msb << 8 | lsb);

                /* Get major and minor version */
                *config_major = BMA4_GET_BITSLICE(lsb_msb, BMA421_CONFIG_MAJOR);
                *config_minor = BMA4_GET_BITS_POS_0(lsb, BMA421_CONFIG_MINOR);
            }
        }
    }
    else
    {
        rslt = BMA4_E_NULL_PTR;
    }

    return rslt;
}

/*! @endcond */
