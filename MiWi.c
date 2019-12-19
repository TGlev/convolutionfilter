/*
Author: 	W Pielage & E Helmond
Date:		11-10-2013
Revision:	2

    MiWi.c:
          MiWi-driver (MRF24J40) for SpARM-board v1

    pin-info:
           PB3  - SCK
           PA6  - MISO
           PA7  - MOSI
           PB13 - Reset
           PC5  - Wake
           PC6  - ChipSelect
           PC4  - ExtInterrupt

MiWi-driver for wireless communication between MiWi devices
*/

#include "main.h"
#if MiWi

/****************Globals********************************/
int	busy = 0;
int sequence_number = 0;
unsigned int adres2;
char errors = '0';
uint8_t RW = 0;
MRF24_DATA_PKG MRF24_Rx_Data;									// Structure for RX Data

void MRF24J40_init(unsigned int adres)
/* MRF Initialize
 * Initialize all MRF24J40 pins (SPI is done separately)
 * Interrupt pin is connected to ExtInterrupt 4.
 *
 * The MiWi will be send through a hard reset.
 * After the reset all registers are filled
 */
{
	SPI_init();		// Init SPI with baudrateprescaler 32

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	GPIO_InitTypeDef gpio;
	gpio.GPIO_OType = GPIO_OType_PP;
	gpio.GPIO_PuPd = GPIO_PuPd_UP;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	gpio.GPIO_Mode = GPIO_Mode_OUT;
	gpio.GPIO_Pin = WAKE_MRF;
	GPIO_Init(P_WAKE_MRF, &gpio);
	gpio.GPIO_Pin = RESET_MRF;
	GPIO_Init(P_RESET_MRF, &gpio);
	gpio.GPIO_Pin = CS_MRF;
	GPIO_Init(P_CS_MRF, &gpio);
	GPIO_ResetBits(P_WAKE_MRF, WAKE_MRF);
	GPIO_SetBits(P_CS_MRF, CS_MRF);

	GPIO_SetBits(P_RESET_MRF, RESET_MRF);
	DELAY_ms(1);
	GPIO_ResetBits(P_RESET_MRF, RESET_MRF);
	DELAY_ms(1);
	GPIO_SetBits(P_RESET_MRF, RESET_MRF);
	DELAY_ms(2);

    adres2 = adres;

	MRF24_Write_Short(SOFTRST | RESET_MRF24);					// Perform a software reset
	MRF24_Write_Short(PACON2 | FIFOEN_24 | TXONST);				// Initialise FIFO and configure Transmitter
	MRF24_Write_Short(TXSTBL | RFSTBL);							// Initialise SIFS
	MRF24_Write_Long(RFCON0 | CH_16);							// Channel select : Channel 16
	MRF24_Write_Long(RFCON1 | VCOOPT);							// Configure VCO
	MRF24_Write_Long(RFCON2 | PLLEN);							// Enable PLL
	MRF24_Write_Long(RFCON6 | TXFIL | CLKRECVR);				// Configure TX fliter and Clock Recovery
	MRF24_Write_Long(RFCON7 | SLPCLKDIV);						// Select 100 kHz sleep clock
	MRF24_Write_Long(RFCON8 | RFVCO);							// Configure VCO Control
	MRF24_Write_Long(SLPCON1 | CLKOUTEN | SLPCLKSEL);			// Disable CLKOUT pin, configure Sleep Clock Divisor
	MRF24_Write_Short(BBREG2 | CCA | CCACSTH);					// Set Clear-Channel-Assessment to Energy-Detection
	MRF24_Write_Short(CCAEDTH | CCAED);							// Set Energy-Detection treshold
	MRF24_Write_Short(BBREG6 | RSSIMODE);						// Caculate RSSI per packet, store in RXFIFO
	MRF24_Write_Short(TXPEND | MLIFS);							// Spacing between frames
	MRF24_Write_Short(TXTIME | TURNTIME);						// Spacing between sending and receiving
	MRF24_Write_Short(TXSTBL | RFSTBL);							// VCO stabilization time and frame spacing
	MRF24_Write_Long(SLPCON0 | INTEDGE | SLPCLKEN);				// Interrupt on falling edge, Sleep clock enabled
	MRF24_Write_Short(INTCON | INTDISABLE);						// All interrupts disabled
	MRF24_Write_Long(RFCON3 | TXPOWER);							// Transmitter max power
	MRF24_Write_Short(RFCTL | RXTXRST);							// Reset transmitter and receiver
	MRF24_Write_Short(RFCTL | RXTXSET);							// Transmitter and receiver off

	RW = 0;
	DELAY_us(192);
	MRF_INT_init();
}

