/*
 * app.h
 *
 *  Created on: Mar 25, 2023
 *      Author: khater
 */

#ifndef APPLICATION_LAYER_APP_H_
#define APPLICATION_LAYER_APP_H_

/***********************************************************************************************/
/*								  Section: INCLUDES                                            */
/***********************************************************************************************/

#include <util/delay.h>
#include "../STD_types.h"
#include "../Common_Macros.h"
#include "../MCAL_layer/GPIO_driver/gpio.h"
#include "../ECUAL_layer/KEYPAD_driver/keypad.h"
#include "../ECUAL_layer/LCD_driver/lcd.h"


/***********************************************************************************************/
/*								  Section: FUNCTION DECLARATIONS                               */
/***********************************************************************************************/

void app_init(void);
void app_start(void);


#endif /* APPLICATION_LAYER_APP_H_ */
