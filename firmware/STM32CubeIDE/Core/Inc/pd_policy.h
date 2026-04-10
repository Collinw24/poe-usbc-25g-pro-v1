#ifndef PD_POLICY_H
#define PD_POLICY_H

#include "telemetry.h"

#include <stdbool.h>
#include <stdint.h>

typedef struct
{
    uint16_t voltage_mv;
    uint16_t current_ma;
    bool is_pps;
    bool is_epr;
} pd_offer_t;

typedef struct
{
    pd_offer_t offers[APP_TPS26750_MAX_OFFERS];
    uint8_t offer_count;
    uint16_t active_voltage_mv;
    uint16_t active_current_ma;
    uint32_t max_budget_mw;
} pd_offer_table_t;

void pd_policy_build(const telemetry_snapshot_t *snapshot, pd_offer_table_t *table);
uint32_t pd_policy_calculate_budget_mw(const telemetry_snapshot_t *snapshot);

#endif /* PD_POLICY_H */

