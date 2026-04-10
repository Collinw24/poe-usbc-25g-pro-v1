#ifndef TPS26750_H
#define TPS26750_H

#include "main.h"
#include "pd_policy.h"

HAL_StatusTypeDef tps26750_init(I2C_HandleTypeDef *hi2c);
HAL_StatusTypeDef tps26750_apply_offer_table(const pd_offer_table_t *table);
HAL_StatusTypeDef tps26750_disable_source(void);
HAL_StatusTypeDef tps26750_force_renegotiation(void);

#endif /* TPS26750_H */

