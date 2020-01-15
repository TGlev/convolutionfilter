/////////////////////////////////////////////////////////////////////////
//	Authors: Ian Baak & Luca Panjer
//	Description: This file contains the algorithm for the circular buffer
//
///////////////////////////////////////////////////////////////////////

#include "circularbuffer.h"
#include "main.h"

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
	readIndex = 0;
	writeIndex = 0;
}

void buffer_write(int data)
{
	//Make sure the buffer isnt full
	if(isBufferFull > 0)
		return;

	//Put the data at the right position in the buffer
	buffer[writeIndex] = data;

	//Increment the writeIndex
	//Make sure the writeIndex never exceeds the size of the buffer by taking the modulus of the index.
	//e.g. writeindex is 11 and buffersize is 10. 11 % 10 would wrap around to 1.
	writeIndex = (writeIndex + 1) % BUFFERSIZE;

	//If the write and read index are at the same position, the buffer is FULL
	if(writeIndex == readIndex)
		isBufferFull = 1;
}

int buffer_read()
{
	//Read the value from the buffer at the reading position
	int value = buffer[readIndex];

	//Increment the readIndex
	//Make sure the readIndex never exceeds the size of the buffer by taking the modulus of the index.
	//e.g. readIndex is 11 and buffersize is 10. 11 % 10 would wrap around to 1.
	readIndex = (readIndex + 1) % BUFFERSIZE;

	//Because we read from the buffer, the buffer CAN NOT BE full anymore
	isBufferFull = 0;
	return value;
}

