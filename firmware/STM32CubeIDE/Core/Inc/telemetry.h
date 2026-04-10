#ifndef TELEMETRY_H
#define TELEMETRY_H

#include "app_config.h"
#include "ina228.h"
#include "ltc9105_i2c.h"

#include <stdbool.h>
#include <stdint.h>

typedef enum
{
    POWER_PROFILE_STANDARD = 0,
    POWER_PROFILE_PRO
} power_profile_t;

typedef enum
{
    THROTTLE_NONE = 0,
    THROTTLE_SOFT,
    THROTTLE_HARD,
    THROTTLE_SHUTDOWN
} throttle_state_t;

typedef struct
{
    ltc9105_status_t poe;
    ina228_reading_t output;
    int16_t temp_primary_c;
    int16_t temp_secondary_c;
    throttle_state_t throttle;
    bool link_up;
    uint16_t link_speed_mbps;
    bool passive_mode_enabled;
    bool latched_fault;
    power_profile_t profile;
} telemetry_snapshot_t;

HAL_StatusTypeDef telemetry_init(I2C_HandleTypeDef *poe_i2c, I2C_HandleTypeDef *sys_i2c);
HAL_StatusTypeDef telemetry_refresh_poe(telemetry_snapshot_t *snapshot);
HAL_StatusTypeDef telemetry_refresh_local(telemetry_snapshot_t *snapshot);
void telemetry_set_link_state(telemetry_snapshot_t *snapshot, bool link_up, uint16_t speed_mbps);

#endif /* TELEMETRY_H */

