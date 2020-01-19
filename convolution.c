///////////////////////////////////////////////////////////////////////
//	Authors: Ian Baak & Luca Panjer
//	Description: This file contains the convolution functions for the
//  digital signal processor.
//
///////////////////////////////////////////////////////////////////////

#include "convolution.h"
#include "main.h"

//Function that executes a convolution on an amount of samples using an impulse response.
int convolve()
{
	int convolvedOutput = 0;

	//Bereken de outputwaarde door een sample met de bijbehorende impulswaarde te vermenigvuldigen en deze allemaal op te tellen.
	//Dit stukje code is bijzonder kort geworden doordat de offset die aan buffer_read gegeven wordt automatisch de juiste sample uit het verleden pakt voor een bepaalde waarde van j.
	for (int j = 0; j < TAPS; j++)
		convolvedOutput += wSincKernel[j] * buffer_read(j);

	//Divide by 10E3 to mitigate the multiplication factor in the filter kernel.
	return convolvedOutput / 10E3;
}
