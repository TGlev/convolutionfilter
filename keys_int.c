/*
Author: 	W Pielage & E Helmond & J.F. van der Bent
Date:		13-9-2015
Revision:	5

    keys_int.c:
          Keyboard_interrupt-driver for SpARM-board v1

    pin-info:
           PD1  - R2
           PD2  - R3
           PD3  - R4
           PD6  - K1
           PD7  - K2
           PD8  - K3
           PD9  - K4
           PD11 - R1
           PB0  - Interrupt

To use keyboard on interrupt base:
Use the following line in your main:
	KEYS_INT_init();

G_KEY is a global variable which holds the last pressed button
*/

#include "main.h"
#if KEYS_INT

/****************Globals********************************/
int G_KEY;				// Global variable. G_KEY is filled with the pressed key

void KEYS_INT_init(void)
/* Keys interrupt initialize
 * In this function the ports are set for the keyboard.
 * The rows are outputs and the columns are input.
 * the interrupt pin is connected with 4 diodes to the columns.
 * This pin is connected to External Interrupt 0.
 */
{
	G_KEY = 0;											// Start with a clean key
	EXTI_InitTypeDef   EXTI_InitStructure;				// External interrupt init structure
	GPIO_InitTypeDef gpio;								// GPIO init structure

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);// Enable clock on GPIOB
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);// Enable clock on GPIOD
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);// Enable SYSCFG clock

	gpio.GPIO_OType = GPIO_OType_PP;
	gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;					// No pull-up or down
	gpio.GPIO_Speed = GPIO_Speed_50MHz;					// GPIO speed


	gpio.GPIO_Pin = KEY_ROW;							// Keys rows (see keys_int.h)
	gpio.GPIO_Mode = GPIO_Mode_OUT;						// Output
	GPIO_Init(PKEY_ROW, &gpio);

	gpio.GPIO_Pin = KEY_COL;							// Keys columns (see keys_int.h)
	gpio.GPIO_Mode = GPIO_Mode_IN;						// Input
	gpio.GPIO_PuPd = GPIO_PuPd_DOWN; 					// Pull down
	GPIO_Init(PKEY_COL, &gpio);

	GPIO_SetBits(PKEY_ROW, KEY_ROW);	// Make the row high. When a button is pressed it's always noted

	GPIO_InitTypeDef   GPIO_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;

	/* Configure PB0 pin as input floating */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Pin = KEY_INT;
	GPIO_Init(PKEY_INT, &GPIO_InitStructure);

	/* Connect EXTI Line0 to PB0 pin */
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource0);

	/* Configure EXTI Line0 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	/* Enable and set EXTI Line0 Interrupt to the lowest priority */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void EXTI0_IRQHandler(void)
/* External Interrupt0 Interrupt Request Handler
 * This function is active after a button on the keyboard is pressed
 * This function puts the pressed key in a global variable
 * and can be used to run code with the pressed button
 */
{
	if(EXTI_GetITStatus(EXTI_Line0) != RESET)
	{
		GPIO_ResetBits(GPIOD, KEY_ROW);	// Reset all rows so KEYS_read can start reading
		G_KEY = KEYS_read();			// Read the pressed key

		// Put your code here.. Or not, and use the global
		LCD_XY(0,1);		// Example
		LCD_put("KEY:  ");
		LCD_XY(4,1);
		LCD_putint(G_KEY);

		GPIO_SetBits(GPIOD, KEY_ROW);	// Make the ROW high, so we can wait for an interrupt

		EXTI_ClearITPendingBit(EXTI_Line0);	// Clear the interrupt bit
	}
}

unsigned int KEYS_read(void)
/* Keys read
 * This function reads which key is pressed.
 * It does so by making a row high and check if there is a connection with a column.
 * If there is a connection the value is returned.
 */
{
	unsigned int key = 0;
	GPIO_SetBits(PKEY_ROW, KEY_R4);	// Make row4 high
	key = KEYS_kolom();				// And check if an column has a connection
	if( key != 0 )
		return key + 12;			// If a button is pressed in the last row, add 12 and return

	GPIO_ResetBits(PKEY_ROW, KEY_R4);
	GPIO_SetBits(PKEY_ROW, KEY_R3);	// Make row3 high
	key = KEYS_kolom();				// And check if an column has a connection
	if( key != 0 )
		return key + 8;				// If a button is pressed in the last row, add 8 and return

	GPIO_ResetBits(PKEY_ROW, KEY_R3);
	GPIO_SetBits(PKEY_ROW, KEY_R2);	// Make row2 high
	key = KEYS_kolom();				// And check if an column has a connection
	if( key != 0 )
		return key + 4;				// If a button is pressed in the last row, add 4 and return

	GPIO_ResetBits(PKEY_ROW, KEY_R2);
	GPIO_SetBits(PKEY_ROW, KEY_R1);	// Make row1 high
	key = KEYS_kolom();				// And check if an column has a connection
	if( key != 0 )
		return key;					// If a button is pressed in the last row, return

	GPIO_ResetBits(PKEY_ROW, KEY_R1);
	return 0;
}

unsigned int KEYS_kolom(void)
/* Keys Column
 * In this function the columns are checked if there is a connection.
 * If a column has a connection the return value is the column number.
 */
{
	unsigned int key = 0;	// Check if a row has a connection
	if(GPIO_ReadInputDataBit(PKEY_COL, KEY_K1) == (uint8_t)Bit_SET)
		key = 1;
	if(GPIO_ReadInputDataBit(PKEY_COL, KEY_K2) == (uint8_t)Bit_SET)
		key = 2;
	if(GPIO_ReadInputDataBit(PKEY_COL, KEY_K3) == (uint8_t)Bit_SET)
		key = 3;
	if(GPIO_ReadInputDataBit(PKEY_COL, KEY_K4) == (uint8_t)Bit_SET)
		key = 4;
	return key;
}
#endif
