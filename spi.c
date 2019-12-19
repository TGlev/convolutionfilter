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
#include "main.h"
#if SPI

void SPI_init(void)
/* SPI Initialize
 * This function sets all SPI pins and SPI mode.
 * If more devices are attached to the SPI-bus, make sure the SlaveSelect are all non-active
 */
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

	/* GPIOA clock enable */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;

	/*-------------------------- GPIO Configuration ----------------------------*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7; // SPI1_MISO & SPI1_MOSI
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	// SPI1_SCK
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* Connect SPI pins to AF */
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_SPI1);  // SCK
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1);  // MISO
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1);  // MOSI

	SPI_InitTypeDef SPI_InitStructure;

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;

	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;

	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
#if MiWi	// MiWi uses a different speed than the SD-card
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;
#else
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;
#endif

	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI1, &SPI_InitStructure);

	// SPI enable
	SPI_Cmd(SPI1, ENABLE);
}

unsigned short SPI_get(unsigned short outgoing)
/* SPI Get
 * SPI get send an 8 bit value to the bus an receives an 8 bit value.
 * This function can also be used for clock generation. Just send 0xFF to activate the clock.
 */
{
#if MiWi	// MiWi does not like the Busy flag much
	uint8_t incoming;
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	SPI1->DR = outgoing;
	while( !(SPI_I2S_GetFlagStatus(SPI1, SPI_FLAG_RXNE) == RESET) );
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET);
	incoming = SPI1->DR;
	return incoming;
#else
	uint8_t incoming;
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET);
	SPI1->DR = outgoing;
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET);
	incoming = SPI1->DR;
	return incoming;
#endif
}
#endif
