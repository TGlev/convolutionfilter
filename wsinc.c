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

	//Put cut off frequency on LCD:
	freq_LCD(cut_freq);

	//Convert cut-off-frequency to fraction of the sampling rate (40 kHz)
	if (cut_freq >= 20000)
	{
		UART_printf(50, "Invalid cut off frequency!\r\n");
		return;
	}
	else
		fc = (float) cut_freq / 40000.0; //Cast to float and calculate cut-off:

	UART_printf(35, "(divide by 10E6) fc: %d\r\n", (int) (fc * 10E6));

	//Calculate windowed-sinc with blackman window:
	for (i = 0; i < kernel_size + 1; i++)
	{
		if (i - (kernel_size / 2) == 0)
			kernel_float[i] = 2 * M_PI * fc;
		if (i - (kernel_size / 2) > 0 || i - (kernel_size / 2) < 0)
			kernel_float[i] = sin(2 * M_PI * fc * (i - (kernel_size / 2)))
					/ (i - (kernel_size / 2));

		kernel_float[i] *= (0.42 - 0.5 * cos((2 * M_PI * i) / kernel_size)
				+ 0.08 * cos((4 * M_PI * i) / kernel_size)); //Apply blackman window to every value.
	}

	//Normalize the kernel for unity gain at DC:
	deelsom = 0.0;
	for (i = 0; i < kernel_size + 1; i++) //Add all values together:
	{
		deelsom += kernel_float[i];
	}

	for (i = 0; i < kernel_size + 1; i++) //Divide all values by the sum:
	{
		kernel_float[i] /= deelsom;
	}

	for (i = 0; i < kernel_size + 1; i++) //Convert float kernel to int kernel.
	{
		wSincKernel[i] = kernel_float[i] * 10E3; //Multiplication factor of 10E3. Don't forget to mitigate this afterwards!
	}
}

//Function that reconfigures the kernel based on the key it receives
void reconfigure_kernel(int key)
{
	static int kantelfreq = DEFAULTFREQ; //Default cut off frequency is 5000. Static so this value is always remembered.

	//Write new cut off frequency based on the key:
	kantelfreq = freq_change(key, kantelfreq);

	gen_kernel(kantelfreq, BUFFERSIZE - 1); //Generate new filter kernel

	//Print kernel for testing purposes:
	for (int i = 0; i < BUFFERSIZE; i++)
		UART_printf(20, "%d, ", wSincKernel[i]);
}

//Function that prints the cut off frequency to the LCD
void freq_LCD(int cut_freq)
{
	LCD_clear();
	LCD_XY(0, 0);
	LCD_put("fc:");
	LCD_XY(4, 0);
	LCD_putint(cut_freq);
}

//Function that converts a key to a change of the frequency:
int freq_change(int key, int freq)
{
	switch (key)
	//Determine what to do based on the value of G_KEY
	{
	case 1:
		freq += 1; //Add 1 to the cut off
		break;
	case 5:
		freq -= 1; //Subtract 1 from the cut off
		break;
	case 2:
		freq += 10; //add 10 to the cut off
		break;
	case 6:
		freq -= 10;
		break; //Subtract 10
	case 3:
		freq += 100; //Add 100
		break;
	case 7:
		freq -= 100; //Sub 100
		break;
	case 4:
		freq += 1000; //Add 1000 to the cut off frequency
		break;
	case 8:
		freq -= 1000; //Subtract 1000 from the cut off frequency.
		break;
	default:
		break;
	}

	//Don't allow nonsensical cut off frequencies:
	if (freq <= 0)
		freq = 1;
	if (freq >= 20000)
		freq = 19999;

	return freq;
}
