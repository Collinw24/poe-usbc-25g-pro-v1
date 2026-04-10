#ifndef BUTTONS_H
#define BUTTONS_H

#include "main.h"

#include <stdbool.h>

typedef enum
{
    BUTTON_EVENT_NONE = 0,
    BUTTON_EVENT_LEFT_SHORT,
    BUTTON_EVENT_RIGHT_SHORT,
    BUTTON_EVENT_RIGHT_LONG
} button_event_t;

typedef struct
{
    GPIO_TypeDef *left_port;
    uint16_t left_pin;
    GPIO_TypeDef *right_port;
    uint16_t right_pin;
    bool left_active_low;
    bool right_active_low;
} buttons_hw_t;

void buttons_init(const buttons_hw_t *hw);
void buttons_poll(uint32_t now_ms);
button_event_t buttons_take_event(void);
bool buttons_boot_combo_detected(uint32_t sample_window_ms);

#endif /* BUTTONS_H */

