#include "buttons.h"

#include "app_config.h"

typedef struct
{
    GPIO_TypeDef *port;
    uint16_t pin;
    bool active_low;
    bool last_sample;
    bool stable_state;
    bool long_sent;
    uint32_t last_change_ms;
    uint32_t press_start_ms;
} button_state_t;

static button_state_t g_left;
static button_state_t g_right;
static button_event_t g_pending_event = BUTTON_EVENT_NONE;

static bool read_button(const button_state_t *state)
{
    GPIO_PinState raw = HAL_GPIO_ReadPin(state->port, state->pin);
    bool asserted = (raw == GPIO_PIN_SET);

    return state->active_low ? !asserted : asserted;
}

static void service_button(button_state_t *state, uint32_t now_ms, bool is_right)
{
    const bool sample = read_button(state);

    if (sample != state->last_sample)
    {
        state->last_sample = sample;
        state->last_change_ms = now_ms;
    }

    if ((now_ms - state->last_change_ms) < APP_BUTTON_DEBOUNCE_MS)
    {
        return;
    }

    if (sample != state->stable_state)
    {
        state->stable_state = sample;
        if (sample)
        {
            state->press_start_ms = now_ms;
            state->long_sent = false;
        }
        else if (!state->long_sent)
        {
            g_pending_event = is_right ? BUTTON_EVENT_RIGHT_SHORT : BUTTON_EVENT_LEFT_SHORT;
        }
    }

    if (state->stable_state && !state->long_sent && is_right &&
        ((now_ms - state->press_start_ms) >= APP_BUTTON_LONG_PRESS_MS))
    {
        state->long_sent = true;
        g_pending_event = BUTTON_EVENT_RIGHT_LONG;
    }
}

void buttons_init(const buttons_hw_t *hw)
{
    g_left.port = hw->left_port;
    g_left.pin = hw->left_pin;
    g_left.active_low = hw->left_active_low;

    g_right.port = hw->right_port;
    g_right.pin = hw->right_pin;
    g_right.active_low = hw->right_active_low;
}

void buttons_poll(uint32_t now_ms)
{
    service_button(&g_left, now_ms, false);
    service_button(&g_right, now_ms, true);
}

button_event_t buttons_take_event(void)
{
    const button_event_t event = g_pending_event;
    g_pending_event = BUTTON_EVENT_NONE;
    return event;
}

bool buttons_boot_combo_detected(uint32_t sample_window_ms)
{
    const uint32_t start = HAL_GetTick();

    while ((HAL_GetTick() - start) < sample_window_ms)
    {
        if (read_button(&g_left) && read_button(&g_right))
        {
            return true;
        }
    }

    return false;
}

