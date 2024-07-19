/*
 * braille_driver.h
 *
 *  Created on: 13 jul 2024
 *      Author: Noel Dominguez
 */

#ifndef INC_BRAILLE_DRIVER_H_
#define INC_BRAILLE_DRIVER_H_


#include "main.h"




typedef enum{
    BRAILLE_OK,
    BRAILLE_NOT_OK,
}braille_dev_err_t;

braille_dev_err_t braille_dev_init(void);
void braille_dev_task(void);
#endif /* INC_BRAILLE_DRIVER_H_ */
