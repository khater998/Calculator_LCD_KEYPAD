/*
 * GPIO_driver.c
 *
 *  Created on: Mar 14, 2023
 *      Author: khater
 */

#include "gpio.h"
#include <avr/io.h> /* To use the IO Ports Registers */


/* REFERNCE TO THE DATA DIRECTION CONTROL REGISTERS */
volatile uint8 * DDR_registers[] = {&DDRA, &DDRB, &DDRC, &DDRD};

/* REFERNCE TO THE PORT CONTROL REGISTERS */
volatile uint8 * PORT_registers[] = {&PORTA, &PORTB, &PORTC, &PORTD};

/* REFERNCE TO THE PIN STATUS REGISTERS */
volatile uint8 * PIN_registers[] = {&PINA, &PINB, &PINC, &PIND};

/**
 * @brief toggles the logic status of the pin @brief logic_t
 * @param pin_config: pointer to the pin configuration variable @ref pin_config_t
 * @return status of the function:
 * 						(E_OK): the function finished successfully
 * 						(E_NOT_OK): A problem occurred in function
 */
STD_ReturnType GPIO_pinInit(const pin_obj_t * pin_obj)
{
	STD_ReturnType error_status = E_OK;						/* to store the error value to be returned */

	/* check for null pointer */
	/* pin_obj_t is a 1 byte structure, port_id is 2 bits in size, and pin_id is 3 bits
	 * can't check the if pin or port numbers are valid, since their values will be
	 * truncated to their relative bit sizes.
	 * user will get a warning message if an implicit truncation happens
	 */
	if(NULL == pin_obj)
	{
		error_status = E_NOT_OK;
	}
	else
	{
		/* check the required pin direction */
		switch (pin_obj->direction)
		{
		case GPIO_INPUT:
			CLR_BIT(*(DDR_registers[pin_obj->port_id]), pin_obj->pin_id);
			break;

		case GPIO_OUTPUT:
			SET_BIT(*(DDR_registers[pin_obj->port_id]), pin_obj->pin_id);
			break;

			/* pin_obj->direction is 1 bit in size, so input is either 1(GPIO_OUTPUT) or zero(GPIO_INPUT)
			 * so, there is no need for default case in here */
			/* default:
						error_status = E_NOT_OK;*/
		}

		/* check the required pin logic */
		switch(pin_obj->logic)
		{
		case GPIO_LOW:
			CLR_BIT(*(PORT_registers[pin_obj->port_id]), pin_obj->pin_id);
			break;

		case GPIO_HIGH:
			SET_BIT(*(PORT_registers[pin_obj->port_id]), pin_obj->pin_id);
			break;

			/* pin_obj->logic is 1 bit in size, so input is either 1(GPIO_HIGH) or zero(GPIO_LOW)
			 * so, there is no need for default case in here */
			/* default:
					error_status = E_NOT_OK;*/
		}
	}

	return error_status;

}


/**
 * @brief initializes the direction of a specific pin @ref direction_t
 * @param pin_config: pointer to the pin configuration variable @ref pin_config_t
 * @param direction: the required direction of the pin
 * @return status of the function:
 * 						(E_OK): the function finished successfully
 * 						(E_NOT_OK): A problem occurred in function
 */
STD_ReturnType GPIO_pinDirectionInit(const pin_obj_t * pin_obj, GPIO_direction_t direction)
{
	STD_ReturnType error_status = E_OK;					/* to store the error value to be returned */

	/* check for null pointer */
	/* pin_obj_t is a 1 byte structure, port_id is 2 bits in size, and pin_id is 3 bits
	 * can't check the if pin or port numbers are valid, since their values will be
	 * truncated to their relative bit sizes.
	 * user will get a warning message if an implicit truncation happens
	 */
	if(NULL == pin_obj)
	{
		error_status = E_NOT_OK;
	}
	else
	{
		/* check the required pin direction */
		switch (direction)
		{
		case GPIO_INPUT:
			CLR_BIT(*(DDR_registers[pin_obj->port_id]), pin_obj->pin_id);
			break;

		case GPIO_OUTPUT:
			SET_BIT(*(DDR_registers[pin_obj->port_id]), pin_obj->pin_id);
			break;

			/* pin_obj->direction is 1 bit in size, so input is either 1(GPIO_OUTPUT) or zero(GPIO_INPUT)
			 * so, there is no need for default case in here */
			/* default:
					error_status = E_NOT_OK;*/
		}
	}
	return error_status;
}

