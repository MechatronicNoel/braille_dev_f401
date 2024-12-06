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

		if( data & ( 0b10000000 << i) ){

			pin_state = SET;
		}
		else {

			pin_state = RESET;
		}

		HAL_GPIO_WritePin(CD74HC595_DS_GPIO_Port, CD74HC595_DS_Pin, pin_state);
		HAL_GPIO_WritePin(CD74HC595_SH_CP_GPIO_Port,CD74HC595_SH_CP_Pin,SET);

	}
}

void shiftOut(bool MSBFIRST, uint8_t command)
{
   for (int i = 0; i < 8; i++)
   {
       bool output = false;
       if (MSBFIRST)
       {
           output = command & 0b10000000;
           command = command << 1;
       }
       else
       {
           output = command & 0b00000001;
           command = command >> 1;
       }
       HAL_GPIO_WritePin(CD74HC595_DS_GPIO_Port, CD74HC595_DS_Pin, output);
       HAL_GPIO_WritePin(CD74HC595_SH_CP_GPIO_Port,CD74HC595_SH_CP_Pin,SET);
       HAL_GPIO_WritePin(CD74HC595_SH_CP_GPIO_Port,CD74HC595_SH_CP_Pin,RESET);

    }
}
