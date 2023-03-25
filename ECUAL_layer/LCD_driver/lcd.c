/*
 * lcd.c
 *
 *  Created on: Mar 23, 2023
 *      Author: khater
 */


#include "../../ECUAL_layer/LCD_driver/lcd.h"
#include "../../STD_types.h"


#include <util/delay.h>
#include <stdio.h>

static STD_ReturnType LCD_4bit_genEnablePulse(const lcd_4bit_t *lcd);
static STD_ReturnType LCD_send4Bits(const lcd_4bit_t *lcd, uint8 data_command);
static STD_ReturnType LCD_8bit_genEnablePulse(const lcd_8bit_t *lcd);
static STD_ReturnType numToStr(const uint32 num, char * str);



/***********************************************************************************************/
/*								  Function: LCD_4bit_init                                      */
/***********************************************************************************************/
/**
 * @brief Initializes LCD control and data pins as output
 * @param lcd: pointer to the LCD object to be controlled
 * @return status of the function:
 * 						(E_OK): the function finished successfully
 * 						(E_NOT_OK): A problem occurred in function
 */
STD_ReturnType LCD_4bit_init(const lcd_4bit_t *lcd)
{
	STD_ReturnType error_status = E_OK;
	if(NULL == lcd)
	{
		error_status = E_NOT_OK;
	}
	else
	{
		uint8 data_pin_counter = 0; /* To loop through the 4 data pins */
		error_status = GPIO_pinDirectionInit(&(lcd->rs), GPIO_OUTPUT);  /* make RS output pin*/
		error_status &= GPIO_pinDirectionInit(&(lcd->en), GPIO_OUTPUT); /* make EN output pin */

		for(; data_pin_counter < 4; data_pin_counter++)
		{
			error_status &= GPIO_pinDirectionInit(&(lcd->data_pin[data_pin_counter]), GPIO_OUTPUT);
		}

		/* generate a delay of 20 msec at  startup*/
		_delay_ms(20);

		error_status &= LCD_4bit_sendCommand(lcd, 0x33);	/* required 4-bit mode initialization command */
		error_status &= LCD_4bit_sendCommand(lcd, 0x32);	/* required 4-bit mode initialization command */

		error_status &= LCD_4bit_sendCommand(lcd, _LCD_4BIT_MODE_2_LINE); /* set 4 bit mode 2 line */
		_delay_ms(5);
		error_status &= LCD_4bit_sendCommand(lcd, _LCD_4BIT_MODE_2_LINE);
		_delay_us(150);
		error_status &= LCD_4bit_sendCommand(lcd, _LCD_4BIT_MODE_2_LINE);

		error_status &= LCD_4bit_sendCommand(lcd, _LCD_CURSOR_OFF_DISPLAY_ON);
		error_status &= LCD_4bit_sendCommand(lcd, _LCD_ENTRY_MODE);
		error_status &= LCD_4bit_sendCommand(lcd, _LCD_CLEAR);

	}
	return error_status;
}

/***********************************************************************************************/
/*								  Function: LCD_4bit_clear                                     */
/***********************************************************************************************/
/**
 * @brief Clears LCD and return cursor to home position
 * @param lcd: pointer to the LCD object to be controlled
 * @return status of the function:
 * 						(E_OK): the function finished successfully
 * 						(E_NOT_OK): A problem occurred in function
 */
STD_ReturnType LCD_4bit_clear(const lcd_4bit_t *lcd)
{
	STD_ReturnType error_status = E_OK;
	if(NULL == lcd)
	{
		error_status = E_NOT_OK;
	}
	else
	{
		LCD_4bit_sendCommand(lcd, _LCD_CLEAR);
	}
	return error_status;
}


/***********************************************************************************************/
/*								  Function: LCD_4bit_sendCommand                               */
/***********************************************************************************************/
/**
 * @brief Send a command to the LCD controller
 * @param lcd: pointer to the LCD object to be controlled
 * @param command: The command to be sent to the LCD
 * @return status of the function:
 * 						(E_OK): the function finished successfully
 * 						(E_NOT_OK): A problem occurred in function
 */
