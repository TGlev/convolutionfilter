/*
Author: 	W Pielage & E Helmond & J.F. van der Bent
Date:		29-11-2016
Revision:	3

    dac.c:
          DAC driver with or without interrupts for ARM-board v5

    pin-info:
           PA4 - DAC Channel 1
           PA5 - DAC Channel 2


To use the DAC channel 1 or 2 use the following initialize:
	DAC_init(Channel_1);
	DAC_init(Channel_2);
To use a timer interrupt (TIM3) use:
	DAC_INT_init();
The interrupt handler called is TIM3_IRQHandler
*/

#include "main.h"
#if DA

uint32_t Data = 0;
uint32_t Data1 = 0;
uint32_t Data2 = 250;

uint32_t Data;



void DAC_init(int Channel)
/* DAC Initialize
 * Use this function to initialize the DAC channel 1 or 2.
 * The output of the DAC channel 1 is on pin PA.4.
 * The output of the DAC channel 2 is on pin PA.5.
 * The DAC can be loaded with:
 * (Channel_1) DAC_SetChannel1Data(uint32_t DAC_Align, uint16_t Data)
 * (Channel_2) DAC_SetChannel1Data(uint32_t DAC_Align, uint16_t Data)
 * DAC_Align can be: DAC_Align_12b_R
 * 					 DAC_Align_12b_L
 * 					 DAC_Align_8b_R
 * Data is the data that has to be converted.
 */
{
  /* Preconfiguration before using DAC----------------------------------------*/
	GPIO_InitTypeDef GPIO_InitStructure;
	DAC_InitTypeDef  DAC_InitStructure;

	/* DMA1 clock and GPIOA clock enable (to be used with DAC) */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	/* DAC Periph clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);

	/* DAC channel 1 & 2 (DAC_OUT1 = PA.4, DAC_OUT2 = PA.5) configuration */
	if( Channel == Channel_1)
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	else
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* DAC channel1 Configuration */
	DAC_InitStructure.DAC_Trigger = DAC_Trigger_None;
	DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
	DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
	if( Channel == Channel_1)
	{
		DAC_Init(DAC_Channel_1, &DAC_InitStructure);
		/* Enable DAC Channel1 */
		DAC_Cmd(DAC_Channel_1, ENABLE);
	}
	else
	{
		DAC_Init(DAC_Channel_2, &DAC_InitStructure);
		/* Enable DAC Channel1 */
		DAC_Cmd(DAC_Channel_2, ENABLE);
	}
}

void DAC_INT_init(void)
/* DAC Interrupt Initialize
 * This function can be used to start a timer (TIM3).
 * The timer generates interrupt which can be used to
 * load the DAC.
 */
{
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	/* Enable the TIM3 gloabal Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* TIM3 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	/* Time base configuration */

	TIM_TimeBaseStructure.TIM_Period = 3000;
	TIM_TimeBaseStructure.TIM_Prescaler = 1;

	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	/* TIM IT enable */
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	/* TIM3 enable counter */
	TIM_Cmd(TIM3, ENABLE);
}

void TIM3_IRQHandler(void)
/* TIM3 Interrupt Request Handler
 * This function is called when an TIM3 interrupt occurred.
 * In this function you can load the DAC channels so the step size
 * stays the same.
 */
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);

		// With interrupt enabled, this will be done to the DAC channels
		if (Data1++>512) Data1=0;
		DAC_SetChannel1Data(DAC_Align_12b_R, Data1);
		//DAC_SetChannel2Data(DAC_Align_12b_R, Data1);

		// example read ADC value and store in DAC
		//DAC_SetChannel1Data(DAC_Align_12b_R, Get_ADC_Value(Channel_1));

		DAC_SetChannel2Data(DAC_Align_12b_R, (Get_ADC_Value(Channel_2) + Get_ADC_Value(Channel_1)));  // also called a very expensive wire!
	}
}
#endif
