///////////////////////////////////////////////////////////////////////
//	Authors: Ian Baak & Luca Panjer
//	Description: This file contains the functions required to generate a windowed-sinc
//				 filter kernel.
//
///////////////////////////////////////////////////////////////////////

#include "main.h"
#include "wsinc.h"
#include <math.h>



//Function that generates a windowed sinc in integers (division by 10E6 needed):
void gen_kernel(int cut_freq, int kernel_size)
{
	int i; //Iterator for loops.
	float deelsom = 0.0; //Required for normalization.
	float fc; //Contains the cut off frequency as a fraction.
	float kernel_float[kernel_size + 1]; //Temporary array to hold the kernel before conversion to int.

	//Convert design cut-off-frequency to fraction of the sampling rate (40 kHz)
	if(cut_freq >= 20000)
	{
		UART_printf(50, "Invalid cut off frequency!\r\n");
		return;
	}
	else
		fc = (float)cut_freq / 40000.0; //Cast to double and calculate cut-off:

	UART_printf(35, "(divide by 10E6) fc: %d\r\n", (int)(fc*10E6));

	//Calculate windowed-sinc with blackman window:
	for(i = 0; i < kernel_size + 1; i++)
	{
		if(i - (kernel_size/2) == 0)
			kernel_float[i] = 2*M_PI*fc;
	    if(i - (kernel_size/2) > 0 || i - (kernel_size/2) < 0)
	    	kernel_float[i] = sin(2*M_PI*fc*(i - (kernel_size/2))) / (i - (kernel_size/2));

		kernel_float[i] *= (0.42 - 0.5*cos((2*M_PI*i)/kernel_size) + 0.08*cos((4*M_PI*i)/kernel_size)); //Apply blackman window to every value.
	}

	//Normalize the kernel for unity gain at DC:
	deelsom = 0.0;
	for(i=0; i < kernel_size + 1; i++) //Add all values together:
	{
		deelsom += kernel_float[i];
	}

	for(i=0; i < kernel_size + 1; i++) //Divide all values by the sum:
	{
		kernel_float[i] /= deelsom;
	}

	for(i = 0; i < kernel_size +1; i++) //Convert float kernel to int kernel.
	{
		wSincKernel[i] = kernel_float[i];// * 1E2; Multiplication factor of 10 million. Don't forget to mitigate this afterwards!
	}
}