/**
 * @brief reads the direction status of the pin @ref direction_t
 * @param pin_config: pointer to the pin configuration variable @ref pin_config_t
 * @param direction: the variable to store the direction status of the pin
 * @return status of the function:
 * 						(E_OK): the function finished successfully
 * 						(E_NOT_OK): A problem occurred in function
 */
STD_ReturnType GPIO_pinGetDirection(const pin_obj_t * pin_obj, GPIO_direction_t * direction)
{
	STD_ReturnType error_status = E_OK;						/* to store the error value to be returned */

	/* check for null pointer */
	/* pin_obj_t is a 1 byte structure, port_id is 2 bits in size, and pin_id is 3 bits
	 * can't check the if pin or port numbers are valid, since their values will be
	 * truncated to their relative bit sizes.
	 * user will get a warning message if an implicit truncation happens
	 */
	if(NULL == pin_obj)
	{
		error_status = E_NOT_OK;
	}
	else
	{
		/* update the passed argument with the current pin state */
		*direction = READ_BIT(*(DDR_registers[pin_obj->port_id]), pin_obj->pin_id);
	}

	return error_status;
}
/**
 * @brief writes a logic on the pin @ref logic_t
 * @param pin_config: pointer to the pin configuration variable @ref pin_config_t
 * @param logic: the required logic status of the pin
 * @return status of the function:
 * 						(E_OK): the function finished successfully
 * 						(E_NOT_OK): A problem occurred in function
 */
STD_ReturnType GPIO_pinWrite(const pin_obj_t * pin_obj, GPIO_logic_t logic)
{
	STD_ReturnType error_status = E_OK;

	/* check for null pointer */
	/* pin_obj_t is a 1 byte structure, port_id is 2 bits in size, and pin_id is 3 bits
	 * can't check the if pin or port numbers are valid, since their values will be
	 * truncated to their relative bit sizes.
	 * user will get a warning message if an implicit truncation happens
	 */
	if(NULL == pin_obj)
	{
		error_status = E_NOT_OK;
	}
	else
	{
		/* check the required pin logic */
		switch(logic)
		{
		case GPIO_LOW:
			CLR_BIT(*(PORT_registers[pin_obj->port_id]), pin_obj->pin_id);
			break;

		case GPIO_HIGH:
			SET_BIT(*(PORT_registers[pin_obj->port_id]), pin_obj->pin_id);
			break;


			/* pin_obj->logic is 1 bit in size, so input is either 1(GPIO_HIGH) or zero(GPIO_LOW)
			 * so, there is no need for default case in here */
			/* default:
						error_status = E_NOT_OK;*/
		}
	}

	return error_status;
}
/**
 * @brief reads the logic status of the pin @ref logic_t
 * @param pin_config: pointer to the pin configuration variable @ref pin_config_t
 * @param logic: a pointer variable to store the logic status of pin
 * @return status of the function:
 * 						(E_OK): the function finished successfully
 * 						(E_NOT_OK): A problem occurred in function
 */
STD_ReturnType GPIO_pinRead(const pin_obj_t * pin_obj, GPIO_logic_t * logic)
{
	STD_ReturnType error_status = E_OK;						/* to store the error value to be returned */

	/* check for null pointer */
	/* pin_obj_t is a 1 byte structure, port_id is 2 bits in size, and pin_id is 3 bits
	 * can't check the if pin or port numbers are valid, since their values will be
	 * truncated to their relative bit sizes.
	 * user will get a warning message if an implicit truncation happens
	 */
	if(NULL == pin_obj)
	{
		error_status = E_NOT_OK;
	}
	else
	{
		*logic = READ_BIT(*(PIN_registers[pin_obj->port_id]), pin_obj->pin_id);
	}

	return error_status;
}
/**
 * @brief toggles the logic status of the pin @brief logic_t
 * @param pin_config: pointer to the pin configuration variable @ref pin_config_t
 * @return status of the function:
 * 						(E_OK): the function finished successfully
 * 						(E_NOT_OK): A problem occurred in function
 */
