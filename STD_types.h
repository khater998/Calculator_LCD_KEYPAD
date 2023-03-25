/*
 * STD_types.h
 *
 *  Created on: Mar 14, 2023
 *      Author: khater
 */

#ifndef _std_types_H_
#define _std_types_H_


/*************** Section: Data Types Declarations *****************/
typedef unsigned char 		uint8;
typedef unsigned short 		uint16;
typedef unsigned long 		uint32;
typedef unsigned long long 	uint64;


typedef signed char 		sint8;
typedef signed short 		sint16;
typedef signed long 		sint32;
typedef signed long long  	sint64;

typedef uint8 				STD_ReturnType;

/*************** Section: Macro Declarations **********************/

#ifndef NULL
#define NULL 				((void *) 0)
#endif


#define E_NOT_OK 			((STD_ReturnType) 0x00)
#define E_OK 				((STD_ReturnType) 0x01)




#endif
