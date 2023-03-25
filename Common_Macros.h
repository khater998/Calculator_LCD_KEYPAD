/*
 * Common_Macros.h
 *
 *  Created on: Mar 14, 2023
 *      Author: khater
 */

#ifndef COMMON_MACROS_H_
#define COMMON_MACROS_H_

#define SET_BIT(REG, BIT_NO)		(REG |= (1 << BIT_NO))
#define CLR_BIT(REG, BIT_NO)		(REG &= ~(1 << BIT_NO))
#define TOGGLE_BIT(REG, BIT_NO)		(REG ^= (1 << BIT_NO))
#define READ_BIT(REG, BIT_NO)		((REG >> BIT_NO) & 0x01)

#define ROR(REG, NUM)				(REG = ((REG >> NUM) | (REG << 8-NUM)))
#define ROL(REG, NUM)				(REG = ((REG << NUM) | (REG >> 8-NUM)))

#define BIT_IS_SET(REG, BIT_NO)		(((REG >> BIT_NO) & 0x01) == 1)
#define BIT_IS_CLEAR(REG, BIT_NO)	(((REG >> BIT_NO) & 0x01) == 0)


#endif /* COMMON_MACROS_H_ */
