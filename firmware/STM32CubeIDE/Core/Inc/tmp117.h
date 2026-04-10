#ifndef TMP117_H
#define TMP117_H

#include "main.h"

typedef enum
{
    TMP117_PRIMARY = 0,
    TMP117_SECONDARY = 1
} tmp117_sensor_id_t;

HAL_StatusTypeDef tmp117_init(I2C_HandleTypeDef *hi2c);
HAL_StatusTypeDef tmp117_read_celsius(tmp117_sensor_id_t sensor, int16_t *temperature_c);

#endif /* TMP117_H */

