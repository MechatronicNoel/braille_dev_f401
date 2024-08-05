/*
 * cd74hc406x.c
 *
 *  Created on: 4 ago 2024
 *      Author: Noel Dominguez
 */


#include "cd74hc406x.h"

#define BIT_READ(value, bit) (((value) >> (bit)) & 0x01)


static const uint8_t cd74hc406x_channels[] = {

    0x00,
    0x08,
    0x04,
    0x0C,
    0x02,
    0x0A,
    0x06,
    0x0E,
    0x01,
    0x09,
    0x05,
    0x0D,
    0x03,
    0x0B,
    0x07,
    0x0F

};

uint32_t cd74hc406x_read_channel(uint8_t channel){


    uint32_t pin_mask = 0;
    uint8_t mux_value = cd74hc406x_channels[channel];
    
    pin_mask |= (BIT_READ(mux_value, 3) ? CD74HCT4067_S0_Pin : CD74HCT4067_S0_Pin << 16);
    pin_mask |= (BIT_READ(mux_value, 2) ? CD74HCT4067_S1_Pin : CD74HCT4067_S1_Pin << 16);
    pin_mask |= (BIT_READ(mux_value, 1) ? CD74HCT4067_S2_Pin : CD74HCT4067_S2_Pin << 16);
    pin_mask |= (BIT_READ(mux_value, 0) ? CD74HCT4067_S3_Pin : CD74HCT4067_S3_Pin << 16);
    
    HAL_GPIO_WritePin(GPIOB,pin_mask,SET);

    return HAL_GPIO_ReadPin(CD74HCT4067_S_GPIO_Port,CD74HCT4067_S_Pin);

}

void cd74hc406x_read_all_channels(uint32_t *channels){

	for(uint8_t i = 0; i < 16; i++){

		channels[i] = cd74hc406x_read_channel(i);
		HAL_Delay(200);

	}

}
