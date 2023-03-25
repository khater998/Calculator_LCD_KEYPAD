/*
 * lcd.h
 *
 *  Created on: Mar 23, 2023
 *      Author: khater
 */

#ifndef ECUAL_LAYER_LCD_DRIVER_LCD_H_
#define ECUAL_LAYER_LCD_DRIVER_LCD_H_

/***********************************************************************************************/
/*								  Section: INCLUDES                                            */
/***********************************************************************************************/
#include "../../MCAL_layer/GPIO_driver/gpio.h"


/***********************************************************************************************/
/*								  Section: MACRO DECLARATIONS                                  */
/***********************************************************************************************/
#define _LCD_CLEAR 						0x01	/* clear LCD contents and return home */
#define _LCD_RETURN_HOME 				0x02	/* return cursor to home position*/
#define _LCD_ENTRY_MODE 				0x06	/* auto increment cursor after printing a char */
#define _LCD_CURSOR_OFF_DISPLAY_ON 		0x0C	/* display on without cursor */
#define _LCD_CURSOR_OFF_DISPLAY_OFF 	0x08	/* display off*/
#define _LCD_CURSOR_ON_BLINK_ON 		0x0F
#define _LCD_CURSOR_ON_BLINK_OFF 		0x0E
#define _LCD_DISPLAY_SHIFT_RIGHT 		0x1C	/* shift all LCD content right by 1 position */
#define _LCD_DISPLAY_SHIFT_LEFT 		0x18	/* shift all LCD content left  by 1 position */
#define _LCD_8BIT_MODE_2_LINE 			0x38
#define _LCD_4BIT_MODE_2_LINE 			0x28
#define _LCD_CGRAM_START 				0x40
#define _LCD_DDRAM_START 				0x80


#define ROW1 1
#define ROW2 2
#define ROW3 3
#define ROW4 4


/***********************************************************************************************/
/*								  Section: MACRO FUNCTIONS DECLARATIONS                        */
/***********************************************************************************************/




/***********************************************************************************************/
/*								  Section: DATA TYPE DECLARATIONS                              */
/***********************************************************************************************/
typedef struct{
	pin_obj_t rs;
	pin_obj_t en;
	pin_obj_t data_pin[4];
}lcd_4bit_t;

typedef struct{
	pin_obj_t rs;				/** @ref pin_obj_t */
	pin_obj_t en;
	pin_obj_t data_pin[8];
}lcd_8bit_t;

/***********************************************************************************************/
/*								  Section: FUNCTION DECLARATIONS                               */
/***********************************************************************************************/

/**
 * @brief Initializes LCD control and data pins as output
 * @param lcd: pointer to the LCD object to be controlled
 * @return status of the function:
 * 						(E_OK): the function finished successfully
 * 						(E_NOT_OK): A problem occurred in function
 */
STD_ReturnType LCD_4bit_init(const lcd_4bit_t *lcd);

/**
 * @brief Send a command to the LCD controller
 * @param lcd: pointer to the LCD object to be controlled
 * @param command: The command to be sent to the LCD
 * @return status of the function:
 * 						(E_OK): the function finished successfully
 * 						(E_NOT_OK): A problem occurred in function
 */
STD_ReturnType LCD_4bit_sendCommand(const lcd_4bit_t *lcd, uint8 command);

/**
 * @brief Clears LCD and return cursor to home position
 * @param lcd: pointer to the LCD object to be controlled
 * @return status of the function:
 * 						(E_OK): the function finished successfully
 * 						(E_NOT_OK): A problem occurred in function
 */
STD_ReturnType LCD_4bit_clear(const lcd_4bit_t *lcd);

/**
 * @brief Prints a character on the LCD
 * @param lcd: pointer to the LCD object to be controlled
 * @param _char: The character to be printed on the LCD
 * @return status of the function:
 * 						(E_OK): the function finished successfully
 * 						(E_NOT_OK): A problem occurred in function
 */
STD_ReturnType LCD_4bit_sendChar(const lcd_4bit_t *lcd, uint8 _char);


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
STD_ReturnType LCD_4bit_sendCharAtPosition(const lcd_4bit_t *lcd, uint8 row, uint8 col, uint8 _char);

/**
 * @brief Prints a string on the LCD
 * @param lcd: pointer to the LCD object to be controlled
 * @param str: The string to be printed
 * @return status of the function:
 * 						(E_OK): the function finished successfully
 * 						(E_NOT_OK): A problem occurred in function
 */
STD_ReturnType LCD_4bit_sendString(const lcd_4bit_t *lcd, uint8 * str);

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
STD_ReturnType LCD_4bit_sendStringAtPosition(const lcd_4bit_t *lcd, uint8 row, uint8 col, uint8 * str );


/**
 * @brief Prints a custom character by passing its pattern array on the LCD
 * @param lcd: pointer to the LCD object to be controlled
 * @param char_arr: The char array required to generate the required character
 * @return status of the function:
 * 						(E_OK): the function finished successfully
 * 						(E_NOT_OK): A problem occurred in function
 */
