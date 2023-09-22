/*
 * main.c
 *
 *  Created on: Aug 12, 2021
 *      Author: ibrahim
 */
#include "../LIB/STD_TYPES/STD_TYPES.h"
#include  "../LIB/BIT_MATH/BIT_MATH.h"

#include "../MCAL/DIO/GPIO_interface.h"
#include "../MCAL/EEPROM/EEPROM_interface.h"
#include "../MCAL/SPI/SPI_interface.h"
#include "../HAL/LCD/CLCD_interface.h"
#include "../HAL/KeyPad/KEYPAD_interface.h"
#include "util/delay.h"

#include "Application.h"


int main(void)
{
	u8 FirstTimeLogin = 0;
	u8 Pressed_Key = 0;
	u8 FirstDigPlace = 6;
	u8 LoopIterator = 0;
	u8 EnteredPassword[PASS_SIZE];
	u8 ReadEEPROMPassword[PASS_SIZE];
	s8 NumberOfTries = MAX_TRIES;
	u8 Local_u8Check = 0;
	u8 Local_u8LoginFinished = 0;

	CLCD_voidInit(); // initialize the LCD
	KEYPAD_voidInit(); // initialize the LCD

	/* Set the Password for First Time */
	EEPROM_voidReadByte(FIRST_RUN_FLAG_ADDRESS,&FirstTimeLogin);

	SPI_MasterConfigs_t MASTER ;
	MASTER.Mode = SPI_MASTER;
	MASTER.Spi_Interrupt = SPI_DISABLE_INTERRUPT;
	MASTER.Spi_ClockPhase = SPI_LEADING_SETUP;
	MASTER.Spi_ClockPolarity = SPI_LEADING_FALLING;
	MASTER.Spi_DataOrder = SPI_MSB_FIRST;
	MASTER.Spi_Speed = FCLK_16;
	MASTER.Spi_State = SPI_ENABLE;
	SPI_u8SetMasterConfigs(&MASTER);

	while(FirstTimeLogin == 0xFF)
	{
		/* LCD_Messages */
		CLCD_voidSendString((u8*)"Welcome to our");
		CLCD_voidGoToX_Y(CLCD_ROW2, CLCD_COL1);
		CLCD_voidSendString((u8*)"System");
		_delay_ms(2000);
		CLCD_voidClearScreen();
		CLCD_voidGoToX_Y(CLCD_ROW1, CLCD_COL1);
		CLCD_voidSendString((u8*)"Login for first");
		CLCD_voidGoToX_Y(CLCD_ROW2, CLCD_COL1);
		CLCD_voidSendString((u8*)"Time");
		_delay_ms(1000);
		CLCD_voidClearScreen();
		CLCD_voidGoToX_Y(CLCD_ROW1, CLCD_COL1);
		CLCD_voidSendString((u8*)"Set Password");
		CLCD_voidGoToX_Y(CLCD_ROW2, CLCD_COL1);
		CLCD_voidSendString((u8*)"Pass:");
		for(LoopIterator = 0; LoopIterator < PASS_SIZE; LoopIterator++)
		{
			do
			{
				Pressed_Key = KEYPAD_voidGetPressedButton();
			}while(Pressed_Key == NOT_PRESSED);

			CLCD_voidGoToX_Y(CLCD_ROW2, FirstDigPlace);
			CLCD_voidSendCharacter(Pressed_Key);
			EnteredPassword[LoopIterator] = Pressed_Key;
			_delay_ms(200);
			CLCD_voidGoToX_Y(CLCD_ROW2, FirstDigPlace);
			CLCD_voidSendCharacter('*');
			FirstDigPlace++ ;
		}
		EEPROM_voidWriteBlock(PASSWORD_BASE_ADDRESS, PASS_SIZE, EnteredPassword);
		EEPROM_voidWriteByte(FIRST_RUN_FLAG_ADDRESS, 0);
		CLCD_voidClearScreen();
		CLCD_voidSendString((u8*)"Password Saved");
		_delay_ms(1000);
		CLCD_voidClearScreen();
		EEPROM_voidReadByte(FIRST_RUN_FLAG_ADDRESS,&FirstTimeLogin);
		FirstDigPlace = 6;
		Pressed_Key =NOT_PRESSED;
	}

	while(1)
	{

		while( (NumberOfTries>0 ) && (Local_u8LoginFinished == 0 ) ){
			CLCD_voidGoToX_Y(CLCD_ROW1, CLCD_COL1);
			CLCD_voidSendString((u8*)"Plz,Enter Pass");
			CLCD_voidGoToX_Y(CLCD_ROW2, CLCD_COL1);
			CLCD_voidSendString((u8*)"Pass:");
			for(LoopIterator = 0; LoopIterator < PASS_SIZE; LoopIterator++)
			{
				do
				{
					Pressed_Key = KEYPAD_voidGetPressedButton();
				}while(Pressed_Key == NOT_PRESSED);
				CLCD_voidGoToX_Y(CLCD_ROW2, FirstDigPlace);
				CLCD_voidSendCharacter(Pressed_Key);
				_delay_ms(200);
				CLCD_voidGoToX_Y(CLCD_ROW2, FirstDigPlace);
				CLCD_voidSendCharacter('*');
				EnteredPassword[LoopIterator] = Pressed_Key;
				Pressed_Key = NOT_PRESSED;
				FirstDigPlace++;
			}
			/* Read Password from EEPROM and Compare With the Entered Password */

			EEPROM_voidReadBlock(PASSWORD_BASE_ADDRESS,PASS_SIZE, ReadEEPROMPassword );
			Local_u8Check = ComparePass(EnteredPassword, ReadEEPROMPassword, PASS_SIZE);
			if(Local_u8Check==1)
			{
				CLCD_voidClearScreen();
				CLCD_voidGoToX_Y(CLCD_ROW1, CLCD_COL1);
				CLCD_voidSendString((u8*)"Welcome..");
				Local_u8LoginFinished = 1;
				break;
			}
			else{
				CLCD_voidClearScreen();
				CLCD_voidGoToX_Y(CLCD_ROW1, CLCD_COL1);
				CLCD_voidSendString((u8*)"Wrong PassWord..");
				CLCD_voidGoToX_Y(CLCD_ROW2, CLCD_COL1);
				CLCD_voidSendString((u8*)"Try again ");
				_delay_ms(1000);
				CLCD_voidClearScreen();
				Local_u8LoginFinished = 0;
				NumberOfTries -=1;
				FirstDigPlace = 6;
			}
		}

		if(Local_u8LoginFinished == 1)
		{
			/* Open the Door */
			SPI_u8TransmitReceive(OPEN_DOOR);
			do
			{
				Pressed_Key = KEYPAD_voidGetPressedButton();

			}while(Pressed_Key == NOT_PRESSED || Pressed_Key!= 'C');
			SPI_u8TransmitReceive(CLOSE_DOOR);
		}
		else
		{
			CLCD_voidClearScreen();
			CLCD_voidGoToX_Y(CLCD_ROW1 , CLCD_COL1);
			CLCD_voidSendString((u8*)"No more tries");
			CLCD_voidGoToX_Y(CLCD_ROW2 , CLCD_COL1);
			CLCD_voidSendString((u8*)"wait 20 sec");
			_delay_ms(20000);
			Local_u8LoginFinished = 0;
			NumberOfTries = MAX_TRIES;
		}

	}

	return 0;
}

