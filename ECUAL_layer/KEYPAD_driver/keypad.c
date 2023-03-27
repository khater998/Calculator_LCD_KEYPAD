/*
 * keypad.c
 *
 *  Created on: Mar 20, 2023
 *      Author: khater
 */


#include "../../ECUAL_layer/KEYPAD_driver/keypad.h"

#include "../../MCAL_layer/GPIO_driver/gpio.h"
#include <util/delay.h>


#if (NUMBER_OF_COLUMNS == 4)
char btn_values[NUMBER_OF_ROWS][NUMBER_OF_COLUMNS] = {
		{btn1_value, btn2_value, btn3_value, btn13_value},
		{btn4_value, btn5_value, btn6_value, btn14_value},
		{btn7_value, btn8_value, btn9_value, btn15_value},
		{btn10_value, btn11_value , btn12_value , btn16_value}
};
#elif (NUMBER_OF_COLUMNS == 3)
char btn_values[NUMBER_OF_ROWS][NUMBER_OF_COLUMNS] = {
		{btn1_value, btn2_value, btn3_value},
		{btn4_value, btn5_value, btn6_value},
		{btn7_value, btn8_value, btn9_value},
		{btn10_value, btn11_value, btn12_value}
#endif


/**
 * @brief initializes rows pins as output, columns pins as output
 * @param keypad_obj: pointer to the keypad object
 * @return status of the function:
 * 						(E_OK): the function finished successfully
 * 						(E_NOT_OK): A problem occurred in function
 */
STD_ReturnType KEYPAD_init(const keypad_t *keypad_obj)
{
	STD_ReturnType error_status = E_OK; /* to store the error value to be returned */

	if(NULL == keypad_obj)				/* check for null pointer */
	{
		error_status = E_NOT_OK;
	}
	else
	{
		int loop_counter;

		/* Make rows pins as output */
		for(loop_counter = 0; loop_counter < NUMBER_OF_ROWS; loop_counter++)
		{
			error_status = GPIO_pinDirectionInit(&(keypad_obj->rows_pins[loop_counter]), GPIO_OUTPUT);
		}

		/* Make columns pins as input */
		for(loop_counter = 0; loop_counter < NUMBER_OF_COLUMNS; loop_counter++)
		{
			error_status = GPIO_pinDirectionInit(&(keypad_obj->columns_pins[loop_counter]), GPIO_INPUT);
		}
	}
	return error_status;
}

/**
 * @brief get the pressed key value
 * @param keypad_obj: pointer to the keypad object
 * @param value: pointer to the variable that will receive the pressed value
 * @return status of the function:
 * 						(E_OK): the function finished successfully
 * 						(E_NOT_OK): A problem occurred in function
 */
STD_ReturnType KEYPAD_getPressedKey(const keypad_t *keypad_obj, uint8 *value)
{
	STD_ReturnType error_status = E_OK; /* to store the error value to be returned */

	if(NULL == keypad_obj)
	{
		error_status = E_NOT_OK;
	}
	else{
		uint8 row_counter,col_counter,state; /* state will check the logic on each column pin */
		uint8 found_flag = 0;
		/* make sure that initially all 4 rows pins logic is the same as the default of columns pins */
		for(row_counter = 0; row_counter < NUMBER_OF_ROWS; row_counter++)
		{
			error_status = GPIO_pinWrite(&(keypad_obj->rows_pins[row_counter]), KEYPAD_BUTTON_RELEASED);
		}

		/* loop through Rows pins change their logic status one at a time */
		for(row_counter = 0; row_counter < NUMBER_OF_ROWS; row_counter++)
		{
			error_status &= GPIO_pinWrite(&(keypad_obj->rows_pins[row_counter]), KEYPAD_BUTTON_PRESSED);
			_delay_ms(2);

			/* then, read all columns pin status */
			for(col_counter = 0; col_counter < NUMBER_OF_COLUMNS; col_counter++)
			{
				error_status &= GPIO_pinRead(&(keypad_obj->columns_pins[col_counter]), &state);

				/* Check if state variable changes to  KEYPAD_BUTTON_PRESSED */
				if(KEYPAD_BUTTON_PRESSED == state)
				{
					/* Store the value in the passed argument */
					*value = btn_values[row_counter][col_counter];

					/* set the found flag, break to outer loop */
					found_flag = 1;
					break;
				}else{}
			}

			/* Reset the row pin to default */
			error_status &= GPIO_pinWrite(&(keypad_obj->rows_pins[row_counter]), KEYPAD_BUTTON_RELEASED);

			/* if a key has been pressed, break to end of function */
			if (1 == found_flag)
			{
				break;
			}else{}
		}
	}
	return error_status;
}
