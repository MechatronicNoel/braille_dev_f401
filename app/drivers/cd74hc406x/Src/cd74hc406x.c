/*
 * cd74hc406x.c
 *
 *  Created on: 4 ago 2024
 *      Author: Noel Dominguez
 */


#include "cd74hc406x.h"

const int select_pins[] = { CD74HCT4067_S0_Pin, CD74HCT4067_S1_Pin, CD74HCT4067_S2_Pin ,CD74HCT4067_S3_Pin };

/**
 * @brief 
 * 
 * @param channel 
 */
static void cd74hc406x_select_channel(uint8_t channel){

	for (uint8_t i = 0; i < (uint8_t)4; i++)
	{
		if (channel & (1<<i))
			HAL_GPIO_WritePin(GPIOB, select_pins[i], GPIO_PIN_SET);
		else
			HAL_GPIO_WritePin(GPIOB, select_pins[i], GPIO_PIN_RESET);
	}

}


void cd74hc406x_read_all_channels(uint8_t *channels){

	for(uint8_t i = 0; i < (uint8_t) 16; i++){

		cd74hc406x_select_channel(i);
		channels[i] = (uint8_t) HAL_GPIO_ReadPin(CD74HCT4067_S_GPIO_Port,CD74HCT4067_S_Pin);

	}

}

