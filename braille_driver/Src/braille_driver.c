/*
 * braille_driver.c
 *
 *  Created on: 13 jul 2024
 *      Author: Noel Dominguez
 */

#ifndef SRC_BRAILLE_DRIVER_C_
#define SRC_BRAILLE_DRIVER_C_

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "braille_driver.h"
#include "usart.h"
#include "braille_driver_config.h"


#define UART_HANDLER huart1
#define UART_INSTANCE USART1

const uint8_t describe_tag[] = {0xFF,0xFF,0x0A}; /* Sent to identify the display and receive amount of cells this unit has */

typedef enum 
{
    NVDA_CONNECTED,
    NVDA_NOT_CONNECTED
}braille_driver_stm_t;

typedef struct 
{
    uint8_t data_in[BRAILLE_CELLS_MAX_BUFFER];
    uint8_t data_out[8];
    bool uart_data_ready;
    braille_driver_stm_t state;
    
}braille_dev_t;


static braille_dev_t braille_dev;

braille_dev_err_t braille_dev_init(void){

	braille_dev_err_t err;

    memset(braille_dev.data_in,'0',BRAILLE_CELLS_MAX_BUFFER);
    braille_dev.state = NVDA_NOT_CONNECTED;
    braille_dev.uart_data_ready = false;
    /* Prepare NVDA response message */
    braille_dev.data_out[0] = 0x00;
    braille_dev.data_out[1] = 0x05;
    braille_dev.data_out[2] = CELL_NUMBERS;
    braille_dev.data_out[3] = 0xFF;
    braille_dev.data_out[4] = 0xFF;
    braille_dev.data_out[5] = 0xFF;
    braille_dev.data_out[6] = 0xFF;
    braille_dev.data_out[7] = 0xFF;

    /* Prepare IRQ for NVDA-Braille dev setup */
    err = HAL_UART_Receive_IT(&UART_HANDLER,braille_dev.data_in,3U);
//    err = HAL_UART_Receive_IT(&UART_HANDLER,braille_dev.data_in,BRAILLE_CELLS_MAX_BUFFER);
    HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin, SET);
    return err;

}


static void braille_uart_handler(void){

	switch (braille_dev.state){

	case NVDA_NOT_CONNECTED:

		for(uint8_t i = 0; i < (uint8_t) 10; i++){ /* Transmit several times in orden to valiadate the connecion with the MCU-NVDA */
			HAL_UART_Transmit(&UART_HANDLER, braille_dev.data_out,  8U, 100U);
		}

		HAL_UART_Receive_IT(&UART_HANDLER,braille_dev.data_in, CELL_NUMBERS);
		braille_dev.state = NVDA_CONNECTED;

		break;

	case NVDA_CONNECTED:

		HAL_UART_Receive_IT(&UART_HANDLER,braille_dev.data_in, UART_RX_PACKAGE_SIZE);
		break;

	default:
		break;

	}

    HAL_GPIO_TogglePin(LED_GPIO_Port,LED_Pin);


}
void braille_dev_task(void){


}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{

	if ( huart->Instance == UART_INSTANCE ){

	    braille_uart_handler();
	}


}

#endif /* SRC_BRAILLE_DRIVER_C_ */
