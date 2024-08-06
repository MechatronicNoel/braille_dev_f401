/*
 * 74hc595 .c
 *
 *  Created on: 5 ago 2024
 *      Author: Noel Dominguez
 */

#include "74hc595.h"
#include "main.h"
#include <stdbool.h>

void IC74hc595_write_byte(uint8_t data){

	bool pin_state = false;

    // start data load
	for( int i = 7; i >= 0; i--){

		HAL_GPIO_WritePin(CD74HC595_SH_CP_GPIO_Port,CD74HC595_SH_CP_Pin,RESET);

		if( data & ( 1 << i) ){

			pin_state = SET;
		}
		else {

			pin_state = RESET;
		}

		HAL_GPIO_WritePin(CD74HC595_DS_GPIO_Port, CD74HC595_DS_Pin, pin_state);
		HAL_GPIO_WritePin(CD74HC595_SH_CP_GPIO_Port,CD74HC595_SH_CP_Pin,SET);

	}
}
