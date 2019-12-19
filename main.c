#include "main.h"

/* 	Analog to Digital and Digital to analog template
 *  J.F. van der Bent Nov 2016 version 1.1
 *  The main.h can be used disable functions but this not needed
 *  Program used timer interrupt to set DA value and if needed, to read AD value
 *  at full precision
 *  Please look into the TIMER example code on the main ARM site on SharePoint to alter timer overrun values
 *  full debugging capability! Have FUN! Total file size 18kB
 */

extern uint32_t G_CLK;


int main(void)
{

	char *functionality =
	"ARM-board v2 \r\
	CLK speed: %d Mhz\r\r";		// Showed on UART3 at startup
	char *version = "ARM M4";	// Showed on LCD at startup

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	GPIOD -> MODER |= ( 1 << 26 );
	GPIOD -> MODER |= ( 1 << 30 );

	static int set=1;		// reset identifier

	SystemInit();			// Set SystemCLK
	set++;					// if this value is > 1 you have made a serious pointer error, reset happened
	DELAY_init();			// Initialise Delay functions

	UART_init();			// Initialise UART3 Without interrupt
	UART_printf(256,functionality,G_CLK/1000000);

	UART_INT_init();		// Initialise UART3 With interrupt enabled (Also use UART_init())

	LCD_init();				// Initialise LCD-display

	//PWM_init();			// Initialise PWM modulation -> Maybe not needed

	LCD_clear();			// Start with a clear display
	LCD_XY(0,0);			// Shift one spot
	LCD_put(version);		// Put the version




	DAC_init(Channel_1);	// init channel 1 of DA converter full precision
	DAC_init(Channel_2);	// init channel 2 of DA converter full precision
	DAC_INT_init();

	ADC_init(Channel_1);	// init channel 1 of AD converter full precision
	ADC_init(Channel_2);	// init channel 2 of AD converter full precision

	while(1); 				// AD/DA will run under interrupt

}
