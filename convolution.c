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
	for(int j = 0; j < BUFFERSIZE; j++) //Bereken de outputwaarde door een sample met de bijbehorende impulswaarde te vermenigvuldigen en deze allemaal op te tellen.
		convolvedOutput += wSincKernel[j] * buffer_read(j);

	return convolvedOutput;
}
