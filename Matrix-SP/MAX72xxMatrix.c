/*
 * MAX72XXMatrix.c
 *
 * Created: 06.02.2018 20:20:58
 *  Author: AnToI-IA
 */ 

#include "MAX72xxMatrix.h"
#include "SPI.h"
#include "util/delay.h"


//#include "main.h"


void Matrix_init ()
{
	fillScreen ();
	// Make sure we are not in test mode
	spiTransfer(OP_DISPLAYTEST, 0);
	_delay_us(1);
	// We need the multiplexer to scan all segments
	spiTransfer(OP_SCANLIMIT, 7);
	_delay_us(1);
	// We don't want the multiplexer to decode segments for us
	spiTransfer(OP_DECODEMODE, 0);
	_delay_us(1);
	// Enable display
	shutdown(false);
	_delay_us(1);
	// Set the brightness to a medium value
	setIntensity(6);
	_delay_us(1);
}

void shutdown(uc b) {
	spiTransfer(OP_SHUTDOWN, b ? 0 : 1);
}

void setIntensity(uc intensity) {
	spiTransfer(OP_INTENSITY, intensity);
}

void fillScreen ()
{
	int tmp,maxspi;
	maxspi=sizeof(bitmap);
	for (tmp=0; tmp<maxspi; tmp++)
	{
		bitmap[tmp] = 0;
	}
}

void matrix_write() 
{
	// Send the bitmap buffer to the displays.
	uc row;
	for (row = OP_DIGIT7; row >= OP_DIGIT0; row-- ) {
		spiTransfer(row,0);
		_delay_us(1);
	}
}

void spiTransfer(unsigned char opcode, uc data)
{
	uc end, start;
	PORT_SPI &= ~(1<<PIN_CS);	// опустим выход чип селект
	end = opcode-OP_DIGIT0;
	start = (displays<<3) + end;
	do {
		start-=8;
		SPI_MasterTransmit(opcode);
		SPI_MasterTransmit(opcode <= OP_DIGIT7 ? bitmap[start] : data);
	}
	while ( start > end );
	PORT_SPI |= (1<<PIN_CS);
}

