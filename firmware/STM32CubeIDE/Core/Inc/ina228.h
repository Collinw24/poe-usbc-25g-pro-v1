#ifndef INA228_H
#define INA228_H

#include "main.h"

typedef struct
{
    uint32_t bus_voltage_mv;
    uint32_t current_ma;
    uint32_t power_mw;
} ina228_reading_t;

HAL_StatusTypeDef ina228_init(I2C_HandleTypeDef *hi2c);
HAL_StatusTypeDef ina228_read(ina228_reading_t *reading);

#endif /* INA228_H */

