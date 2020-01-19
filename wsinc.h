///////////////////////////////////////////////////////////////////////
//	Authors: Ian Baak & Luca Panjer
//	Description: Header belonging to wsinc.c
//
///////////////////////////////////////////////////////////////////////

//Prototypes:
void gen_kernel(int cut_freq, int kernel_size); //Cut_freq in Hz.
void reconfigure_kernel(int key);
void freq_LCD(int cut_freq);
int freq_change(int key, int freq);