STD_ReturnType LCD_4bit_sendCommand(const lcd_4bit_t *lcd, uint8 command)
{
	STD_ReturnType error_status = E_OK;
	if(NULL == lcd)
	{
		error_status = E_NOT_OK;
	}
	else
	{
		/* Write 0 on the RS pin */
		error_status &= GPIO_pinWrite(&(lcd->rs), GPIO_LOW);

		/* Send the higher command nibble first */
		error_status &= LCD_send4Bits(lcd, (command >> 4));	/* Send the higher command nibble first */
		error_status &= LCD_4bit_genEnablePulse(lcd);			/* generate a low pulse on the enable pin */

		error_status &= LCD_send4Bits(lcd, command);			/* Send the lower command nibble */
		error_status &= LCD_4bit_genEnablePulse(lcd);			/* generate a low pulse on the enable pin */

		_delay_ms(2);
	}
	return error_status;
}

/***********************************************************************************************/
/*								  Function: LCD_4bit_sendChar                                  */
/***********************************************************************************************/
/**
 * @brief Prints a character on the LCD
 * @param lcd: pointer to the LCD object to be controlled
 * @param _char: The character to be printed on the LCD
 * @return status of the function:
 * 						(E_OK): the function finished successfully
 * 						(E_NOT_OK): A problem occurred in function
 */
STD_ReturnType LCD_4bit_sendChar(const lcd_4bit_t *lcd, uint8 _char)
{
	STD_ReturnType error_status = E_OK;
	if(NULL == lcd)
	{
		error_status = E_NOT_OK;
	}
	else
	{
		/* Write 0 on the RS pin */
		error_status &= GPIO_pinWrite(&(lcd->rs), GPIO_HIGH);

		/* Send the higher command nibble first */
		error_status &= LCD_send4Bits(lcd, (_char >> 4));		/* Send the higher char nibble first */
		error_status &= LCD_4bit_genEnablePulse(lcd);			/* generate a low pulse on the enable pin */

		error_status &= LCD_send4Bits(lcd, _char);				/* Send the lower char nibble */
		error_status &= LCD_4bit_genEnablePulse(lcd);			/* generate a low pulse on the enable pin */

		_delay_ms(2);

	}

	return error_status;
}


/***********************************************************************************************/
/*								  Function: LCD_4bit_sendCharAtPosition                                  */
/***********************************************************************************************/
/**
 * @brief Prints a character on the LCD
 * @param lcd: pointer to the LCD object to be controlled
 * @param row: The row index of the cursor. possible values are 1, 2, 3, or 4.
 * @param col: The column index of the cursor. possible values are 0 to 80. (80 to cover one-line mode)
 * @param _char: The character to be printed on the LCD
 * @return status of the function:
 * 						(E_OK): the function finished successfully
 * 						(E_NOT_OK): A problem occurred in function
 */
STD_ReturnType LCD_4bit_sendCharAtPosition(const lcd_4bit_t *lcd, uint8 row, uint8 col, uint8 _char)
{
	STD_ReturnType error_status = E_OK;
	if(NULL == lcd)
	{
		error_status = E_NOT_OK;
	}
	else
	{

		/* set cursor to the required position */
		LCD_4bit_setCursorLocation(lcd, row, col);

		/* Write 0 on the RS pin */
		error_status &= GPIO_pinWrite(&(lcd->rs), GPIO_HIGH);


		/* Send the higher command nibble first */
		error_status &= LCD_send4Bits(lcd, (_char >> 4));		/* Send the higher char nibble first */
		error_status &= LCD_4bit_genEnablePulse(lcd);			/* generate a low pulse on the enable pin */

		error_status &= LCD_send4Bits(lcd, _char);				/* Send the lower char nibble */
		error_status &= LCD_4bit_genEnablePulse(lcd);			/* generate a low pulse on the enable pin */

		_delay_ms(2);

	}

	return error_status;
}

/***********************************************************************************************/
/*								  Function: LCD_4bit_sendString                                */
/***********************************************************************************************/
/**
 * @brief Prints a string on the LCD
 * @param lcd: pointer to the LCD object to be controlled
 * @param str: The string to be printed
 * @return status of the function:
 * 						(E_OK): the function finished successfully
 * 						(E_NOT_OK): A problem occurred in function
 */
STD_ReturnType LCD_4bit_sendString(const lcd_4bit_t *lcd, uint8 * str)
{
	STD_ReturnType error_status = E_OK;
	if(NULL == lcd || NULL == str)
	{
		error_status = E_NOT_OK;
	}
	else
	{
		while(*str)
		{
			LCD_4bit_sendChar(lcd, *str++); /* print character then increment pointer */
		}

	}

	return error_status;
}

