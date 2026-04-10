#include "safety.h"

#include "app_config.h"

throttle_state_t safety_evaluate(telemetry_snapshot_t *snapshot)
{
    int16_t hottest_c;

    if (snapshot == NULL)
    {
        return THROTTLE_SHUTDOWN;
    }

    hottest_c = (snapshot->temp_primary_c > snapshot->temp_secondary_c) ? snapshot->temp_primary_c : snapshot->temp_secondary_c;

    if (hottest_c >= APP_TEMP_SHUTDOWN_C)
    {
        snapshot->throttle = THROTTLE_SHUTDOWN;
    }
    else if (hottest_c >= APP_TEMP_HARD_DERATE_C)
    {
        snapshot->throttle = THROTTLE_HARD;
    }
    else if (hottest_c >= APP_TEMP_SOFT_DERATE_C)
    {
        snapshot->throttle = THROTTLE_SOFT;
    }
    else
    {
        snapshot->throttle = THROTTLE_NONE;
    }

    return snapshot->throttle;
}

bool safety_should_latch_fault(const telemetry_snapshot_t *snapshot)
{
    if (snapshot == NULL)
    {
        return true;
    }

    return (snapshot->throttle == THROTTLE_SHUTDOWN);
}

