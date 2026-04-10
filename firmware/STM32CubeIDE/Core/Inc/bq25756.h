#ifndef BQ25756_H
#define BQ25756_H

#include "main.h"
#include "pd_policy.h"

HAL_StatusTypeDef bq25756_init(I2C_HandleTypeDef *hi2c);
HAL_StatusTypeDef bq25756_configure_for_offers(const pd_offer_table_t *table);
HAL_StatusTypeDef bq25756_disable_source(void);

#endif /* BQ25756_H */

