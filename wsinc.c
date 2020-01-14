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
void gen_kernel(int* pkernel, int cut_freq, int kernel_size)
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
		pkernel[i] = kernel_float[i] * 10E6; //Multiplication factor of 10 million. Don't forget to mitigate this afterwards!
	}
}

//Function that calls the gen_kernel function and spits out the values of the windowed sinc to the UART.
//Useful for testing the gen_kernel function.
void testwsinc_gen()
{
	//Array that contains the kernel of the windowed-sinc filter.
		//Length is detemined by the roll-off, via M = 4/BW, where BW is the desired transition bandwidth as a fraction of the sampling frequency between 0 and 0.5.
		int w_sinc_kernel[KERNELgrot + 1];
		int i;

		gen_kernel(w_sinc_kernel, 2590, KERNELgrot); //Generate kernel by giving the pointer to the array to this function.

		//Print generated kernel:
		for(i = 0; i < KERNELgrot + 1; i++)
			UART_printf(25 ,"%d, ", w_sinc_kernel[i]);
}
