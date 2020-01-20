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

	//Set the index to zero
	bufferPosition = 0;
}

/*
 * The buffer is made significantly faster by "calculating overflows out" instead of comparing two numbers and then changing the index accordingly.
 * This works by doing a bitwise AND with the saved index and a mask that is the size of the buffer - 1.
 * This calculation results in the address of where to store our sample.
 * The disadvantage to doing this is that the circular buffer always has to be an even number in size.
 * To understand this, let's imagine a buffer with a size of 4. We will start sampling and placing the samples in the buffer one by one.
 * To make the concept clear, we will show each bitwise AND.
 * Our index starts at 0 and our mask is 3 (011).
 * For sample 0, the calculation will be: 000 & 011 = 000
 * For sample 1, the calculation will be: 001 & 011 = 001
 * For sample 2, the calculation will be: 010 & 011 = 010
 * For sample 3, the calculation will be: 011 & 011 = 011
 * For these first four samples, the bitwise AND doesn't seem to have any effect at all.
 * Now, watch the magic happen as we try to put a sample into position 4
 * For sample 4, the calculation will be 100 & 011 = 000
 * The 4th sample will be placed in position 0 again! This principle will also work if you have a way larger number, let's say 127(1111111)
 * Sample 128; 1111111 & 011 = 011
 */

void buffer_write(int value)
{
    buffer[(++bufferPosition) & BUFFERMASK] = value;
}

int buffer_read(unsigned offset)
{
    return buffer[(bufferPosition - offset) & BUFFERMASK];
}

/*
 * OLD & SLOW BUFFER CODE
 */
/*
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

int buffer_read(unsigned offset)
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
*/
