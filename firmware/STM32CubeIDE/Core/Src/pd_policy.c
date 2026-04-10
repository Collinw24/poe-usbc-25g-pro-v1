#include "pd_policy.h"

#include "app_config.h"

static void add_offer(pd_offer_table_t *table, uint16_t mv, uint16_t ma, bool is_pps, bool is_epr)
{
    if ((table == NULL) || (table->offer_count >= APP_TPS26750_MAX_OFFERS))
    {
        return;
    }

    table->offers[table->offer_count].voltage_mv = mv;
    table->offers[table->offer_count].current_ma = ma;
    table->offers[table->offer_count].is_pps = is_pps;
    table->offers[table->offer_count].is_epr = is_epr;
    table->offer_count++;
}

static uint32_t apply_headroom(uint32_t budget_mw, power_profile_t profile)
{
    const uint32_t reserve = (profile == POWER_PROFILE_PRO) ? APP_PRO_HEADROOM_PERCENT : APP_STANDARD_HEADROOM_PERCENT;
    return (budget_mw * (100U - reserve)) / 100U;
}

uint32_t pd_policy_calculate_budget_mw(const telemetry_snapshot_t *snapshot)
{
    uint32_t budget_mw;
    int16_t hottest_c;

    if (snapshot == NULL)
    {
        return 0U;
    }

    switch (snapshot->poe.source)
    {
    case LTC9105_SOURCE_AF:
        budget_mw = 10000U;
        break;
    case LTC9105_SOURCE_AT:
        budget_mw = 22500U;
        break;
    case LTC9105_SOURCE_BT_TYPE3:
        budget_mw = 45000U;
        break;
    case LTC9105_SOURCE_BT_TYPE4:
        budget_mw = 71300U;
        break;
    case LTC9105_SOURCE_PASSIVE_48V:
        budget_mw = 80000U;
        break;
    case LTC9105_SOURCE_UNKNOWN:
    default:
        budget_mw = 5000U;
        break;
    }

    if ((snapshot->poe.allocated_power_mw != 0U) && (snapshot->poe.allocated_power_mw < budget_mw))
    {
        budget_mw = snapshot->poe.allocated_power_mw;
    }

    hottest_c = (snapshot->temp_primary_c > snapshot->temp_secondary_c) ? snapshot->temp_primary_c : snapshot->temp_secondary_c;

    if (hottest_c >= APP_TEMP_SHUTDOWN_C)
    {
        return 0U;
    }

    if (hottest_c >= APP_TEMP_HARD_DERATE_C)
    {
        return APP_CLAMP_POWER_MW;
    }

    if (hottest_c >= APP_TEMP_SOFT_DERATE_C)
    {
        budget_mw = (budget_mw * (100U - APP_SOFT_DERATE_PERCENT)) / 100U;
    }

    return apply_headroom(budget_mw, snapshot->profile);
}

void pd_policy_build(const telemetry_snapshot_t *snapshot, pd_offer_table_t *table)
{
    uint32_t budget_mw;

    if ((snapshot == NULL) || (table == NULL))
    {
        return;
    }

    table->offer_count = 0U;
    table->active_voltage_mv = 5000U;
    table->active_current_ma = 500U;
    table->max_budget_mw = 0U;

    budget_mw = pd_policy_calculate_budget_mw(snapshot);
    table->max_budget_mw = budget_mw;

    if (budget_mw == 0U)
    {
        return;
    }

    add_offer(table, 5000U, 2000U, false, false);

    if (budget_mw >= 15000U)
    {
        add_offer(table, 9000U, 2000U, false, false);
    }

    if (budget_mw >= 22500U)
    {
        add_offer(table, 15000U, 1500U, false, false);
    }

    if (budget_mw >= 45000U)
    {
        add_offer(table, 20000U, (uint16_t)((budget_mw * 1000U) / 20000U), false, false);
    }

    if (budget_mw >= 60000U)
    {
        add_offer(table, 20000U, 3250U, false, false);
    }

    if ((budget_mw >= 63000U) &&
        (snapshot->profile == POWER_PROFILE_PRO) &&
        (snapshot->poe.source == LTC9105_SOURCE_BT_TYPE4 || snapshot->poe.source == LTC9105_SOURCE_PASSIVE_48V))
    {
        add_offer(table, 28000U, 2250U, false, true);
    }

    if (table->offer_count > 0U)
    {
        const pd_offer_t *top = &table->offers[table->offer_count - 1U];
        table->active_voltage_mv = top->voltage_mv;
        table->active_current_ma = top->current_ma;
    }
}
