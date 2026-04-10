#include "oled_ui.h"

#include "ltc9105_i2c.h"

#include <stdio.h>
#include <string.h>

static I2C_HandleTypeDef *g_oled_i2c = NULL;
static oled_page_t g_current_page = OLED_PAGE_INPUT;

static void clear_buffer(oled_page_buffer_t *buffer)
{
    uint8_t i;

    for (i = 0U; i < APP_OLED_LINE_COUNT; i++)
    {
        memset(buffer->lines[i], 0, APP_OLED_LINE_LENGTH);
    }
}

HAL_StatusTypeDef oled_ui_init(I2C_HandleTypeDef *hi2c)
{
    g_oled_i2c = hi2c;
    return (g_oled_i2c != NULL) ? HAL_OK : HAL_ERROR;
}

void oled_ui_next_page(void)
{
    g_current_page = (oled_page_t)(((uint32_t)g_current_page + 1U) % APP_UI_PAGE_COUNT);
}

oled_page_t oled_ui_get_current_page(void)
{
    return g_current_page;
}

void oled_ui_render(const telemetry_snapshot_t *snapshot, const pd_offer_table_t *table, oled_page_buffer_t *buffer)
{
    if ((snapshot == NULL) || (table == NULL) || (buffer == NULL))
    {
        return;
    }

    clear_buffer(buffer);

    switch (g_current_page)
    {
    case OLED_PAGE_INPUT:
        snprintf(buffer->lines[0], APP_OLED_LINE_LENGTH, "INPUT");
        snprintf(buffer->lines[1], APP_OLED_LINE_LENGTH, "%s", ltc9105_source_to_string(snapshot->poe.source));
        snprintf(buffer->lines[2], APP_OLED_LINE_LENGTH, "Bud %lumW", (unsigned long)snapshot->poe.allocated_power_mw);
        snprintf(buffer->lines[3], APP_OLED_LINE_LENGTH, "%lumV %lumA", (unsigned long)snapshot->poe.input_voltage_mv, (unsigned long)snapshot->poe.input_current_ma);
        snprintf(buffer->lines[4], APP_OLED_LINE_LENGTH, "%s", snapshot->passive_mode_enabled ? "ADV:PASSIVE" : "ADV:STD");
        snprintf(buffer->lines[5], APP_OLED_LINE_LENGTH, "%s", snapshot->poe.passive_warning ? "WARN" : "OK");
        break;

    case OLED_PAGE_PD:
        snprintf(buffer->lines[0], APP_OLED_LINE_LENGTH, "PD");
        snprintf(buffer->lines[1], APP_OLED_LINE_LENGTH, "Offers %u", table->offer_count);
        snprintf(buffer->lines[2], APP_OLED_LINE_LENGTH, "Top %umV %umA", table->active_voltage_mv, table->active_current_ma);
        snprintf(buffer->lines[3], APP_OLED_LINE_LENGTH, "Bud %lumW", (unsigned long)table->max_budget_mw);
        snprintf(buffer->lines[4], APP_OLED_LINE_LENGTH, "%s", (table->offer_count > 0U && table->offers[table->offer_count - 1U].is_epr) ? "EPR READY" : "SPR ONLY");
        snprintf(buffer->lines[5], APP_OLED_LINE_LENGTH, "%s", snapshot->profile == POWER_PROFILE_PRO ? "PRO" : "STANDARD");
        break;

    case OLED_PAGE_THERMAL:
        snprintf(buffer->lines[0], APP_OLED_LINE_LENGTH, "THERMAL");
        snprintf(buffer->lines[1], APP_OLED_LINE_LENGTH, "P %dC", snapshot->temp_primary_c);
        snprintf(buffer->lines[2], APP_OLED_LINE_LENGTH, "S %dC", snapshot->temp_secondary_c);
        snprintf(buffer->lines[3], APP_OLED_LINE_LENGTH, "Out %lumW", (unsigned long)snapshot->output.power_mw);
        snprintf(buffer->lines[4], APP_OLED_LINE_LENGTH, "%s",
                 snapshot->throttle == THROTTLE_NONE ? "NORM" :
                 snapshot->throttle == THROTTLE_SOFT ? "SOFT" :
                 snapshot->throttle == THROTTLE_HARD ? "HARD" : "FAULT");
        snprintf(buffer->lines[5], APP_OLED_LINE_LENGTH, "%s", snapshot->latched_fault ? "LATCHED" : "ACTIVE");
        break;

    case OLED_PAGE_LINK:
    default:
        snprintf(buffer->lines[0], APP_OLED_LINE_LENGTH, "LINK");
        snprintf(buffer->lines[1], APP_OLED_LINE_LENGTH, "%s", snapshot->link_up ? "UP" : "DOWN");
        snprintf(buffer->lines[2], APP_OLED_LINE_LENGTH, "%u Mbps", snapshot->link_speed_mbps);
        snprintf(buffer->lines[3], APP_OLED_LINE_LENGTH, "USB Eth only");
        snprintf(buffer->lines[4], APP_OLED_LINE_LENGTH, "No host app");
        snprintf(buffer->lines[5], APP_OLED_LINE_LENGTH, "%s", snapshot->latched_fault ? "CHECK FAULT" : "OK");
        break;
    }
}

HAL_StatusTypeDef oled_ui_flush(const oled_page_buffer_t *buffer)
{
    if ((g_oled_i2c == NULL) || (buffer == NULL))
    {
        return HAL_ERROR;
    }

    /*
     * TODO:
     * - Serialize page buffer into SSD1306 pages
     * - Push over I2C
     * - Add optional dirty-region optimization if UI complexity grows
     */
    (void)buffer;
    return HAL_OK;
}

