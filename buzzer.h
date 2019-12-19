/*
Author: 	W Pielage & E Helmond & J.F. van der Bent
Date:		13-9-2015
Revision:	5


    buzzer.c:
          Buzzer-driver for SpARM-board v1

    pin-info:
           PC8 - Buzzer

Very annoying buzzer.
To use the buzzer, first use the following line in your code to set the IO:
	BUZZER_init();
Next you can set the buzzer time with:
	BUZZER_set(int counter);
Counter is gives the periodtime
*/

/****************Libraries******************************/
/* Libraries needed for Buzzer are (These are in main.h):
 * #include "stm32f4xx.h"
 * #include "stm32f4xx_gpio.h"
 * #include
 */


/****************Function Prototypes********************/
void 	BUZZER_init		(void);
void 	BUZZER_set		(int counter);

void 	morse			(char letter);
void 	lang			(void);
void 	kort			(void);

void	BUZZER_D		(int counter);
void	BUZZER_E		(int counter);
void	BUZZER_F		(int counter);
void	BUZZER_G		(int counter);
void	BUZZER_A		(int counter);
void	BUZZER_B		(int counter);
void	BUZZER_C		(int counter);
void	BUZZER_D2		(int counter);
void	BUZZER_E2		(int counter);
void	BUZZER_R		(int counter);
