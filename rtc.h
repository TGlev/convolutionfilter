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

/****************Libraries******************************/
/* Libraries needed for RTC are (These are in main.h):
 * #include "stm32f4xx.h"
 * #include "stm32f4xx_conf.h"
 * #include "stm32f4xx_rcc.h"
 * #include "stm32f4xx_rtc.h"
 * #include "stm32f4xx_exti.h"
 * #include "misc.h"
 */

/****************Defines********************************/
#define  RTC_STATUS_REG      RTC_BKP_DR0  // Status Register
#define  RTC_STATUS_INIT_OK  0x35AC       // RTC Initialized
#define  RTC_STATUS_TIME_OK  0xA3C5       // RTC time set
#define  RTC_USE_WAKEUP_ISR      1  // WakeUp-Interrupt per RTC

/****************Globals********************************/
typedef enum {
  RTC_UNDEFINED =0, // RTC is not yet initialized
  RTC_INIT_OK,      // RTC is initialized
  RTC_TIME_OK       // RTC is initialized and time is set
}RTC_STATUS_t;

/****************Function Prototypes********************/
void 	RTC_init				(void);
#if RTC_USE_WAKEUP_ISR==1
void 	RTC_SetWakeUpInterrupt	(void);
void 	RTC_WKUP_IRQHandler		(void);
#endif