/***********************************************************************************************/
/*								  Function: LCD_4bit_sendStringAtPosition                                */
/***********************************************************************************************/
/**
 * @brief Prints a string at a required position on the LCD
 * @param lcd: pointer to the LCD object to be controlled
 * @param row: The row index of the cursor. possible values are 1, 2, 3, or 4.
 * @param col: The column index of the cursor. possible values are 0 to 80. (80 to cover one-line mode)
 * @param str: pointer to the string to be printed
 * @return status of the function:
 * 						(E_OK): the function finished successfully
 * 						(E_NOT_OK): A problem occurred in function
 */
STD_ReturnType LCD_4bit_sendStringAtPosition(const lcd_4bit_t *lcd, uint8 row, uint8 col, uint8 * str )
{
	STD_ReturnType error_status = E_OK;
	if(NULL == lcd || NULL == str)
	{
		error_status = E_NOT_OK;
	}
	else
	{
		/* move the cursor to the required position */
		error_status &= LCD_4bit_setCursorLocation(lcd, row, col);
		while(*str)
		{
			LCD_4bit_sendChar(lcd, *str++); /* print character then increment pointer */
		}

	}

	return error_status;
}

/***********************************************************************************************/
/*								  Function: LCD_4bit_sendCustomCharAtPosition                            */
/***********************************************************************************************/
/**
 * @brief Prints a custom character by passing its pattern array on the LCD
 * @param lcd: pointer to the LCD object to be controlled
 * @param char_arr: The char array required to generate the required character
 * @param mem_position: The CGRAM memory position where the character will be stored
 * @return status of the function:
 * 						(E_OK): the function finished successfully
 * 						(E_NOT_OK): A problem occurred in function
 */
STD_ReturnType LCD_4bit_sendCustomCharAtPosition(const lcd_4bit_t *lcd, uint8 * char_arr, uint8 row,\
		uint8 col, uint8 mem_position)
{
	STD_ReturnType error_status = E_OK;
	if(NULL == lcd)
	{
		error_status = E_NOT_OK;
	}
	else
	{
		uint8 new_char_counter;
		/* make AC point to the CGRAM memory position point to where the character will be stored */
		error_status = LCD_4bit_sendCommand(lcd, ((_LCD_CGRAM_START) + mem_position * 8));

		/* every new character is an array that consists of 8 patterns
		 * every pattern is stored in a memory location
		 */
		for(new_char_counter = 0; new_char_counter < 8; new_char_counter++)
		{
			error_status &= LCD_4bit_sendChar(lcd, char_arr[new_char_counter]);
		}
		/* return to DDRAM and send the new char */
		error_status &= LCD_4bit_sendCharAtPosition(lcd, row, col, mem_position);
	}

	return error_status;
}

/***********************************************************************************************/
/*								  Function: LCD_4bit_sendNum                            */
/***********************************************************************************************/
/**
 * brief converts a number to a string then prints it on the LCD
 * @param lcd: pointer to the LCD object to be controlled
 * @param num: The number to be printed on the LCD
 * @return status of the function:
 * 						(E_OK): the function finished successfully
 * 						(E_NOT_OK): A problem occurred in function
 */
STD_ReturnType LCD_4bit_sendNum(const lcd_4bit_t *lcd, uint32 num)
{
	STD_ReturnType error_status = E_OK;
	if(NULL == lcd)
	{
		error_status = E_NOT_OK;
	}
	else
	{
		uint8 str[12];											/* to store the converted number */
		error_status  = numToStr(num, str);						/* call the function to convert the number */
		error_status &= LCD_4bit_sendString(lcd, str);			/* send the string to the LCD */
	}

	return error_status;
}





/***********************************************************************************************/
/*								  Function: LCD_8bit_init                                      */
/***********************************************************************************************/
/**
 * @brief Initializes LCD control and data pins as output
 * @param lcd: pointer to the LCD object to be controlled
 * @return status of the function:
 * 						(E_OK): the function finished successfully
 * 						(E_NOT_OK): A problem occurred in function
 */