STD_ReturnType GPIO_pinToggle(const pin_obj_t * pin_obj)
{
	STD_ReturnType error_status = E_OK;						/* to store the error value to be returned */

	/* check for null pointer */
	/* pin_obj_t is a 1 byte structure, port_id is 2 bits in size, and pin_id is 3 bits
	 * can't check the if pin or port numbers are valid, since their values will be
	 * truncated to their relative bit sizes.
	 * user will get a warning message if an implicit truncation happens
	 */
	if(NULL == pin_obj)
	{
		error_status = E_NOT_OK;
	}
	else
	{
		TOGGLE_BIT(*(PORT_registers[pin_obj->port_id]), pin_obj->pin_id);
	}

	return error_status;

}





/**
 * @brief initializes the whole port pins at once @ref direction_t
 * @param port: The port to be initialized @ref port_index_t
 * @param direction: uint8 variable where each bit configures a pin
 * @return status of the function:
 * 						(E_OK): the function finished successfully
 * 						(E_NOT_OK): A problem occurred in function
 */
STD_ReturnType GPIO_portDirectionInit(const port_id_t port, uint8 direction)
{
	STD_ReturnType error_status = E_OK;						/* to store the error value to be returned */

	/* check if port id is valid */
	if (port > PORT_MAX_NUMBER-1)
	{
		error_status = E_NOT_OK;
	}
	else
	{
		*(DDR_registers[port]) = direction;
	}
	return error_status;
}
/**
 * @brief reads the direction status of the whole port @ref direction_t
 * @param port: The port to be read @ref port_index_t
 * @param direction: a pointer to the variable to store the direction status of the port
 * @return status of the function:
 * 						(E_OK): the function finished successfully
 * 						(E_NOT_OK): A problem occurred in function
 */
STD_ReturnType GPIO_portGetDirection(const port_id_t port, uint8 * direction)
{
	STD_ReturnType error_status = E_OK;						/* to store the error value to be returned */

	/* check if port id is valid */
	if (port > PORT_MAX_NUMBER-1)
	{
		error_status = E_NOT_OK;
	}
	else
	{
		*direction = *(DDR_registers[port]);
	}
	return error_status;

}
/**
 * @brief writes logic on the whole port
 * @param port: The port to be written on @ref port_index_t
 * @param logic: uint8 variable where each bit configures a pin
 * @return status of the function:
 * 						(E_OK): the function finished successfully
 * 						(E_NOT_OK): A problem occurred in function
 */
STD_ReturnType GPIO_portWrite(const port_id_t port, uint8 logic)
{
	STD_ReturnType error_status = E_OK;						/* to store the error value to be returned */
	/* check if port id is valid */

	if (port > PORT_MAX_NUMBER-1)
	{
		error_status = E_NOT_OK;
	}
	else
	{
		*(PORT_registers[port]) = logic;
	}
	return error_status;

}
/**
 * @brief
 * @param port: The port to be initialized @ref port_index_t
 * @param logic: a pointer to the variable to store the logic status of the port
 * @return status of the function:
 * 						(E_OK): the function finished successfully
 * 						(E_NOT_OK): A problem occurred in function
 */
STD_ReturnType GPIO_portRead(const port_id_t port, uint8 * logic)
{
	STD_ReturnType error_status = E_OK;						/* to store the error value to be returned */

	/* check if port id is valid */
	if (port > PORT_MAX_NUMBER-1)
	{
		error_status = E_NOT_OK;
	}
	else
	{
		*logic = *(PORT_registers[port]);
	}
	return error_status;

}
/**
 * @brief toggles all the logic status of the port pins
 * @param port: The port to be toggled @ref port_index_t
 * @return status of the function:
 * 						(E_OK): the function finished successfully
 * 						(E_NOT_OK): A problem occurred in function
 */
STD_ReturnType GPIO_portToggle(const port_id_t port)
{
	STD_ReturnType error_status = E_OK;						/* to store the error value to be returned */

	/* check if port id is valid */
	if (port > PORT_MAX_NUMBER-1)
	{
		error_status = E_NOT_OK;
	}

	else
	{
		/*toggle all port pins*/
		*(PORT_registers[port]) ^= 0xFF;
	}
	return error_status;

}

