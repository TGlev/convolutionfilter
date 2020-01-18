#include "main.h"

/* 	Convolutiefilter-opdracht, STP
 *  Ian Baak en Luca Panjer
 *  19-12-2019
 *  EV3A
 *
 *  Dit is de main die hoort bij een programma dat een convolutiefilter van
 *  het ARM-bord maakt.
 */

extern uint32_t G_CLK;

//Prototypes of local functions:
void printinfo();
void initboard();

void printinfo() //Function that writes some info to the UART.
{
	char *functionality = //Startup message
			"	Convolutiefilter-opdracht \r\n"
					"	Luca Panjer en Ian Baak, EV3A\r\n"
					"	Vak: STP, Docent: Franc van der Bent & Hubert Schuit\r\n\r\n"
					"	CLK speed: %d Mhz\r\n\r\n";
	//char *version = "Convolutie"; // Showed on LCD at startup

	UART_printf(256, functionality, G_CLK / 1000000);
}

void initboard() //Function that contains all initialize functions of the board I/O.
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	GPIOD->MODER |= (1 << 26);
	GPIOD->MODER |= (1 << 30);

	static int set = 1; // reset identifier

	SystemInit(); // Set SystemCLK
	set++; // if this value is > 1 you have made a serious pointer error, reset happened
	DELAY_init(); // Initialize Delay functions

	UART_init(); // Initialize UART3 Without interrupt.

	LCD_init(); // Initialize LCD-display

	//Generate default kernel for use by the convolution:
	gen_kernel(DEFAULTFREQ, BUFFERSIZE - 1);

	KEYS_INT_init(); //Initialize interrupts for the keys, so that these can be used to reconfigure the sinc function.

	buffer_init(); //Initialize circular buffer.

	DAC_init(Channel_1); // init channel 1 of DA converter full precision
	DAC_init(Channel_2); // init channel 2 of DA converter full precision
	DAC_INT_init();

	ADC_init(Channel_1); // init channel 1 of AD converter full precision
	ADC_init(Channel_2); // init channel 2 of AD converter full precision
}

int main(void)
{
	initboard(); //Initialize I/O.
	printinfo();

	while (1)
		; //ADC and DAC will run under interrupt.
}
