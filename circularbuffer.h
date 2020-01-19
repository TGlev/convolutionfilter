///////////////////////////////////////////////////////////////////////
//	Authors: Ian Baak & Luca Panjer
//	Description: This file is the header that belongs to the circularbuffer.c.
//
///////////////////////////////////////////////////////////////////////

//The size of the buffer. Make sure this is an EVEN number and not an odd number!
//Make sure to also put it inbetween brackets suffixed with a U to tell the compiler it's unsigned. !IMPORTANT!
#define BUFFERSIZE (100U)
#define BUFFERMASK (BUFFERSIZE-1U)

//Prototypes
void buffer_init();
void buffer_write(int data);
int buffer_read(unsigned offset);
