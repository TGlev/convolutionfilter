/*
Author: 	W Pielage & E Helmond & J.F. van der Bent
Date:		13-9-2016
Revision:	2

    buzzer.c:
          Buzzer-driver for ARM-board v1

    pin-info:
           PC8 - Buzzer

Very annoying buzzer.
To use the buzzer, first use the following line in your code to set the IO:
	BUZZER_init();
Next you can set the buzzer time with:
	BUZZER_set(int counter);
Counter is gives the periodtime
*/

#include "main.h"
#if BUZZER

void BUZZER_init(void)
/* Buzzer initialize
 * Set the IO port to the buzzer on output.
 */
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* GPIOC clock enable */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

}

void BUZZER_set(int counter)
/* Buzzer set
 * Set the total periods.
 * This function can be changed for different frequencies
 */
{
	while(counter--)
	{
	GPIO_ResetBits(GPIOC, GPIO_Pin_8);
	DELAY_us(280);
	GPIO_SetBits(GPIOC, GPIO_Pin_8);
	DELAY_us(280);
	}
}

void morse(char letter)
{
	switch(letter)
	{
	case 'a':
		kort();lang();
		break;
	case 'b':
		lang();kort();kort();kort();
		break;
	case 'c':
		lang();kort();lang();kort();
		break;
	case 'd':
		lang();kort();kort();
		break;
	case 'e':
		kort();
		break;
	case 'f':
		kort();kort();lang();kort();
		break;
	case 'g':
		lang();lang();kort();
		break;
	case 'h':
		kort();kort();kort();kort();
		break;
	case 'i':
		kort();kort();
		break;
	case 'j':
		kort();lang();lang();lang();
		break;
	case 'k':
		lang();kort();lang();
		break;
	case 'l':
		kort();lang();kort();kort();
		break;
	case 'm':
		lang();lang();
		break;
	case 'n':
		lang();kort();
		break;
	case 'o':
		lang();lang();lang();
		break;
	case 'p':
		kort();lang();lang();kort();
		break;
	case 'q':
		lang();lang();kort();lang();
		break;
	case 'r':
		kort();lang();kort();
		break;
	case 's':
		kort();kort();kort();
		break;
	case 't':
		lang();
		break;
	case 'u':
		kort();kort();lang();
		break;
	case 'v':
		kort();kort();kort();lang();
		break;
	case 'w':
		kort();lang();lang();
		break;
	case 'x':
		lang();kort();kort();lang();
		break;
	case 'y':
		lang();kort();lang();lang();
		break;
	case 'z':
		lang();lang();kort();kort();
		break;
	case '1':
		kort();lang();lang();lang();lang();
		break;
	case '2':
		kort();kort();lang();lang();lang();
		break;
	case '3':
		kort();kort();kort();lang();lang();
		break;
	case '4':
		kort();kort();kort();kort();lang();
		break;
	case '5':
		kort();kort();kort();kort();kort();
		break;
	case '6':
		lang();kort();kort();kort();kort();
		break;
	case '7':
		lang();lang();kort();kort();kort();
		break;
	case '8':
		lang();lang();lang();kort();kort();
		break;
	case '9':
		lang();lang();lang();lang();kort();
		break;
	case '0':
		lang();lang();lang();lang();lang();
		break;
	case ' ':
		DELAY_ms(250);
		break;

	}
	DELAY_ms(200);
}

void lang(void)
{
	//BUZZER_set(250);
	GPIO_SetBits(GPIOC, GPIO_Pin_8);
	DELAY_ms(200);
	GPIO_ResetBits(GPIOC, GPIO_Pin_8);
	DELAY_ms(200);

}

void kort(void)
{
	//BUZZER_set(100);
	GPIO_SetBits(GPIOC, GPIO_Pin_8);
	DELAY_ms(50);
	GPIO_ResetBits(GPIOC, GPIO_Pin_8);
	DELAY_ms(200);

}

