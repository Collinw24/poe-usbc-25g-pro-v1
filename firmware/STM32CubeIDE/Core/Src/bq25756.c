#include "bq25756.h"

static I2C_HandleTypeDef *g_bq25756_i2c = NULL;

HAL_StatusTypeDef bq25756_init(I2C_HandleTypeDef *hi2c)
{
    g_bq25756_i2c = hi2c;
    return (g_bq25756_i2c != NULL) ? HAL_OK : HAL_ERROR;
}

HAL_StatusTypeDef bq25756_configure_for_offers(const pd_offer_table_t *table)
{
    uint16_t highest_mv = 5000U;
    uint16_t highest_ma = 500U;

    if ((g_bq25756_i2c == NULL) || (table == NULL))
    {
        return HAL_ERROR;
    }

    if (table->offer_count > 0U)
    {
        const pd_offer_t *top = &table->offers[table->offer_count - 1U];
        highest_mv = top->voltage_mv;
        highest_ma = top->current_ma;
    }

    /*
     * TODO:
     * - Program reverse-mode output voltage/current
     * - Align source rail with highest offered PDO
     * - Verify transition handling between 20V SPR and 28V EPR modes
     */
    (void)highest_mv;
    (void)highest_ma;
    return HAL_OK;
}

HAL_StatusTypeDef bq25756_disable_source(void)
{
    if (g_bq25756_i2c == NULL)
    {
        return HAL_ERROR;
    }

    /* TODO: disable reverse/source mode cleanly. */
    return HAL_OK;
}

