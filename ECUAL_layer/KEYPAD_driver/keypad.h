/*
 * keypad.h
 *
 *  Created on: Mar 20, 2023
 *      Author: khater
 */

#ifndef ECUAL_LAYER_KEYPAD_DRIVER_KEYPAD_H_
#define ECUAL_LAYER_KEYPAD_DRIVER_KEYPAD_H_


/***********************************************************************************************/
/*								  Section: INCLUDES                                            */
/***********************************************************************************************/
#include "../../STD_types.h"
#include "../../Common_Macros.h"
#include "../../MCAL_layer/GPIO_driver/gpio.h"

/***********************************************************************************************/
/*								  Section: MACRO DECLARATIONS                                  */
/***********************************************************************************************/

#define NUMBER_OF_ROWS 		4
#define NUMBER_OF_COLUMNS 	4
#define KEYPAD_ACTIVE_HIGH  0
/* KEYPAD_ACTIVE_HIGH must be set to 1 when columns pins are connected to ground,
 * so when a button is pressed it sends a HIGH signal.
 *
 * KEYPAD_ACTIVE_HIGH must be set to 0 when columns pins are connected to 5v,
 * so when a button is pressed it sends a LOW signal.
 */
#if(KEYPAD_ACTIVE_HIGH == 1)

	#define KEYPAD_BUTTON_PRESSED 		GPIO_HIGH
	#define KEYPAD_BUTTON_RELEASED 		GPIO_LOW

#elif(KEYPAD_ACTIVE_HIGH == 0)

	#define KEYPAD_BUTTON_PRESSED 		GPIO_LOW
	#define KEYPAD_BUTTON_RELEASED 		GPIO_HIGH

#else

	#error "Undefined Keypad Configuration"

#endif


#define btn1_value		'7'
#define btn2_value		'8'
#define btn3_value 		'9'
#define btn4_value		'4'
#define btn5_value 		'5'
#define btn6_value		'6'
#define btn7_value 		'1'
#define btn8_value		'2'
#define btn9_value 		'3'
#define btn10_value		'c'
#define btn11_value 	'0'
#define btn12_value		'='

#if	(NUMBER_OF_COLUMNS == 4)
#define btn13_value 	'/'
#define btn14_value		'*'
#define btn15_value		'-'
#define btn16_value		'+'
#endif

/***********************************************************************************************/
/*								  Section: MACRO FUNCTIONS DECLARATIONS                        */
/***********************************************************************************************/




/***********************************************************************************************/
/*								  Section: DATA TYPE DECLARATIONS                              */
/***********************************************************************************************/

typedef struct{
	/** keypad takes NUMBER_OF_ROWS pins as output	@ref NUMBER_OF_ROWS*/
	pin_obj_t rows_pins[NUMBER_OF_ROWS];		/** @ref pin_obj_t */

	/** keypad takes NUMBER_OF_COLUMS pins as input	@ref NUMBER_OF_COLUMS*/
	pin_obj_t columns_pins[NUMBER_OF_COLUMNS];
}keypad_t;


/** keypad button mapping for 4x3 keypad
 * 							btn1	btn2	btn3
 * 							btn4	btn5	btn6
 *  						btn7 	btn8 	btn9
 *  						btn10	btn11	btn12
 *
 * 	keypad button mapping for 4x4 keypad
 * 							btn1	btn2	btn3	btn13
 * 							btn4	btn5	btn6	btn14
 *  						btn7 	btn8 	btn9	btn15
 *  						btn10	btn11	btn12	btn16
 */



/***********************************************************************************************/
/*								  Section: FUNCTION DECLARATIONS                               */
/***********************************************************************************************/
/**
 * @brief initializes rows pins as output, columns pins as output
 * @param keypad_obj: pointer to the keypad object
 * @return status of the function:
 * 						(E_OK): the function finished successfully
 * 						(E_NOT_OK): A problem occurred in function
 */
STD_ReturnType KEYPAD_init(const keypad_t *keypad_obj);

/**
 * @brief get the pressed key value
 * @param keypad_obj: pointer to the keypad object
 * @param value: pointer to the variable that will receive the pressed value
 * @return status of the function:
 * 						(E_OK): the function finished successfully
 * 						(E_NOT_OK): A problem occurred in function
 */
STD_ReturnType KEYPAD_getPressedKey(const keypad_t *keypad_obj, uint8 *value);


#endif /* ECUAL_LAYER_KEYPAD_DRIVER_KEYPAD_H_ */
