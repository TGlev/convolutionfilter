/*
Author: 	W Pielage & E Helmond & J.F. van der Bent
Date:		13-9-2015
Revision:	5

    spi.c:
          SPI-driver for SpARM-board v1

    pin-info:
           PB3 - SCK
           PA6 - MISO
           PA7 - MOSI

To use the SPI interface, first use:
	SPI_init();
After that you can send and receive data with:
	incoming = SPI_get(outgoing);
*/

/****************Libraries******************************/
/* Libraries needed for SPI are (These are in main.h):
 * #include "stm32f4xx.h"
 * #include "stm32f4xx_gpio.h"
 * #include "stm32f4xx_spi.h"
 */

/****************Function Prototypes********************/
void 			SPI_init	(void);
unsigned short 	SPI_get		(unsigned short outgoing);
