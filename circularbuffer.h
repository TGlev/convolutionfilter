///////////////////////////////////////////////////////////////////////
//	Authors: Ian Baak & Luca Panjer
//	Description: This file is the header that belongs to the circularbuffer.c.
//
///////////////////////////////////////////////////////////////////////

#define BUFFERSIZE 10

//The main array where the values of the buffer are stored
int buffer[BUFFERSIZE];

//The two variables that keep track of where to read and where to write inside of the circular buffer
int readIndex = 0;
int writeIndex = 0;

//Byte used to indicate if the buffer is full. 0 means not full, 1 means full
char isBufferFull;

//Prototypes
void buffer_init();
void buffer_write(int data);
int buffer_read();