STD_ReturnType LCD_8bit_init(const lcd_8bit_t *lcd)
{
	STD_ReturnType error_status = E_OK;
	if(NULL == lcd)
	{
		error_status = E_NOT_OK;
	}
	else
	{
		uint8 data_pin_counter; 										/* To loop through the 8 data pins */
		error_status = GPIO_pinDirectionInit(&(lcd->rs), GPIO_OUTPUT);  /* make RS output pin*/
		error_status &= GPIO_pinDirectionInit(&(lcd->en), GPIO_OUTPUT); /* make EN output pin */

		for(data_pin_counter = 0; data_pin_counter < 8; data_pin_counter++)
		{
			/* make all 8 data pins output pins */
			error_status &= GPIO_pinDirectionInit(&(lcd->data_pin[data_pin_counter]), GPIO_OUTPUT);
		}

		/* generate a delay of 20 msec at  startup*/
		_delay_ms(20);

		error_status &= LCD_8bit_sendCommand(lcd, _LCD_8BIT_MODE_2_LINE); /* set 8 bit mode 2 line */
		_delay_ms(5);
		error_status &= LCD_8bit_sendCommand(lcd, _LCD_8BIT_MODE_2_LINE);
		_delay_us(150);
		error_status &= LCD_8bit_sendCommand(lcd, _LCD_8BIT_MODE_2_LINE);

		error_status &= LCD_8bit_sendCommand(lcd, _LCD_CLEAR);			  /* clear LCD */
		error_status &= LCD_8bit_sendCommand(lcd, _LCD_ENTRY_MODE);		  /* increment cursor mode */
		error_status &= LCD_8bit_sendCommand(lcd, _LCD_CURSOR_OFF_DISPLAY_ON);
	}

	return error_status;
}
/***********************************************************************************************/
/*								  Function: LCD_8bit_clear                                     */
/***********************************************************************************************/
/**
 * @brief Clears LCD and return cursor to home position
 * @param lcd: pointer to the LCD object to be controlled
 * @return status of the function:
 * 						(E_OK): the function finished successfully
 * 						(E_NOT_OK): A problem occurred in function
 */
STD_ReturnType LCD_8bit_clear(const lcd_8bit_t *lcd)
{
	STD_ReturnType error_status = E_OK;
	if(NULL == lcd)
	{
		error_status = E_NOT_OK;
	}
	else
	{
		LCD_8bit_sendCommand(lcd, _LCD_CLEAR);
	}
	return error_status;
}

/***********************************************************************************************/
/*								  Function: LCD_8bit_sendCommand                               */
/***********************************************************************************************/
/**
 * @brief Send a command to the LCD controller
 * @param lcd: pointer to the LCD object to be controlled
 * @param command: The command to be sent to the LCD
 * @return status of the function:
 * 						(E_OK): the function finished successfully
 * 						(E_NOT_OK): A problem occurred in function
 */
STD_ReturnType LCD_8bit_sendCommand(const lcd_8bit_t *lcd, uint8 command)
{
	STD_ReturnType error_status = E_OK;
	if(NULL == lcd)
	{
		error_status = E_NOT_OK;
	}
	else
	{
		/* Write 0 on the RS pin */
		error_status &= GPIO_pinWrite(&(lcd->rs), GPIO_LOW);
		uint8 data_pin_counter; 									/* To loop through the 8 data pins */

		for(data_pin_counter = 0; data_pin_counter < 8; data_pin_counter++)
		{
			/* load each LCD data pin with the corresponding passed command bits */
			error_status &= GPIO_pinWrite(&(lcd->data_pin[data_pin_counter]), READ_BIT(command, data_pin_counter));
		}
		LCD_8bit_genEnablePulse(lcd);
		_delay_ms(2);
	}

	return error_status;
}

/***********************************************************************************************/
/*								  Function: LCD_8bit_sendChar                                  */
/***********************************************************************************************/
/**
 * @brief Prints a character on the LCD
 * @param lcd: pointer to the LCD object to be controlled
 * @param _char: The character to be printed on the LCD
 * @return status of the function:
 * 						(E_OK): the function finished successfully
 * 						(E_NOT_OK): A problem occurred in function
 */
