/*
 * 74hc595.h
 *
 *  Created on: 5 ago 2024
 *      Author: Noel Dominguez
 */

#ifndef DRIVERS_74HC595_INC_74HC595_H_
#define DRIVERS_74HC595_INC_74HC595_H_

#include "stdint.h"
#include <stdbool.h>

void IC74hc595_write_byte(uint8_t data);
void shiftOut(bool MSBFIRST, uint8_t command);
#endif /* DRIVERS_74HC595_INC_74HC595_H_ */
