#ifndef LTC9105_I2C_H
#define LTC9105_I2C_H

#include "main.h"

typedef enum
{
    LTC9105_SOURCE_UNKNOWN = 0,
    LTC9105_SOURCE_AF,
    LTC9105_SOURCE_AT,
    LTC9105_SOURCE_BT_TYPE3,
    LTC9105_SOURCE_BT_TYPE4,
    LTC9105_SOURCE_PASSIVE_48V
} ltc9105_source_t;

typedef struct
{
    ltc9105_source_t source;
    uint32_t allocated_power_mw;
    uint32_t input_voltage_mv;
    uint32_t input_current_ma;
    bool source_valid;
    bool passive_warning;
} ltc9105_status_t;

HAL_StatusTypeDef ltc9105_init(I2C_HandleTypeDef *hi2c);
HAL_StatusTypeDef ltc9105_poll_status(ltc9105_status_t *status);
const char *ltc9105_source_to_string(ltc9105_source_t source);

#endif /* LTC9105_I2C_H */

