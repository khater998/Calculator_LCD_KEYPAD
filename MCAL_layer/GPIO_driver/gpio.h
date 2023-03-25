/*
 * GPIO_driver.h
 *
 *  Created on: Mar 14, 2023
 *      Author: khater
 */

#ifndef GPIO_DRIVER_GPIO_H_
#define GPIO_DRIVER_GPIO_H_

/***********************************************************************************************/
/*								  Section: INCLUDES                                            */
/***********************************************************************************************/

#include "../../STD_types.h"
#include "../../Common_Macros.h"


/***********************************************************************************************/
/*								  Section: MACRO DECLARATIONS                                  */
/***********************************************************************************************/

#define PORT_MAX_NUMBER 4
#define PIN_MAX_NUMBER  8


/***********************************************************************************************/
/*								  Section: DATA TYPE DECLARATIONS                              */
/***********************************************************************************************/

typedef enum{
	GPIO_INPUT,
	GPIO_OUTPUT
}GPIO_direction_t;

typedef enum{
	GPIO_LOW,
	GPIO_HIGH
}GPIO_logic_t;

typedef enum{
	PORTA_ID,
	PORTB_ID,
	PORTC_ID,
	PORTD_ID
}port_id_t;

typedef enum{
	PIN_0,
	PIN_1,
	PIN_2,
	PIN_3,
	PIN_4,
	PIN_5,
	PIN_6,
	PIN_7
}pin_id_t;

typedef struct{
	uint8 port_id 			:2; 			/** @ref port_id_t */
	uint8 pin_id  			:3;				/** @ref pin_id_t */
	uint8 direction			:1;				/** @ref GPIO_direction_t */
	uint8 logic 			:1;				/** @ref GPIO_logic_t */
	uint8 					:1;				/** padding the remaining 1 bit*/
}pin_obj_t;


/***********************************************************************************************/
/*								  Section: FUNCTION DECLARATIONS                               */
/***********************************************************************************************/

/**
 * @brief toggles the logic status of the pin 					@ref GPIO_logic_t
 * @param pin_config: pointer to the pin configuration variable @ref pin_config_t
 * @return status of the function:
 * 						(E_OK): the function finished successfully
 * 						(E_NOT_OK): A problem occurred in function
 */
STD_ReturnType GPIO_pinInit(const pin_obj_t * pin_obj);

/**
 * @brief initializes the direction of a specific pin 			@ref GPIO_direction_t
 * @param pin_config: pointer to the pin configuration variable @ref pin_config_t
 * @param direction: the required direction of the pin
 * @return status of the function:
 * 						(E_OK): the function finished successfully
 * 						(E_NOT_OK): A problem occurred in function
 */
STD_ReturnType GPIO_pinDirectionInit(const pin_obj_t * pin_obj, GPIO_direction_t direction);


/**
 * @brief reads the direction status of the pin 				@ref GPIO_direction_t
 * @param pin_config: pointer to the pin configuration variable @ref pin_config_t
 * @param direction: the variable to store the direction status of the pin
 * @return status of the function:
 * 						(E_OK): the function finished successfully
 * 						(E_NOT_OK): A problem occurred in function
 */
STD_ReturnType GPIO_pinGetDirection(const pin_obj_t * pin_obj, GPIO_direction_t * direction);


/**
 * @brief writes a logic on the pin 							@ref GPIO_logic_t
 * @param pin_config: pointer to the pin configuration variable @ref pin_config_t
 * @param logic: the required logic status of the pin
 * @return status of the function:
 * 						(E_OK): the function finished successfully
 * 						(E_NOT_OK): A problem occurred in function
 */
STD_ReturnType GPIO_pinWrite(const pin_obj_t * pin_obj, GPIO_logic_t logic);


/**
 * @brief reads the logic status of the pin 					@ref GPIO_logic_t
 * @param pin_config: pointer to the pin configuration variable @ref pin_config_t
 * @param logic: a pointer variable to store the logic status of pin
 * @return status of the function:
 * 						(E_OK): the function finished successfully
 * 						(E_NOT_OK): A problem occurred in function
 */
STD_ReturnType GPIO_pinRead(const pin_obj_t * pin_obj, GPIO_logic_t * logic);


/**
 * @brief toggles the logic status of the pin 					@brief GPIO_logic_t
 * @param pin_config: pointer to the pin configuration variable @ref pin_config_t
 * @return status of the function:
 * 						(E_OK): the function finished successfully
 * 						(E_NOT_OK): A problem occurred in function
 */
STD_ReturnType GPIO_pinToggle(const pin_obj_t * pin_obj);







/**
 * @brief initializes the whole port pins at once 				@ref GPIO_direction_t
 * @param port: The port to be initialized 						@ref port_id_t
 * @param direction: uint8 variable where each bit configures a pin
 * @return status of the function:
 * 						(E_OK): the function finished successfully
 * 						(E_NOT_OK): A problem occurred in function
 */
STD_ReturnType GPIO_portDirectionInit(const port_id_t port, uint8 direction);

/**
 * @brief reads the direction status of the whole port 			@ref GPIO_direction_t
 * @param port: The port to be read @ref port_index_t
 * @param direction: a pointer to the variable to store the direction status of the port
 * @return status of the function:
 * 						(E_OK): the function finished successfully
 * 						(E_NOT_OK): A problem occurred in function
 */
STD_ReturnType GPIO_portGetDirection(const port_id_t port, uint8 * direction);
/**
 * @brief writes logic on the whole port
 * @param port: The port to be written on 						@ref port_id_t
 * @param logic: uint8 variable where each bit configures a pin
 * @return status of the function:
 * 						(E_OK): the function finished successfully
 * 						(E_NOT_OK): A problem occurred in function
 */
STD_ReturnType GPIO_portWrite(const port_id_t port, uint8 logic);
/**
 * @brief
 * @param port: The port to be initialized 						@ref port_id_t
 * @param logic: a pointer to the variable to store the logic status of the port
 * @return status of the function:
 * 						(E_OK): the function finished successfully
 * 						(E_NOT_OK): A problem occurred in function
 */
STD_ReturnType GPIO_portRead(const port_id_t port, uint8 * logic);
/**
 * @brief toggles all the logic status of the port pins
 * @param port: The port to be toggled 							@ref port_id_t
 * @return status of the function:
 * 						(E_OK): the function finished successfully
 * 						(E_NOT_OK): A problem occurred in function
 */
STD_ReturnType GPIO_portToggle(const port_id_t port);



#endif /* GPIO_DRIVER_GPIO_H_ */