STD_ReturnType LCD_4bit_sendCustomCharAtPosition(const lcd_4bit_t *lcd, uint8 * char_arr, uint8 row,\
												uint8 col, uint8 mem_position);


/**
 * brief converts a number to a string then prints it on the LCD
 * @param lcd: pointer to the LCD object to be controlled
 * @param num: The number to be printed on the LCD
 * @return status of the function:
 * 						(E_OK): the function finished successfully
 * 						(E_NOT_OK): A problem occurred in function
 */
STD_ReturnType LCD_4bit_sendNum(const lcd_4bit_t *lcd, uint32 num);




/**
 * @brief Initializes LCD control and data pins as output
 * @param lcd: pointer to the LCD object to be controlled
 * @return status of the function:
 * 						(E_OK): the function finished successfully
 * 						(E_NOT_OK): A problem occurred in function
 */
STD_ReturnType LCD_8bit_init(const lcd_8bit_t *lcd);

/**
 * @brief Clears LCD and return cursor to home position
 * @param lcd: pointer to the LCD object to be controlled
 * @return status of the function:
 * 						(E_OK): the function finished successfully
 * 						(E_NOT_OK): A problem occurred in function
 */
STD_ReturnType LCD_8bit_clear(const lcd_8bit_t *lcd);


/**
 * @brief Send a command to the LCD controller
 * @param lcd: pointer to the LCD object to be controlled
 * @param command: The command to be sent to the LCD
 * @return status of the function:
 * 						(E_OK): the function finished successfully
 * 						(E_NOT_OK): A problem occurred in function
 */
STD_ReturnType LCD_8bit_sendCommand(const lcd_8bit_t *lcd, uint8 command);

/**
 * @brief Prints a character on the LCD
 * @param lcd: pointer to the LCD object to be controlled
 * @param _char: The character to be printed on the LCD
 * @return status of the function:
 * 						(E_OK): the function finished successfully
 * 						(E_NOT_OK): A problem occurred in function
 */
STD_ReturnType LCD_8bit_sendChar(const lcd_8bit_t *lcd, uint8 _char);

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
STD_ReturnType LCD_8bit_sendCharAtPosition(const lcd_8bit_t *lcd, uint8 row, uint8 col, uint8 _char);

/**
 * @brief Prints a string on the LCD
 * @param lcd: pointer to the LCD object to be controlled
 * @param str: The string to be printed
 * @return status of the function:
 * 						(E_OK): the function finished successfully
 * 						(E_NOT_OK): A problem occurred in function
 */
STD_ReturnType LCD_8bit_sendString(const lcd_8bit_t *lcd, uint8 * str);

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
STD_ReturnType LCD_8bit_sendStringAtPosition(const lcd_8bit_t *lcd, uint8 row, uint8 col, uint8 * str );


/**
 * @brief Prints a custom character by passing its pattern array on the LCD
 * @param lcd: pointer to the LCD object to be controlled
 * @param char_arr: The char array required to generate the required character
 * @return status of the function:
 * 						(E_OK): the function finished successfully
 * 						(E_NOT_OK): A problem occurred in function
 */

STD_ReturnType LCD_8bit_sendCustomChar(const lcd_8bit_t *lcd, uint8 * char_arr);
/**
 * @brief Set the LCD cursor on the specified row and column
 * @param lcd: Pointer to the LCD object to be controlled
 * @param row: The row index of the cursor. possible values are 0, 1, 2, or 3.
 * @param col: The column index of the cursor. possible values are 0 to 79. (79 to cover one-line mode)
 * @return status of the function:
 * 						(E_OK): the function finished successfully
 * 						(E_NOT_OK): A problem occurred in function
 */
STD_ReturnType LCD_4bit_setCursorLocation(const lcd_4bit_t *lcd, uint8 row, uint8 col);

/**
 * @brief Set the LCD cursor on the specified row and column
 * @param lcd: Pointer to the LCD object to be controlled
 * @param row: The row index of the cursor. possible values are 0, 1, 2, or 3.
 * @param col: The column index of the cursor. possible values are 0 to 79. (79 to cover one-line mode)
 * @return status of the function:
 * 						(E_OK): the function finished successfully
 * 						(E_NOT_OK): A problem occurred in function
 */
STD_ReturnType LCD_8bit_setCursorLocation(const lcd_8bit_t *lcd, uint8 row, uint8 col);

/**
 * @brief Converts a number to a string then prints it to the LCD
 * @param lcd: Pointer to the LCD object to be controlled
 * @param num: The number to be printed on the screen
 * @return status of the function:
 * 						(E_OK): the function finished successfully
 * 						(E_NOT_OK): A problem occurred in function
 */
STD_ReturnType LCD_8bit_sendNum(const lcd_8bit_t *lcd, uint32 num);





#endif /* ECUAL_LAYER_LCD_DRIVER_LCD_H_ */
