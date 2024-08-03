/*
 * braille_driver_config.h
 *
 *  Created on: 13 jul 2024
 *      Author: Noel Dominguez
 */

#ifndef INC_BRAILLE_DRIVER_CONFIG_H_
#define INC_BRAILLE_DRIVER_CONFIG_H_

#define BRAILLE_NVDA_MAX_BUFFER     48U
#define CELL_NUMBERS				20U
#define UART_RX_PACKAGE_SIZE        1U
#define NVDA_RX_FRAMES_TIME_OUT     1000U

/* Reflash the micro and changue the macro when the micro connect to NVDA
   Temporary workaround */
#define config_NVDA_FIRST_CONNECTION	0U /* Only valid in dev mode 
                                            0 = Connection established with NVDA 
                                            1 = No Connection established with NVDA*/
#endif /* INC_BRAILLE_DRIVER_CONFIG_H_ */
