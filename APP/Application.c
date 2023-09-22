/*
 * Application.c
 *
 *  Created on: Aug 30, 2021
 *      Author: ibrahim
 */


#include "../LIB/STD_TYPES/STD_TYPES.h"
#include "../LIB/BIT_MATH/BIT_MATH.h"

u8 ComparePass(u8 * Copy_ptrArray1, u8 * Copy_ptrArray2 , u8 Copy_u8ArraySize)
{
	u8 Local_u8CheckResult = 1;
	u8 Local_u8LoopIterator = 0;
	for(Local_u8LoopIterator = 0; Local_u8LoopIterator < Copy_u8ArraySize; Local_u8LoopIterator++)
	{
		if( Copy_ptrArray1[Local_u8LoopIterator] != Copy_ptrArray2[Local_u8LoopIterator])
		{
			Local_u8CheckResult = 0;
			break;
		}
		else
		{
			continue;
		}
	}
	return Local_u8CheckResult;
}
