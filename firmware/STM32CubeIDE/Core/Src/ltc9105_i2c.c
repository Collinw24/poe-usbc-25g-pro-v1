#include "ltc9105_i2c.h"

static I2C_HandleTypeDef *g_ltc9105_i2c = NULL;

HAL_StatusTypeDef ltc9105_init(I2C_HandleTypeDef *hi2c)
{
    g_ltc9105_i2c = hi2c;
    return (g_ltc9105_i2c != NULL) ? HAL_OK : HAL_ERROR;
}

HAL_StatusTypeDef ltc9105_poll_status(ltc9105_status_t *status)
{
    if ((g_ltc9105_i2c == NULL) || (status == NULL))
    {
        return HAL_ERROR;
    }

    /*
     * TODO:
     * - Replace placeholder field population with real LTC9105 register reads
     * - Decode allocation, classification, and priority status
     * - Gate passive-mode interpretation behind explicit advanced-mode logic
     */
    status->source = LTC9105_SOURCE_BT_TYPE4;
    status->allocated_power_mw = 71300U;
    status->input_voltage_mv = 52000U;
    status->input_current_ma = 1220U;
    status->source_valid = true;
    status->passive_warning = false;

    return HAL_OK;
}

const char *ltc9105_source_to_string(ltc9105_source_t source)
{
    switch (source)
    {
    case LTC9105_SOURCE_AF:
        return "802.3af";
    case LTC9105_SOURCE_AT:
        return "802.3at";
    case LTC9105_SOURCE_BT_TYPE3:
        return "802.3bt T3";
    case LTC9105_SOURCE_BT_TYPE4:
        return "802.3bt T4";
    case LTC9105_SOURCE_PASSIVE_48V:
        return "48V Passive";
    case LTC9105_SOURCE_UNKNOWN:
    default:
        return "Unknown";
    }
}