STD_ReturnType LCD_8bit_sendChar(const lcd_8bit_t *lcd, uint8 _char)
{
	STD_ReturnType error_status = E_OK;
	if(NULL == lcd)
	{
		error_status = E_NOT_OK;
	}
	else
	{
		/* Write 1 on the RS pin */
		error_status &= GPIO_pinWrite(&(lcd->rs), GPIO_HIGH);
		uint8 data_pin_counter; 									/* To loop through the 8 data pins */

		for(data_pin_counter = 0; data_pin_counter < 8; data_pin_counter++)
		{
			/* load each LCD data pin with the corresponding passed command bits */
			error_status &= GPIO_pinWrite(&(lcd->data_pin[data_pin_counter]), READ_BIT(_char, data_pin_counter));
		}
		LCD_8bit_genEnablePulse(lcd);
		_delay_ms(2);
	}

	return error_status;
}

/***********************************************************************************************/
/*								  Function: LCD_8bit_sendCharAtPosition                                  */
/***********************************************************************************************/
/**
 * @brief Prints a character on the LCD
 * @param lcd: pointer to the LCD object to be controlled
 * @param row: The row index of the cursor. possible values are 1, 2, 3, or 4.
 * @param col: The column index of the cursor. possible values are 0 to 80. (80 to cover one-line mode)
 * @param _char: The character to be printed on the LCD
 * @return status of the function:
 * 						(E_OK): the function finished successfully
 * 						(E_NOT_OK): A problem occurred in function
 */
STD_ReturnType LCD_8bit_sendCharAtPosition(const lcd_8bit_t *lcd, uint8 row, uint8 col, uint8 _char)
{
	STD_ReturnType error_status = E_OK;
	if(NULL == lcd)
	{
		error_status = E_NOT_OK;
	}
	else
	{
		/* set cursor to the required position */
		LCD_8bit_setCursorLocation(lcd, row, col);

		/* Write 1 on the RS pin */
		error_status &= GPIO_pinWrite(&(lcd->rs), GPIO_HIGH);

		/* Send the higher command nibble first */
		error_status &= LCD_8bit_sendChar(lcd, _char);

		_delay_ms(2);
	}

	return error_status;
}


/***********************************************************************************************/
/*								  Function: LCD_8bit_sendString                                */
/***********************************************************************************************/
/**
 * @brief Prints a string on the LCD
 * @param lcd: pointer to the LCD object to be controlled
 * @param str: The string to be printed
 * @return status of the function:
 * 						(E_OK): the function finished successfully
 * 						(E_NOT_OK): A problem occurred in function
 */
STD_ReturnType LCD_8bit_sendString(const lcd_8bit_t *lcd, uint8 * str)
{
	STD_ReturnType error_status = E_OK;
	if(NULL == lcd || NULL == str)
	{
		error_status = E_NOT_OK;
	}
	else
	{
		while(*str)
		{
			LCD_8bit_sendChar(lcd, *str++); /* print character then increment pointer */
		}
	}

	return error_status;
}

/**
 * @brief Prints a string at a required position on the LCD
 * @param lcd: pointer to the LCD object to be controlled
 * @param row: The row index of the cursor. possible values are 1, 2, 3, or 4.
 * @param col: The column index of the cursor. possible values are 1 to 80. (80 to cover one-line mode)
 * @param str: pointer to the string to be printed
 * @return status of the function:
 * 						(E_OK): the function finished successfully
 * 						(E_NOT_OK): A problem occurred in function
 */
STD_ReturnType LCD_8bit_sendStringAtPosition(const lcd_8bit_t *lcd, uint8 row, uint8 col, uint8 * str )
{
	STD_ReturnType error_status = E_OK;
	if(NULL == lcd || NULL == str)
	{
		error_status = E_NOT_OK;
	}
	else
	{
		/* move the cursor to the required position */
		error_status &= LCD_8bit_setCursorLocation(lcd, row, col);
		while(*str)
		{
			LCD_8bit_sendChar(lcd, *str++); /* print character then increment pointer */
		}

	}

	return error_status;
}

/***********************************************************************************************/
/*								  Function: LCD_8bit_sendCustomChar                            */
/***********************************************************************************************/
/**
 * @brief Prints a custom character by passing its pattern array on the LCD
 * @param lcd: pointer to the LCD object to be controlled
 * @param char_arr: The char array required to generate the required character
 * @return status of the function:
 * 						(E_OK): the function finished successfully
 * 						(E_NOT_OK): A problem occurred in function
 */
