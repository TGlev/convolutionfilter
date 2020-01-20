///////////////////////////////////////////////////////////////////////
//	Authors: Ian Baak & Luca Panjer
//	Description: This file is the header that belongs to the circularbuffer.c.
//
///////////////////////////////////////////////////////////////////////

//The size of the buffer. Make sure the number is a power of 2!
//Make sure to also put it inbetween brackets suffixed with a U to tell the compiler it's unsigned. !IMPORTANT!
#define BUFFERSIZE (256U)
#define BUFFERMASK (BUFFERSIZE-1U)

//Prototypes
void buffer_init();
void buffer_write(int data);
int buffer_read(unsigned offset);
