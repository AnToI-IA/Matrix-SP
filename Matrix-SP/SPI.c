/*
 * SPI.c
 *
 * Created: 07.02.2018 19:33:12
 *  Author: AnToI
 */ 
 #include "SPI.h"

 void SPI_init ()
 {
	 // ��������� MOSI � SCK �� �����
	 DDR_SPI = (1<<DD_MOSI) | (1<<DD_SCK) | (1<<DD_CS);
	 PORT_SPI |= (1<<PIN_CS);	// �������� ����� CS
	 
	 SPCR = 0; //�������� ������� SPCR
	 SPSR = 0; //�������� ������� SPSR
	 // ����� �������, ������� fck/8
	 SPCR |= (1<<MSTR) | (1<<SPR1);
	 SPCR |=(1<<SPE);//�������� SPI
	 
	 
 }

 void SPI_MasterTransmit(char cData)
 {
	 /* Start transmission */
	 SPDR = cData;
	 /* Wait for transmission complete */
	 while(!(SPSR & (1<<SPIF))) continue ;
 }