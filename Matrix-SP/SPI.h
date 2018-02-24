/*
 * SPI.h
 *
 * Created: 06.02.2018 20:45:27
 *  Author: AnToI
 */ 


#ifndef SPI_H_
#define SPI_H_

#include <avr/io.h>
#define F_CPU	16000000UL

#define DDR_SPI					DDRB
#define PORT_SPI				PORTB
#define DD_MOSI					DDB3
#define DD_SCK					DDB5
#define DD_CS					DDB2
#define PIN_CS					PINB2

void SPI_init ();

void SPI_MasterTransmit(char cData);

#endif /* SPI_H_ */