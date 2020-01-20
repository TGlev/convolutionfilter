///////////////////////////////////////////////////////////////////////
//	Authors: Ian Baak & Luca Panjer
//	Description: This file is the header that belongs to the convolution.c.
//
///////////////////////////////////////////////////////////////////////

#define TAPS 141

//Prototypes:
int convolve();
void loadarrays(int* in, int* impuls, int in_size, int impuls_size);
void printarrays(int* in, int* impuls, int in_size, int impuls_size);
void printresultaat(int* out, int output_size);

//Define voor het verkrijgen van arraysize:
#define ARLEN(x) (sizeof(x)/sizeof(x[0]))
