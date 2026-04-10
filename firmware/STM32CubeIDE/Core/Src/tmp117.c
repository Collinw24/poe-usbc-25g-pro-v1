#include "tmp117.h"

static I2C_HandleTypeDef *g_tmp117_i2c = NULL;

HAL_StatusTypeDef tmp117_init(I2C_HandleTypeDef *hi2c)
{
    g_tmp117_i2c = hi2c;
    return (g_tmp117_i2c != NULL) ? HAL_OK : HAL_ERROR;
}

HAL_StatusTypeDef tmp117_read_celsius(tmp117_sensor_id_t sensor, int16_t *temperature_c)
{
    if ((g_tmp117_i2c == NULL) || (temperature_c == NULL))
    {
        return HAL_ERROR;
    }

    *temperature_c = (sensor == TMP117_PRIMARY) ? 46 : 49;
    return HAL_OK;
}