void MRF24J40_send_byte(char data, unsigned short adres)
/* MRF24J40 Send Byte
 * Send a byte to specified adress.
 */
{
	MRF24J40_send(&data, 1, adres);
}

void MRF24J40_send_string(char *tekst, unsigned short adres)
/* MRF24J40 Send String
 * Send a string to specified adress.
 */
{
	int Data_length_24 = 0;

	while(tekst[Data_length_24] != '\0')
		Data_length_24++;

	MRF24J40_send(tekst, Data_length_24, adres);
}

void MRF24J40_send(char *data, int Data_length_24, unsigned int adres)
/* MRF24J40 Send
 * Send data to sepcified adress.
 * Length is neccesary. A dataframe is made ready to send.
 */
{
	if(busy == 1)
	while(busy);	// sending in progress?? Wait forever
	busy = 1;
	//Create dataframe
	int i, j=0;
	MRF24_Write_Short(INTCON | TXNIE);							// Enable transmit interrupt
	MRF24_Write_Long((0x000000 << 1) | HEADER_SIZE);			// Header length = 9
	MRF24_Write_Long((0x001000 << 1) | (Data_length_24 + HEADER_SIZE));	// Frame length = datalength + header length
	MRF24_Write_Long((0x002000 << 1) | FRAMECTRL1);				// FrameControl; one PAN frame, ask for Ack, no following packets, usecured, data trasfer
	MRF24_Write_Long((0x003000 << 1) | FRAMECTRL2);				// FrameControl; source, PAN and destination addresses are 2 bytes each
	MRF24_Write_Long((0x004000 << 1) | ((sequence_number++)&0xFF));// Sequence Number
	MRF24_Write_Long((0x005000 << 1) | ((adres & 0xFF00)>>8));	// Destination PAN
	MRF24_Write_Long((0x006000 << 1) | (adres & 0x00FF));		// Destination PAN
	MRF24_Write_Long((0x007000 << 1) | ((adres & 0xFF00)>>8));	// Destination address
	MRF24_Write_Long((0x008000 << 1) | (adres & 0x00FF));		// Destination address
	MRF24_Write_Long((0x009000 << 1) | ((adres2 & 0xFF00)>>8));	// Source address
	MRF24_Write_Long((0x00A000 << 1) | (adres2 & 0x00FF));		// Source address
	for(i=0x00B000;i<(0x00B000 + (Data_length_24<<12));i=i+0x1000)
	{
		MRF24_Write_Long((i << 1) | data[j]);					// Payload
		j++;
	}
	MRF24_Write_Short(TXCON | TXNTRIG | TXNACKREQ);				// Start transmission with Ack
	RW = 0;
	// wait for interrupt (+/- 15ms)
}

