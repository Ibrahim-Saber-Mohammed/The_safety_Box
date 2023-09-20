/*
 * Application.h
 *
 *  Created on: Aug 30, 2021
 *      Author: ibrahim
 */

#ifndef APPLICATION_APPLICATION_H_
#define APPLICATION_APPLICATION_H_

#define MOTOR_1		GPIOC , PIN1
#define MOTOR_2		GPIOC , PIN2

#define NOT_PRESSED 		0
#define PASS_SIZE			4
#define OPEN_DOOR			1
#define CLOSE_DOOR			0
#define MAX_TRIES			3
#define PASSWORD_BASE_ADDRESS		 0x00
#define FIRST_RUN_FLAG_ADDRESS		(0x05)

u8 ComparePass(u8 * Copy_ptrArray1, u8 * Copy_ptrArray2 , u8 Copy_u8ArraySize);

#endif /* APPLICATION_APPLICATION_H_ */
