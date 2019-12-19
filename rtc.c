/*
Author: 	W Pielage & E Helmond & J.F. van der Bent
Date:		13-9-2015
Revision:	5


    rtc.c:
          RTC driver for SpARM-board v1


To use the RTC use the following initialize:
	RTC_init();
To use with WakeUp interrupt:
	Find rtc.h and change RTC_USE_WAKEUP_ISR to 1
*/

#include "main.h"
#if RTC1	// Value in main.h for the compiler. If set RTC is compiled. Else, code is left behind

/****************Globals********************************/
RTC_TimeTypeDef   RTC_TimeStructure;
RTC_DateTypeDef   RTC_DateStructure;
RTC_InitTypeDef   RTC_InitStructure;

void RTC_init(void)
/* Real Time Clock Initialize
 * Use this function to initialize the RTC
 * If interrupt are enabled this function jumps to the interrupt init.
 * Make sure a crystal is present on the STM32F4 Discovery
 */
{
	uint32_t status=RTC_UNDEFINED;

	// Clock enable for PWR
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

	// Release backup from RTC
	PWR_BackupAccessCmd(ENABLE);

	// Test from Status-Register
	status=RTC_ReadBackupRegister(RTC_STATUS_REG);
	if(status==RTC_STATUS_TIME_OK) // When time is already initialized
	{
		// Wait for sync
		RTC_WaitForSynchro();
		#if RTC_USE_WAKEUP_ISR==1
		RTC_ClearITPendingBit(RTC_IT_WUT);
		EXTI_ClearITPendingBit(EXTI_Line22);
		#endif
	}
	else if(status==RTC_STATUS_INIT_OK) // If RTC is already initialized
	{
		// Wait for sync
		RTC_WaitForSynchro();
		#if RTC_USE_WAKEUP_ISR==1
		RTC_ClearITPendingBit(RTC_IT_WUT);
		EXTI_ClearITPendingBit(EXTI_Line22);
		#endif
	}
	else // If RTC is not yet initialized
	{
		// Init Clock LSE
		RCC_LSEConfig(RCC_LSE_ON);

		// Wait for clock to be stable
		while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);

		// Clock enable for LSE
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

		// enable RTC
		RCC_RTCCLKCmd(ENABLE);

		// Wait for sync
		RTC_WaitForSynchro();

		// Write Status-Register
		RTC_WriteBackupRegister(RTC_STATUS_REG, RTC_STATUS_INIT_OK);

		// Set Time
		RTC_TimeStructure.RTC_Hours = 15;
		RTC_TimeStructure.RTC_Minutes = 34;
		RTC_TimeStructure.RTC_Seconds = 23;
		RTC_SetTime(RTC_Format_BIN, &RTC_TimeStructure);

		// Set Date
		RTC_DateStructure.RTC_Date = 5;
		RTC_DateStructure.RTC_Month = 10;
		RTC_DateStructure.RTC_Year = 13;
		RTC_SetDate(RTC_Format_BIN, &RTC_DateStructure);

		// Settings
		RTC_InitStructure.RTC_AsynchPrediv = 0x7F;
		RTC_InitStructure.RTC_SynchPrediv =  0xFF;
		RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;
		RTC_Init(&RTC_InitStructure);

		// WriteStatus-Register
		if(status!=RTC_UNDEFINED)
		RTC_WriteBackupRegister(RTC_STATUS_REG, RTC_STATUS_TIME_OK);

		status=RTC_INIT_OK;
	}

	#if RTC_USE_WAKEUP_ISR==1
	RTC_SetWakeUpInterrupt();
	#endif
}

#if RTC_USE_WAKEUP_ISR==1
void RTC_SetWakeUpInterrupt(void)
/* Real Time Clock Set WakeUp Interrupt
 * This function sets the wakeup timer and configures the interrupt
 * Default wakeup value is 1 second
 */
{
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef  EXTI_InitStructure;
	uint32_t wakeup_time = 0x7FF;

    // NVIC init
    NVIC_InitStructure.NVIC_IRQChannel = RTC_WKUP_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure); 

    // Init ext Interrupt 22 (for WakeUp)
    EXTI_ClearITPendingBit(EXTI_Line22);
    EXTI_InitStructure.EXTI_Line = EXTI_Line22;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    // To config, wakeup must be disabled
    RTC_WakeUpCmd(DISABLE);

    // Counter 16 => 32,768kHz:16 => 2048 Hz
    RTC_WakeUpClockConfig(RTC_WakeUpClock_RTCCLK_Div16);
    // Init WakeUp Counter
    RTC_SetWakeUpCounter(wakeup_time);

    // enable Interrupt
    RTC_ITConfig(RTC_IT_WUT, ENABLE);

    // enable Wakeup
    RTC_WakeUpCmd(ENABLE);
}
#endif

void RTC_WKUP_IRQHandler(void)
/* Real Time Clock WakeUp Interrupt Request Handler
 * This function is called when an RTC wakeup interrupt happens
 */
{
	if(RTC_GetITStatus(RTC_IT_WUT) != RESET)
	{
		// Clear Interrupt Flags
		RTC_ClearITPendingBit(RTC_IT_WUT);
		EXTI_ClearITPendingBit(EXTI_Line22);

		// Put your own code here
		RTC_TimeTypeDef RTC_Time;
		RTC_GetTime(RTC_Format_BIN, &RTC_Time);
		char c[9];
		snprintf(c,9,"%02d:%02d:%02d",RTC_Time.RTC_Hours,RTC_Time.RTC_Minutes,RTC_Time.RTC_Seconds);
		LCD_XY(8,0);
		LCD_put(c);
	}
}
#endif