void MRF24J40_receive(void)
/* MRF24J40 Receive
 * This function asks if there is a packet available on the MRF24J40.
 * Packet receive is handled in the interrupt routine.
 */
{
	MRF24_Write_Short(PANIDL | ((adres2 & 0xFF00)>>8));			// Set Receiver PAN address
	MRF24_Write_Short(PANIDH | (adres2 & 0x00FF));				// Set Receiver PAN address
	MRF24_Write_Short(SADRL | ((adres2 & 0xFF00)>>8));			// Set Receiver address
	MRF24_Write_Short(SADRH | (adres2 & 0x00FF));				// Set Receiver address
	MRF24_Write_Short(RXMCR | RX_MODE);							// Receiver in Normal mode: 0x00, Accept errors: 0x03, pancoordinator: 0x04
	MRF24_Write_Short(INTCON | RXIE);							// Enable Receive interrupt
	RW = 1;
}

void MRF24J40_error(void)
/* MRF24J40 Error
 * If there is an error, ask the error code.
 */
{
	short error;
	error = SPI_get(0xFF);
}

void MRF24_Write_Short(unsigned short data)
/* MRF24J40 Write Short
 * Write an 16 bit value to the MRF24J40.
 */
{
		MRF24_Read_Short(data | 0x0100);								// Send data | Write bit
}

unsigned short MRF24_Read_Short(unsigned short data)
/* MRF24J40 Read Short
 * Perform a read instruction. First 8 bits are:
 * Bit 8: short or long command (0 is short)
 * Bit 7:1: command
 * Bit 0: Read or write command (If Write short, this bit is set)
 * Return value with a read is the next 8 bits
 */
{
	unsigned short temp;

	GPIO_ResetBits(P_CS_MRF, CS_MRF);					// Chip Select Active
    temp = SPI_get((data & 0xFF00)>>8);					// Send MSB 8 Bits
    temp = SPI_get((data & 0x00FF));					// Send LSB 8 Bits
    GPIO_SetBits(P_CS_MRF, CS_MRF);						// CS not active
    DELAY_us(5);

    return temp;
}

void MRF24_Write_Long(unsigned int data)
/* MRF24J40 Write Long
 * Write an 24 bit value to the MRF24J40.
 */
{
		MRF24_Read_Long(data | 0x001000);		// Send data | Write bit
}

unsigned short MRF24_Read_Long(unsigned int data)
/* MRF24J40 Read Short
 * Perform a read instruction. First 12 bits are:
 * Bit 12: short or long command (1 is long)
 * Bit 11:1: command
 * Bit 0: Read or write command (If Write long, this bit is set)
 * Return value with a read is the next 12 bits, where the 4 MSB are not defined.
 */
{
	unsigned int temp;

	GPIO_ResetBits(P_CS_MRF, CS_MRF);					// Chip Select Active
	temp = 	SPI_get(((data & 0xFF0000)>>16)|0x80);		// Send MSB 12 Bits | Long bit
	temp = 	SPI_get(((data & 0x00FF00)>>8));			// Send MiddleSB 12 Bits
    temp = 	SPI_get(((data & 0x0000FF)));				// Send LSB 12 Bits
    GPIO_SetBits(P_CS_MRF, CS_MRF);						// CS not active
    DELAY_us(5);
	return temp;
}

void MRF24J40_sleep(void)
/* MRF24J40 Sleep
 * If the MiWi is not used, one can set is to sleep mode to reduce its power consumtion
 */
{
	MRF24_Write_Short(SOFTRST | RSTPWR);				// Power management reset
	MRF24_Write_Short(SLPACK | WAKECNT);				// Put to sleep
}

void MRF24J40_wake(void)
/* MRF24J40 Wake
 * After a sleep command the MiWi needs to get a wake command to become active
 */
{
	GPIO_SetBits(P_WAKE_MRF, WAKE_MRF);					// Wake MRF24J40
	DELAY_ms(2);										// delay for stabilisation oscillator
	GPIO_ResetBits(P_WAKE_MRF, WAKE_MRF);
}

void StringCopy(char *destination, char *source, int length)
/* String Copy
 * This function copies a string.
 */
{
	int i = 0;

	while(i<length)
	{
		destination[i] = source[i];
		i++;
	}
}

