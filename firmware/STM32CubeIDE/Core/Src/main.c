#include "main.h"

#include "app_config.h"
#include "bq25756.h"
#include "buttons.h"
#include "dfu_hooks.h"
#include "ltc9105_i2c.h"
#include "oled_ui.h"
#include "pd_policy.h"
#include "safety.h"
#include "telemetry.h"
#include "tps26750.h"

#include <string.h>

I2C_HandleTypeDef hi2c1;
I2C_HandleTypeDef hi2c2;
TIM_HandleTypeDef htim3;

static telemetry_snapshot_t g_snapshot;
static pd_offer_table_t g_offer_table;

static void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_I2C2_Init(void);
static void MX_TIM3_Init(void);
static void process_button_event(button_event_t event);
static void apply_pd_policy(bool force_renegotiate);

int main(void)
{
    uint32_t last_ui_ms = 0U;
    uint32_t last_local_ms = 0U;
    uint32_t last_poe_ms = 0U;
    uint32_t last_policy_ms = 0U;

    HAL_Init();
    SystemClock_Config();

    MX_GPIO_Init();
    MX_I2C1_Init();
    MX_I2C2_Init();
    MX_TIM3_Init();

    memset(&g_snapshot, 0, sizeof(g_snapshot));
    memset(&g_offer_table, 0, sizeof(g_offer_table));
    buttons_hw_t buttons_hw = {
        .left_port = GPIOA,
        .left_pin = GPIO_PIN_0,
        .right_port = GPIOA,
        .right_pin = GPIO_PIN_1,
        .left_active_low = true,
        .right_active_low = true
    };
    buttons_init(&buttons_hw);
    g_snapshot.profile = POWER_PROFILE_STANDARD;
    g_snapshot.passive_mode_enabled = buttons_boot_combo_detected(APP_BOOT_COMBO_WINDOW_MS);

    if (dfu_should_enter_bootloader())
    {
        dfu_jump_to_bootloader();
    }

    if (telemetry_init(&hi2c1, &hi2c2) != HAL_OK)
    {
        Error_Handler();
    }

    if (tps26750_init(&hi2c2) != HAL_OK || bq25756_init(&hi2c2) != HAL_OK || oled_ui_init(&hi2c2) != HAL_OK)
    {
        Error_Handler();
    }

    apply_pd_policy(false);

    while (1)
    {
        const uint32_t now_ms = HAL_GetTick();
        button_event_t event;
        oled_page_buffer_t page_buffer;

        buttons_poll(now_ms);
        event = buttons_take_event();
        if (event != BUTTON_EVENT_NONE)
        {
            process_button_event(event);
        }

        if ((now_ms - last_local_ms) >= 100U)
        {
            (void)telemetry_refresh_local(&g_snapshot);
            g_snapshot.throttle = safety_evaluate(&g_snapshot);
            last_local_ms = now_ms;
        }

        if ((now_ms - last_poe_ms) >= 250U)
        {
            (void)telemetry_refresh_poe(&g_snapshot);
            last_poe_ms = now_ms;
        }

        if ((now_ms - last_policy_ms) >= 500U)
        {
            apply_pd_policy(false);
            last_policy_ms = now_ms;
        }

        if ((now_ms - last_ui_ms) >= 10U)
        {
            oled_ui_render(&g_snapshot, &g_offer_table, &page_buffer);
            (void)oled_ui_flush(&page_buffer);
            last_ui_ms = now_ms;
        }
    }
}

static void process_button_event(button_event_t event)
{
    switch (event)
    {
    case BUTTON_EVENT_LEFT_SHORT:
        oled_ui_next_page();
        break;

    case BUTTON_EVENT_RIGHT_SHORT:
        apply_pd_policy(true);
        break;

    case BUTTON_EVENT_RIGHT_LONG:
        g_snapshot.profile = (g_snapshot.profile == POWER_PROFILE_STANDARD) ? POWER_PROFILE_PRO : POWER_PROFILE_STANDARD;
        apply_pd_policy(true);
        break;

    case BUTTON_EVENT_NONE:
    default:
        break;
    }
}

static void apply_pd_policy(bool force_renegotiate)
{
    if (g_snapshot.latched_fault)
    {
        (void)bq25756_disable_source();
        (void)tps26750_disable_source();
        return;
    }

    pd_policy_build(&g_snapshot, &g_offer_table);
    (void)bq25756_configure_for_offers(&g_offer_table);
    (void)tps26750_apply_offer_table(&g_offer_table);

    if (force_renegotiate)
    {
        (void)tps26750_force_renegotiation();
    }

    if (safety_should_latch_fault(&g_snapshot))
    {
        g_snapshot.latched_fault = true;
        (void)bq25756_disable_source();
        (void)tps26750_disable_source();
    }
}

static void SystemClock_Config(void)
{
    /* TODO: Replace with CubeMX-generated clock tree. */
}

static void MX_GPIO_Init(void)
{
    /* TODO: Replace with CubeMX-generated GPIO initialization. */
}

static void MX_I2C1_Init(void)
{
    /* I2C1: isolated PoE telemetry path */
}

static void MX_I2C2_Init(void)
{
    /* I2C2: local telemetry, OLED, PD controller programming */
}

static void MX_TIM3_Init(void)
{
    /* Optional heartbeat / UI timing source */
}

void Error_Handler(void)
{
    __disable_irq();
    while (1)
    {
    }
}
