///////////////////////////////////////////////////////////////////////
//	Authors: Ian Baak & Luca Panjer
//	Description: This file is the header that belongs to the circularbuffer.c.
//
///////////////////////////////////////////////////////////////////////

#define BUFFERSIZE 100

//Prototypes
void buffer_init();
void buffer_write(int data);
int buffer_read(int offset);