void MRF_INT_init(void)
/* MRF24J40 Interrupt Initialize
 * If the MiWi received a package in receive option, an interrupt pin gets low.
 * This functions is the initializing of the interrupt pin and handler.
 */
{
	EXTI_InitTypeDef   EXTI_InitStructure;	// External interrupt init structure
	GPIO_InitTypeDef   GPIO_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;

	/* Enable GPIOC clock */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	/* Enable SYSCFG clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	/* Configure PC4 pin as input floating */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = INT_MRF;
	GPIO_Init(P_INT_MRF, &GPIO_InitStructure);

	/* Connect EXTI Line4 to PC4 pin */
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource4);

	/* Configure EXTI Line4 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line4;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	/* Enable and set EXTI Line4 Interrupt to the lowest priority */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	EXTI_ClearITPendingBit(EXTI_Line4);
}

void EXTI4_IRQHandler(void)
/* External 4 Interrupt Request Handler
 * This function is active after an interrupt from the MiWi.
 * If a datablock is received, the data is stripped and put on the UART.
 * If the MiWi sends an acknowledge back to a set of commands, this function checks for
 * errors.
 */
{
	if(EXTI_GetITStatus(EXTI_Line4) != RESET)
	{
		GPIO_ToggleBits(LED2);
		char error_tx_stat = 0;
		char int_stat = 0;
		int i, j;
		int Received;
		char Received_Data[128];

		if (RW == 1)
			Received = 1;
		else
			Received = 0;
		RW == 0;

		int_stat = MRF24_Read_Short(INTSTAT);
		if(int_stat)										// Transmit mode, TX Normal FIFO Release Interrupt
		{
			error_tx_stat = MRF24_Read_Short(TXSTAT);
			if(error_tx_stat != 0)							// Check TX MAC Status Register for errors
				MRF24J40_error();

			busy = 0;										// Sending complete
			MRF24_Write_Short(INTCON | RXIE);				// Enable Receive interrupt
		}

		if( Received == 1 )
			if(int_stat)									// Receive mode, FIFO is full
			{
				MRF24_Write_Short(BBREG1 | RXDECINV);	// Disable receiver
				i = (MRF24_Read_Long(RXFIFO));			// Read RXFIFO, first byte is frame length, i = frame length -9 is data length

				for(j = 0 ; j<=i ; j++)					// Read data from FIFO buffer
					Received_Data[j] = MRF24_Read_Long(RXFIFO | (j << 13));	// Read RXFIFO

				int k;
				UART_printf(32,"MiWi: ");
				for(k=10;k<(i-1);k++)
					UART_printf(2,"%c",Received_Data[k]);
				UART_printf(2,"\n");
				MRF24_Rx_Data.Frame_Length = Received_Data[0];	// Parse data into MIWI_DATA_PKG Struct
				StringCopy(MRF24_Rx_Data.MAC_Control, &Received_Data[1], 2);
				MRF24_Rx_Data.Payload_Length = Received_Data[0] - 11;
				MRF24_Rx_Data.Seq_Nr = Received_Data[3];
				StringCopy(MRF24_Rx_Data.Destination_PAN, &Received_Data[4], 2);
				StringCopy(MRF24_Rx_Data.Destination_Addr, &Received_Data[6], 2);
				StringCopy(MRF24_Rx_Data.Source_Addr, &Received_Data[8], 2);
				StringCopy(MRF24_Rx_Data.Payload, &Received_Data[10], i - 11);
				MRF24_Rx_Data.Payload[i-11] = '\0';
				StringCopy(MRF24_Rx_Data.FCS, &Received_Data[10 + (i-11)], 2);

				MRF24_Write_Short(RXFLUSH | RX_FLUSH);			// Clear RX FIFO buffer
				MRF24_Write_Short(BBREG1);						// Enable receiver
			}
		EXTI_ClearITPendingBit(EXTI_Line4);
	}
}
#endif