STD_ReturnType LCD_8bit_sendCustomCharAtPosition(const lcd_8bit_t *lcd, uint8 * char_arr, uint8 row,\
		uint8 col, uint8 mem_position)
{
	STD_ReturnType error_status = E_OK;
	if(NULL == lcd)
	{
		error_status = E_NOT_OK;
	}
	else
	{
		uint8 new_char_counter;
		/* make AC point to the CGRAM memory position point to where the character will be stored */
		error_status = LCD_8bit_sendCommand(lcd, ((_LCD_CGRAM_START) + mem_position * 8));

		/* every new character is an array that consists of 8 patterns
		 * every pattern is stored in a memory location
		 */
		for(new_char_counter = 0; new_char_counter < 8; new_char_counter++)
		{
			error_status &= LCD_8bit_sendChar(lcd, char_arr[new_char_counter]);
		}
		/* return to DDRAM and send the new char */
		error_status &= LCD_8bit_sendCharAtPosition(lcd, row, col, mem_position);

	}

	return error_status;
}

/***********************************************************************************************/
/*								  Function: LCD_8bit_sendNum                            */
/***********************************************************************************************/
/**
 * @brief converts a number to a string then prints it on the LCD
 * @param lcd: pointer to the LCD object to be controlled
 * @param num: The number to be printed on the LCD
 * @return status of the function:
 * 						(E_OK): the function finished successfully
 * 						(E_NOT_OK): A problem occurred in function
 */
STD_ReturnType LCD_8bit_sendNum(const lcd_8bit_t *lcd, uint32 num)
{
	STD_ReturnType error_status = E_OK;
	if(NULL == lcd)
	{
		error_status = E_NOT_OK;
	}
	else
	{
		uint8 str[12];											/* to store the converted number */
		error_status  = numToStr(num, str);						/* call the function to convert the number */
		error_status &= LCD_8bit_sendString(lcd, str);			/* send the string to the LCD */
	}

	return error_status;
}


/***********************************************************************************************/
/*								  Function: LCD_4bit_setCursorLocation                         */
/***********************************************************************************************/
/**
 * @brief Set the LCD cursor on the specified row and column
 * @param lcd: Pointer to the LCD object to be controlled
 * @param row: The row index of the cursor. possible values are 1, 2, 3, or 4.
 * @param col: The column index of the cursor. possible values are 0 to 80. (80 to cover one-line mode)
 * @return status of the function:
 * 						(E_OK): the function finished successfully
 * 						(E_NOT_OK): A problem occurred in function
 */
STD_ReturnType LCD_4bit_setCursorLocation(const lcd_4bit_t *lcd, uint8 row, uint8 col)
{
	STD_ReturnType error_status = E_OK;
	if(NULL == lcd)
	{
		error_status = E_NOT_OK;
	}
	else
	{
		col--;
		/* check row value */
		switch(row)
		{
		case ROW1:
			/* address format is 1 A6 A5 A4 A3 A2 A1 A0,
			 * so, we add 0x80 to the base address of every row */

			error_status &= LCD_4bit_sendCommand(lcd, (0x80 + col)); /* first row start address is 0x00 */
			break;

		case ROW2:
			error_status &= LCD_4bit_sendCommand(lcd, (0xC0 + col)); /* second row start address is 0x40 */
			break;

		case ROW3:
			error_status &= LCD_4bit_sendCommand(lcd, (0x94 + col)); /* third row start address is 0x14 */
			break;

		case ROW4:
			error_status &= LCD_4bit_sendCommand(lcd, (0xD4 + col)); /* fourth row start address is 0x54 */
			break;

		default:
			error_status = E_NOT_OK;
		}
	}
	return error_status;
}

/***********************************************************************************************/
/*								  Function: LCD_8bit_setCursorLocation                         */
/***********************************************************************************************/
/**
 * @brief Set the LCD cursor on the specified row and column
 * @param lcd: Pointer to the LCD object to be controlled
 * @param row: The row index of the cursor. possible values are 1, 2, 3, or 4.
 * @param col: The column index of the cursor. possible values are 0 to 80. (80 to cover one-line mode)
 * @return status of the function:
 * 						(E_OK): the function finished successfully
 * 						(E_NOT_OK): A problem occurred in function
 */
