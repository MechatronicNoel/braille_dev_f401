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
#include "FreeRTOS.h"
#include "timers.h"
#include "cd74hc406x.h"
#include "74hc595.h"

#define UART_HANDLER huart1
#define UART_INSTANCE USART1


const uint8_t describe_tag[] = { 0xFF,0xFF,0x0A }; /* Sent to identify the display and receive amount of cells this unit has */
const uint8_t display_tag[] = { 0xFF,0xFF,0x04,0x00,0x99,0x00,0x50,0x00 }; /* Sent to request displaying of cells */

typedef enum 
{
    NVDA_CONNECTED,
    NVDA_NOT_CONNECTED,
    NVDA_FILLING_BUFFER,
    NVDA_BUFFER_FULL
}braille_driver_stm_t;

typedef struct 
{
    uint8_t data_in[3];
    uint8_t data_out[8];
    uint8_t nvda_buffer[BRAILLE_NVDA_MAX_BUFFER];
    volatile uint8_t nvda_braille_data_buffer[CELL_NUMBERS];
    uint8_t celds_state[CELL_NUMBERS];
    uint16_t nvda_data_in_counter;
    braille_driver_stm_t state;
    
}braille_dev_t;


enum{

    VIRTUAL_CELL_PRESSED,
    VIRTUAL_CELL_UNPRESSED
};

static braille_dev_t braille_dev;

/**
 * @brief Braille UART State machine
 * 
 */
static void braille_uart_handler(void){

	uint8_t temp_index = 0;

	HAL_UART_Receive_IT(&UART_HANDLER,braille_dev.data_in, UART_RX_PACKAGE_SIZE);

	switch (braille_dev.state){

	case NVDA_NOT_CONNECTED:

		for(uint8_t i = 0; i < (uint8_t) 10; i++){ /* Transmit several times in orden to validate the connection with the MCU-NVDA */

			HAL_UART_Transmit(&UART_HANDLER, braille_dev.data_out,  8U, 100U);
		}

		braille_dev.state = NVDA_CONNECTED;

		break;

	case NVDA_CONNECTED:

        if(braille_dev.data_in[0] == 0xFF){

            braille_dev.state = NVDA_FILLING_BUFFER;
            braille_dev.nvda_buffer[braille_dev.nvda_data_in_counter] = braille_dev.data_in[0];
            braille_dev.nvda_data_in_counter++;
        }

		break;


    case NVDA_FILLING_BUFFER:

        braille_dev.nvda_buffer[braille_dev.nvda_data_in_counter] = braille_dev.data_in[0];
        braille_dev.nvda_data_in_counter++;

        if( braille_dev.nvda_data_in_counter == 47U ){

            braille_dev.state = NVDA_BUFFER_FULL;
        }
        break;

    case NVDA_BUFFER_FULL:

        braille_dev.nvda_data_in_counter = 0;

        /* Save braille display data into array buffer */

        for ( uint8_t i = 9; i < (uint8_t) 48; i+= (uint8_t)2 ){

        	braille_dev.nvda_braille_data_buffer[temp_index] = braille_dev.nvda_buffer[i];
        	temp_index++;

        }

        braille_dev.state = NVDA_CONNECTED;
        break;


	default:
		break;

	}

    HAL_GPIO_TogglePin(LED_GPIO_Port,LED_Pin);


}

static void braille_app_task(void *arg){

    (void)arg;

    while(1){


        if ( braille_dev.state != NVDA_NOT_CONNECTED ){

            /* Read virtual cells state and store into buffer */
            cd74hc406x_read_all_channels(braille_dev.celds_state);

            /* Update mechanical cell when some virtual cells is pressed */
            for ( uint8_t i = 0; i < (uint8_t)16; i++ ){

                if ( braille_dev.celds_state[i] == VIRTUAL_CELL_PRESSED ){

                    HAL_GPIO_WritePin(CD74HC595_ST_CP_GPIO_Port,CD74HC595_ST_CP_Pin,RESET);
                    IC74hc595_write_byte(braille_dev.nvda_braille_data_buffer[i]);
                    HAL_GPIO_WritePin(CD74HC595_ST_CP_GPIO_Port,CD74HC595_ST_CP_Pin,SET);

                }

            }
        }

    }
    
}


braille_dev_err_t braille_dev_init(void){

	braille_dev_err_t err;

    memset(braille_dev.data_in,'0',3);

#if config_NVDA_FIRST_CONNECTION
    braille_dev.state = NVDA_NOT_CONNECTED;
#else
    braille_dev.state = NVDA_CONNECTED;
#endif

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
#if config_NVDA_FIRST_CONNECTION
    err = HAL_UART_Receive_IT(&UART_HANDLER,braille_dev.data_in,3U);
#else
    err = HAL_UART_Receive_IT(&UART_HANDLER,braille_dev.data_in,1U);
#endif

    HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin, SET);


    xTaskCreate(braille_app_task,
                "braille_app_task",
                config_BRAILLE_APP_STACK_TASK,
                NULL,
                config_BRAILLE_APP_TASK_PRIORITY,
                NULL);


    return err;

}



void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{

	if ( huart->Instance == UART_INSTANCE ){

	    braille_uart_handler();
	}

}

#endif /* SRC_BRAILLE_DRIVER_C_ */
