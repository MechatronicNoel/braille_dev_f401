/*
 * cd74hc406x.h
 *
 *  Created on: 4 ago 2024
 *      Author: Noel Dominguez
 */

#ifndef DRIVERS_CD74HC406X_INC_CD74HC406X_H_
#define DRIVERS_CD74HC406X_INC_CD74HC406X_H_

#include "main.h"

uint32_t cd74hc406x_read_channel(uint8_t channel);
void cd74hc406x_read_all_channels(uint32_t *channels);

#endif /* DRIVERS_CD74HC406X_INC_CD74HC406X_H_ */
