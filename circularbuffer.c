/////////////////////////////////////////////////////////////////////////
//	Authors: Ian Baak & Luca Panjer
//	Description: This file contains the algorithm for the circular buffer
//
///////////////////////////////////////////////////////////////////////

#include "circularbuffer.h"
#include "main.h"


//The main array where the values of the buffer are stored
int buffer[BUFFERSIZE];

//The two variables that keep track of where to read and where to write inside of the circular buffer
int bufferPosition = 0;
/*
 * Import function to initialize the buffer.
 * It basically loops trough all of the elements in the buffer array and zeros them
 */
void buffer_init()
{
	//Set the array to zero
	for(int i = 0; i < BUFFERSIZE; i++)
		buffer[i] = 0;

	//Set the indexes to zero
	bufferPosition = 0;
}

void buffer_write(int data)
{

	//Put the data at the right position in the buffer
	buffer[bufferPosition] = data;

	//Increment the writeIndex
	//Make sure the writeIndex never exceeds the size of the buffer by taking the modulus of the index.
	//e.g. writeindex is 11 and buffersize is 10. 11 % 10 would wrap around to 1.
	bufferPosition = (bufferPosition + 1) % BUFFERSIZE;
	//bufferPosition++;
	//if(bufferPosition > BUFFERSIZE)
		//bufferPosition = 0;
}

int buffer_read(int offset)
{
	//The -1 is necessary to get the newest sample, the newest sample is always the one right before the index
	//The offset is kept in mind to get samples before or after the index
	int readPosition = bufferPosition - offset - 1;

	//Make sure the position doesn't get below 0
	if(readPosition < 0)
		readPosition = readPosition + BUFFERSIZE;

	//Return the value from the buffer
	return buffer[readPosition];
}

