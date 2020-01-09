///////////////////////////////////////////////////////////////////////
//	Authors: Ian Baak & Luca Panjer
//	Description: This file contains the convolution functions for the
//  digital signal processor.
//
///////////////////////////////////////////////////////////////////////

#include "convolution.h"
#include "main.h"

//Function that executes a convolution on an amount of samples using an impulse response.
void convolve()
{
	//Benodigde arrays voor de convolutie:
	int input[80];
	int impulsresp[30];
	int output[109];

	loadarrays(input, impulsresp, ARLEN(input), ARLEN(impulsresp)); //Fill arrays.

	printarrays(input, impulsresp, ARLEN(input), ARLEN(impulsresp)); //Print arrays.

	//Loop voor het uitvoeren van de convolutie met het outputalgoritme:
	for(int i = 0; i < ARLEN(output); i++)
	{
		output[i] = 0; //Maak eerst een outputpunt 0.
		for(int j = 0; j < ARLEN(impulsresp); j++) //Bereken de outputwaarde door een sample met de bijbehorende impulswaarde te vermenigvuldigen en deze allemaal op te tellen.
		{
			//Controleer of de impulsrespons binnen de input-array valt:
			if (i - j < 0)
				continue; //Ga naar de volgende iteratie als de index buiten de arrays valt.
			else if(i - j > 79)
				continue;
			else
				output[i] = output[i] + impulsresp[j] * input[i - j];
		}
	}

	printresultaat(output, ARLEN(output)); //Print results.

	while(1);
}

//Vul de arrays met waarden.
void loadarrays(int* in, int* impuls, int in_size, int impuls_size)
{
	int i = 0;

	in[0] = 3;

	for(i = 1; i < in_size; i++)
		in[i] = in[i-1] + 3; //Vul alles van de array input.

	impuls[0] = 69;

	for(i = 1; i < impuls_size; i++)
		impuls[i] = impuls[i-1] - 2; //Vul alles van de array impulsresp.
}

//Print de waarden van de inputarrays naar de uart.
void printarrays(int* in, int* impuls, int in_size, int impuls_size)
{
	int i = 0;

	UART_printf(20, "\r\nInputwaarde: ");
	for(i=0; i < in_size; i++)
		UART_printf(20, " %d,", in[i]);

	UART_printf(10, "\r\n");

	UART_printf(20, "\r\nImpulswaarde: ");
	for(i=0; i < impuls_size; i++)
		UART_printf(20, " %d,", impuls[i]);
}

//Function for printing the results of the convolution to the UART.
void printresultaat(int* out, int output_size)
{
	int i = 0;

	UART_printf(10, "\r\n");

	for(i = 0; i < output_size; i++)
		UART_printf(128, "uit: %d: %d\r\n", i + 1,out[i]);
}