void BUZZER_D(int counter)
{
	int i;
	while(counter--)		// 280 is i-d-e-aal
		for(i=0;i<26*2;i++)
		{
		GPIO_ResetBits(GPIOC, GPIO_Pin_8);
		DELAY_us(648);
		GPIO_SetBits(GPIOC, GPIO_Pin_8);
		DELAY_us(648);
		}
	DELAY_ms(100);
}
void BUZZER_E(int counter)
{
	int i;
	while(counter--)		// 280 is i-d-e-aal
		for(i=0;i<28*2;i++)
		{
		GPIO_ResetBits(GPIOC, GPIO_Pin_8);
		DELAY_us(608);
		GPIO_SetBits(GPIOC, GPIO_Pin_8);
		DELAY_us(608);
		}
	DELAY_ms(100);
}
void BUZZER_F(int counter)
{
	int i;
	while(counter--)		// 280 is i-d-e-aal
		for(i=0;i<30*2;i++)
		{
		GPIO_ResetBits(GPIOC, GPIO_Pin_8);
		DELAY_us(568);
		GPIO_SetBits(GPIOC, GPIO_Pin_8);
		DELAY_us(568);
		}
	DELAY_ms(100);
}
void BUZZER_G(int counter)
{
	int i;
	while(counter--)		// 280 is i-d-e-aal
		for(i=0;i<34*2;i++)
		{
		GPIO_ResetBits(GPIOC, GPIO_Pin_8);
		DELAY_us(504);
		GPIO_SetBits(GPIOC, GPIO_Pin_8);
		DELAY_us(504);
		}
	DELAY_ms(100);
}
void BUZZER_A(int counter)
{
	int i;
	while(counter--)		// 280 is i-d-e-aal
		for(i=0;i<38*2;i++)
		{
		GPIO_ResetBits(GPIOC, GPIO_Pin_8);
		DELAY_us(456);
		GPIO_SetBits(GPIOC, GPIO_Pin_8);
		DELAY_us(456);
		}
	DELAY_ms(100);
}
void BUZZER_B(int counter)
{
	int i;
	while(counter--)		// 280 is i-d-e-aal
		for(i=0;i<42*2;i++)
		{
		GPIO_ResetBits(GPIOC, GPIO_Pin_8);
		DELAY_us(404);
		GPIO_SetBits(GPIOC, GPIO_Pin_8);
		DELAY_us(404);
		}
	DELAY_ms(100);
}
void BUZZER_C(int counter)
{
	int i;
	while(counter--)		// 280 is i-d-e-aal
		for(i=0;i<45*2;i++)
		{
		GPIO_ResetBits(GPIOC, GPIO_Pin_8);
		DELAY_us(380);
		GPIO_SetBits(GPIOC, GPIO_Pin_8);
		DELAY_us(380);
		}
	DELAY_ms(100);
}
void BUZZER_D2(int counter)
{
	int i;
	while(counter--)		// 280 is i-d-e-aal
		for(i=0;i<48*2;i++)
		{
		GPIO_ResetBits(GPIOC, GPIO_Pin_8);
		DELAY_us(356);
		GPIO_SetBits(GPIOC, GPIO_Pin_8);
		DELAY_us(356);
		}
	DELAY_ms(100);
}
void BUZZER_E2(int counter)
{
	int i;
	while(counter--)		// 280 is i-d-e-aal
		for(i=0;i<51*2;i++)
		{
		GPIO_ResetBits(GPIOC, GPIO_Pin_8);
		DELAY_us(336);
		GPIO_SetBits(GPIOC, GPIO_Pin_8);
		DELAY_us(336);
		}
	DELAY_ms(100);
}
void BUZZER_R(int counter)
{
	int i;
	while(counter--)		// 280 is i-d-e-aal
		for(i=0;i<51*2;i++)
		{
//		GPIO_ResetBits(GPIOC, GPIO_Pin_8);
		DELAY_us(336);
//		GPIO_SetBits(GPIOC, GPIO_Pin_8);
		DELAY_us(336);
		}
	DELAY_ms(100);
}


#endif
