#include "telemetry.h"

#include "tmp117.h"

HAL_StatusTypeDef telemetry_init(I2C_HandleTypeDef *poe_i2c, I2C_HandleTypeDef *sys_i2c)
{
    if (ltc9105_init(poe_i2c) != HAL_OK)
    {
        return HAL_ERROR;
    }

    if (ina228_init(sys_i2c) != HAL_OK)
    {
        return HAL_ERROR;
    }

    if (tmp117_init(sys_i2c) != HAL_OK)
    {
        return HAL_ERROR;
    }

    return HAL_OK;
}

HAL_StatusTypeDef telemetry_refresh_poe(telemetry_snapshot_t *snapshot)
{
    if (snapshot == NULL)
    {
        return HAL_ERROR;
    }

    return ltc9105_poll_status(&snapshot->poe);
}

HAL_StatusTypeDef telemetry_refresh_local(telemetry_snapshot_t *snapshot)
{
    if (snapshot == NULL)
    {
        return HAL_ERROR;
    }

    if (ina228_read(&snapshot->output) != HAL_OK)
    {
        return HAL_ERROR;
    }

    if (tmp117_read_celsius(TMP117_PRIMARY, &snapshot->temp_primary_c) != HAL_OK)
    {
        return HAL_ERROR;
    }

    if (tmp117_read_celsius(TMP117_SECONDARY, &snapshot->temp_secondary_c) != HAL_OK)
    {
        return HAL_ERROR;
    }

    return HAL_OK;
}

void telemetry_set_link_state(telemetry_snapshot_t *snapshot, bool link_up, uint16_t speed_mbps)
{
    if (snapshot == NULL)
    {
        return;
    }

    snapshot->link_up = link_up;
    snapshot->link_speed_mbps = speed_mbps;
}

