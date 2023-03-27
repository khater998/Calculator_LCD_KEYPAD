/*
 * app.c
 *
 *  Created on: Mar 25, 2023
 *      Author: khater
 */

#include "app.h"
#include <stdlib.h>

lcd_4bit_t board_lcd =
{
		.rs.port_id = PORTA_ID,
		.rs.pin_id  = PIN_1,
		.en.port_id = PORTA_ID,
		.en.pin_id  = PIN_2,

		.data_pin[0].port_id = PORTA_ID,
		.data_pin[0].pin_id = PIN_3,
		.data_pin[1].port_id = PORTA_ID,
		.data_pin[1].pin_id = PIN_4,
		.data_pin[2].port_id = PORTA_ID,
		.data_pin[2].pin_id = PIN_5,
		.data_pin[3].port_id = PORTA_ID,
		.data_pin[3].pin_id = PIN_6,

};




keypad_t board_keypad = {
		.rows_pins[0].port_id = PORTB_ID,
		.rows_pins[0].pin_id = PIN_4,
		.rows_pins[1].port_id = PORTB_ID,
		.rows_pins[1].pin_id = PIN_5,
		.rows_pins[2].port_id = PORTB_ID,
		.rows_pins[2].pin_id = PIN_6,
		.rows_pins[3].port_id = PORTB_ID,
		.rows_pins[3].pin_id = PIN_7,

		.columns_pins[0].port_id = PORTD_ID,
		.columns_pins[0].pin_id = PIN_2,
		.columns_pins[1].port_id = PORTD_ID,
		.columns_pins[1].pin_id = PIN_3,
		.columns_pins[2].port_id = PORTD_ID,
		.columns_pins[2].pin_id = PIN_4,
		.columns_pins[3].port_id = PORTD_ID,
		.columns_pins[3].pin_id = PIN_5,

};

static uint8 str[16] = {'\0'}; 	/* to store the entered data through keypad */
static uint8 counter = 0;		/* to track characters in the str array */

void app_init(void)
{

	/* initiate both lcd && keypad */
	LCD_4bit_init(&board_lcd);
	KEYPAD_init(&board_keypad);

}

