#ifndef OLED_UI_H
#define OLED_UI_H

#include "telemetry.h"
#include "pd_policy.h"

typedef enum
{
    OLED_PAGE_INPUT = 0,
    OLED_PAGE_PD,
    OLED_PAGE_THERMAL,
    OLED_PAGE_LINK
} oled_page_t;

typedef struct
{
    char lines[APP_OLED_LINE_COUNT][APP_OLED_LINE_LENGTH];
} oled_page_buffer_t;

HAL_StatusTypeDef oled_ui_init(I2C_HandleTypeDef *hi2c);
void oled_ui_next_page(void);
oled_page_t oled_ui_get_current_page(void);
void oled_ui_render(const telemetry_snapshot_t *snapshot, const pd_offer_table_t *table, oled_page_buffer_t *buffer);
HAL_StatusTypeDef oled_ui_flush(const oled_page_buffer_t *buffer);

#endif /* OLED_UI_H */

