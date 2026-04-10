#include "ina228.h"

static I2C_HandleTypeDef *g_ina228_i2c = NULL;

HAL_StatusTypeDef ina228_init(I2C_HandleTypeDef *hi2c)
{
    g_ina228_i2c = hi2c;
    return (g_ina228_i2c != NULL) ? HAL_OK : HAL_ERROR;
}

HAL_StatusTypeDef ina228_read(ina228_reading_t *reading)
{
    if ((g_ina228_i2c == NULL) || (reading == NULL))
    {
        return HAL_ERROR;
    }

    /*
     * TODO:
     * - Program shunt calibration
     * - Read bus voltage, current, and power registers
     * - Convert to millivolt / milliamp / milliwatt units
     */
    reading->bus_voltage_mv = 20000U;
    reading->current_ma = 2500U;
    reading->power_mw = 50000U;

    return HAL_OK;
}

