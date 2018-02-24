/*
 * Matrix-SP.c
 *
 * Created: 29.01.2018 19:43:44
 * Author : AnToI
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "main.h"
#include "SPI.h"
#include "adc.h"
#include "MAX72xxMatrix.h"

void TIM2_INIT ()
{
	TCNT2 = 0x00;
	TCCR2B = TIM2_PRESCALER_1024;
	//TIMSK2 = (1<<TOIE2);
}

void init ()
{
	TIM2_INIT();
	
	//DDRB |= (1<<DDRB5);
	SPI_init();
	Matrix_init ();
	adcInit();
}

ISR (TIMER2_OVF_vect)
{
	//if (del--) return;
	//del =60000;
	//for (uint8_t n =0; n<32; n++)
	//{
	//if (bitmapbuff[n][1])bitmapbuff[n][1]-=1;
	//}
	
}

void sdvig (uint8_t n)
{
	uint8_t i;
	for (i=buff_size-1;i;i--)
	{
		bitmapbuff[n][i] = bitmapbuff[n][i-1];
	}
	
}

uint8_t maximum (uint8_t n) 
{
	uint8_t max= 0;
	uint8_t i;
	for (i=0;i<buff_size;i++)
	{
		if (bitmapbuff[n][i]>max)max = bitmapbuff[n][i];
	}

	return max;
}

uint8_t maxdown (uint8_t n)
{
	if (bitmapbuff[n][0]<bitmapbuff[n][1]) 
		bitmapbuff[n][0]=bitmapbuff[n][1]-1;
	
	return (uint8_t)bitmapbuff[n][0];
}

uint8_t sred (uint8_t n) 
{
	uint16_t summ = 0;
	uint8_t i;
	for (i=0;i<buff_size;i++)
	{
		summ += bitmapbuff[n][i];
	}

	return (uint8_t)(summ/buff_size);
}

#define sd    0
uint8_t remap_out[16];
uint8_t bimap[16];

void remap ()
{
	uint8_t i, n;
	uint8_t tmp,tmp2;
	for (i = 0; i<SP_N; i++)	// бегаем по столбикам
	{
		tmp = 0;
		for (n = setremap[i].s1;n<=setremap[i].s2;n++) 
		// бегаме по результату преобразования согласно таблице
		{
			tmp2 = setremap[i].sp->fht_out[n];
			if (tmp2>tmp) tmp = tmp2;
		}
		if (tmp<remap_out[i])remap_out[i]--;
		else remap_out[i] = tmp;	//сохраним максимальный столбец результата
	}
}

void showSpectrum(void)
{
	uint16_t i=0;
	uint8_t n;
	uint8_t val;
	uint16_t bar;
	
	if (spfast.state==WAIT_FHT) getSpData(&spfast);
	else if (splow.state==WAIT_FHT) getSpData(&splow);
	else 
	{
		if (splow.state==WAIT_RUN)
		{
			
			remap();
			for (i = 0; i < 16; i++)
			{
				
				//val = splow.fht_out[i+sd];
				val = remap_out[i];
				if (val > 15) val = 15;
				if(i<8)
				{
					bar = (0xFFFF<<(15-val));
					bitmap[i] = (uint8_t) (bar & 0x00FF);
					bitmap[i+8] = (uint8_t) ((bar & 0xFF00)>>8);
				}
				else
				{
					bar = (0xFFFF>>(15-val));
					uint8_t iinv = 8+(15-i);
					bitmap[iinv+8] = (uint8_t) (bar & 0x00FF);
					bitmap[iinv+16] = (uint8_t) ((bar & 0xFF00)>>8);
				}
			}
						splow.state = 0;
						spfast.state = 0;
						ADCSRA |= (1<<ADATE);
			matrix_write();


		}
	}	
}

int main(void)
{
    /* Replace with your application code */

	init();
	sei();
	
    while (1) 
    {
		if (spfast.state || splow.state) showSpectrum();
    }
}

