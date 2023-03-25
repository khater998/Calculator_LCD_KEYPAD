/*
 * main.c
 *
 *  Created on: Mar 25, 2023
 *      Author: khater
 */

#include "Application_layer/app.h"
#include <avr/io.h>
#include <stdlib.h>


int main()
{
	app_init();

	while(1)
	{
		app_start();
	}
}