void app_start(void)
{
	KEYPAD_getPressedKey(&board_keypad, str+counter); /* if a key is pressed store its value in the str array */

	if(str[counter] != '\0')	/* if a key is pressed the value in str won't be NULL */
	{
		/* implementation of clear button */
		if ('c' == str[counter]) /* if clear button is pressed */
		{
			for(counter = 0; counter < 16; counter++) /* reset str array to null character only */
			{
				str[counter] = '\0';
			}
			counter = 0;				/* reset counter to 0 to return to the beginning of str array*/
			LCD_4bit_clear(&board_lcd); /* then clear lcd */
		}

		/* end of clear button code */

		/* implementation of equal button */
		else if ('=' == str[counter])  /* if the equal operator key is pressed */
		{
			LCD_4bit_clear(&board_lcd);	/* clear screen to print the result next */

			/* operator_index variable will get the index of operator required
			   found flag will be set when operation is found */
			uint8 operator_index, found_flag = 0;

			/* search in  the str arr till the last entered character */
			for (operator_index = 0; operator_index < counter; operator_index++)
			{
				/* check if the element is an operator */
				if (str[operator_index] == '/' || str[operator_index] == '*' || \
						str[operator_index] == '-' ||str[operator_index] == '+')
				{
					/* found the operator, now extract the operands */
					/* operator variable will store the required operator required (*, /, +, or -)
					 * operand1[] will store the operand1 (all chars before the operator)
					 * operand2[] will store the second operand (all chars after operator and first null character
					 * op_counter variable will be used to
					 * zero_div_flag to indicate a division by zero operation*/
					uint8 operator = 0, operand1[16], operand2[16], op_counter, zero_div_flag = 0;

					/* op1 will store operand1[] integer value.
					 * op2 will store operand2[] integer value.
					   result will store the operation result */
					uint32 op1, op2, result;

					/* the current character at current index is the operator */
					operator = str[operator_index];


					/* set the found flag */
					found_flag = 1;


					/* show the detected operator  to the user */
					LCD_4bit_sendStringAtPosition(&board_lcd,2 , 1, "operator: ");
					LCD_4bit_sendCharAtPosition(&board_lcd,2 , 11, operator);

					/* store all character from the beginning of str array to the operator index in operand1[] */
					for(op_counter = 0; op_counter < operator_index; op_counter++)
					{
						operand1[op_counter] = str[op_counter];
					}

					/* store all character after operator index to the first null encountered in  operand2[] */
					for(op_counter = operator_index+1; str[op_counter] != '\0'; op_counter++)
					{
						operand2[op_counter-(operator_index+1)] = str[op_counter];
					}

					/* convert operand1 && operand2 to their numeric values */
					op1 = atoi(operand1);
					op2 = atoi(operand2);

					/* check the required operator and do the relevant calculations*/
					switch(operator)
					{
					case '*':
						result = op1 * op2;
						break;

					case '/':
						if(op2 == 0)
						{
							LCD_4bit_sendStringAtPosition(&board_lcd,2 , 1, "DIV by ZERO!!");
							_delay_ms(500);
							LCD_4bit_clear(&board_lcd);
							_delay_ms(500);
							LCD_4bit_sendStringAtPosition(&board_lcd,2 , 1, "DIV by ZERO!!");
							_delay_ms(500);
							LCD_4bit_clear(&board_lcd);
							zero_div_flag = 1;
							break;
						}
						result = op1 / op2;
						break;

					case '+':
						result = op1 + op2;
						break;

					case '-':
						result = op1 - op2;
					}
					if(1 == zero_div_flag)
					{
						/* skip printing result step*/
					}
					else
					{
						/* print the result on the screen for 2 seconds */
						LCD_4bit_setCursorLocation(&board_lcd, 1, 1);
						LCD_4bit_sendNum(&board_lcd, result);
						_delay_ms(2000);

						/* clear the LCD */
						LCD_4bit_clear(&board_lcd);
					}
					/* WE HAVE FOUND THE OPERATOR AND DONE THE OPERATION, GET OUT OF THE LOOP */
					break;
				}else{}
			}

			/* we extractred all required information from str array,
			 * so, reset it to null characters.
			 */
			for(counter = 0; counter < 16; counter++)
			{
				str[counter] = '\0';
			}


			counter = 0; /* reset counter to zero */

			/* if found_flag is still 0 "no operator was detected" */
			if (found_flag == 0)
			{
				LCD_4bit_sendStringAtPosition(&board_lcd,2 , 1, "No operator!!!");
				_delay_ms(500);
				LCD_4bit_clear(&board_lcd);
				_delay_ms(500);
				LCD_4bit_sendStringAtPosition(&board_lcd,2 , 1, "No operator!!!");
				_delay_ms(500);
				LCD_4bit_clear(&board_lcd);
			}
		}
		else
		{
			counter++;
			if (16 == counter) /* first row of the LCD is filled */
			{
				/* reset str array to null characters.
				 */
				for(counter = 0; counter < 16; counter++)
				{
					str[counter] = '\0';
				}
				counter = 0; /* reset counter to zero */

				LCD_4bit_sendStringAtPosition(&board_lcd,2 , 1, "!! OVERFLOW !!");
				_delay_ms(500);
				LCD_4bit_clear(&board_lcd);
				_delay_ms(500);
				LCD_4bit_sendStringAtPosition(&board_lcd,2 , 1, "!! OVERFLOW !!");
				_delay_ms(500);
				LCD_4bit_clear(&board_lcd);
			}

		}
		/* print the content of str array on the LCD */
		LCD_4bit_sendStringAtPosition(&board_lcd,1 , 1, str);
		_delay_ms(250); /* delay 250 ms to avoid reading the button multiple times */
	}
}