STD_ReturnType LCD_8bit_setCursorLocation(const lcd_8bit_t *lcd, uint8 row, uint8 col)
{
	STD_ReturnType error_status = E_OK;
	if(NULL == lcd)
	{
		error_status = E_NOT_OK;
	}
	else
	{
		col--;
		/* check row value */
		switch(row)
		{
		case ROW1:
			error_status &= LCD_8bit_sendCommand(lcd, (0x80 + col)); /* first row start address is 0x00 */
			break;

		case ROW2:
			error_status &= LCD_8bit_sendCommand(lcd, (0xC0 + col)); /* second row start address is 0x40 */
			break;

		case ROW3:
			error_status &= LCD_8bit_sendCommand(lcd, (0x94 + col)); /* third row start address is 0x14 */
			break;

		case ROW4:
			error_status &= LCD_8bit_sendCommand(lcd, (0xD4 + col)); /* fourth row start address is 0x54 */
			break;

		default:
			error_status = E_NOT_OK;
		}
	}
	return error_status;
}

/***********************************************************************************************/
/*								  Function: LCD_send4Bits                                      */
/***********************************************************************************************/
/**
 * @brief Write the lower nibble of passed data or command to the 4 data pins of the LCD
 * @param lcd: pointer to the LCD object to be controlled
 * @param data_command: The data or command to be written, only lower nibble is used
 * @return status of the function:
 * 						(E_OK): the function finished successfully
 * 						(E_NOT_OK): A problem occurred in function
 */
static STD_ReturnType LCD_send4Bits(const lcd_4bit_t *lcd, uint8 data_command)
{
	STD_ReturnType error_status = E_OK;
	if(NULL == lcd)
	{
		error_status = E_NOT_OK;
	}
	else
	{
		/* write the lower nibble of the passed data_command argument to the data pin of LCD */
		error_status = GPIO_pinWrite(&(lcd->data_pin[0]), READ_BIT(data_command, 0));
		error_status = GPIO_pinWrite(&(lcd->data_pin[1]), READ_BIT(data_command, 1));
		error_status = GPIO_pinWrite(&(lcd->data_pin[2]), READ_BIT(data_command, 2));
		error_status = GPIO_pinWrite(&(lcd->data_pin[3]), READ_BIT(data_command, 3));
	}
	return error_status;
}

/***********************************************************************************************/
/*								  Function: LCD_4bit_genEnablePulse                            */
/***********************************************************************************************/
/**
 * @brief Generates a low pulse signal on the enable pin of a 4-bit LCD
 * @param lcd: pointer to the LCD object to be controlled
 * @return status of the function:
 * 						(E_OK): the function finished successfully
 * 						(E_NOT_OK): A problem occurred in function
 */
static STD_ReturnType LCD_4bit_genEnablePulse(const lcd_4bit_t *lcd)
{
	STD_ReturnType error_status = E_OK;
	if(NULL == lcd)
	{
		error_status = E_NOT_OK;
	}
	else
	{
		error_status = GPIO_pinWrite(&(lcd->en), GPIO_HIGH);
		_delay_us(1);
		error_status = GPIO_pinWrite(&(lcd->en), GPIO_LOW);

	}
	return error_status;
}

/***********************************************************************************************/
/*								  Function: LCD_8bit_genEnablePulse                            */
/***********************************************************************************************/
/**
 * @brief Generates a HIGH TO LOW pulse on the enable pin of an 8-bit LCD
 * @param lcd: pointer to the LCD object to be controlled
 * @return status of the function:
 * 						(E_OK): the function finished successfully
 * 						(E_NOT_OK): A problem occurred in function
 */
static STD_ReturnType LCD_8bit_genEnablePulse(const lcd_8bit_t *lcd)
{
	STD_ReturnType error_status = E_OK;
	if(NULL == lcd)
	{
		error_status = E_NOT_OK;
	}
	else
	{
		error_status = GPIO_pinWrite(&(lcd->en), GPIO_HIGH);
		_delay_us(1);
		error_status = GPIO_pinWrite(&(lcd->en), GPIO_LOW);
	}
	return error_status;
}

/***********************************************************************************************/
/*								  Function: numToStr                                           */
/***********************************************************************************************/
/**
 * @brief Converts a number to string
 * @param num: The number to be printed on the LCD
 * @param str: The string where the converted number will be inserted into
 * @return status of the function:
 * 						(E_OK): the function finished successfully
 * 						(E_NOT_OK): A problem occurred in function
 */
static STD_ReturnType numToStr(const uint32 num, char * str)
{
	STD_ReturnType error_status = E_OK;
	if(NULL == str)
	{
		error_status = E_NOT_OK;
	}
	else
	{
		sprintf(str, "%i", num);
	}
	return error_status;
}
