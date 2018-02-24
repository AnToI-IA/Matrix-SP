#include <avr/io.h>
#include <avr/interrupt.h>
#include "util/delay.h"
#include <avr/pgmspace.h>



#include "adc.h"
#include "FHT/FHT.h"
#include "fft.h"
#include "MAX72xxMatrix.h"

void adcInit()
{
	TCCR0B |= (0<<CS02) | (0<<CS01) | (1<<CS00);
	TIMSK0 |= (1<<TOIE0);

	// Enable ADC with prescaler 16
	ADCSRA = (1<<ADEN) | (0<<ADPS2) | (1<<ADPS1) | (0<<ADPS0);

	// Set mux to ADC2 with adjusting result to left
	ADMUX = (1<<REFS0) | (0<<ADLAR) | (0<<MUX3) | (0<<MUX2) | (1<<MUX1) | (0<<MUX0);

	ADCSRA |= (1<<ADIE);	// включим прерывания
	ADCSRA |= (1<<ADATE);	// запуск преобразования по триггеру
	ADCSRB |= (1<<ADTS2);	// настройка триггера на переполнение таймера 0

	spfast.state=0;
	spfast.fht_count = 0;
	splow.state=0;
	splow.fht_count = 0;
}


ISR (TIMER0_OVF_vect)
{
	TCNT0 = 100;

	//ADCSRA |= 1<<ADSC;	//запуск преобразования
	// не требуется настроен автозапуск по этому прерыванию
};

ISR (ADC_vect)
{
	if (++adc_timer<LOW_COUNT)
	{
		if (spfast.state) return;
	}
	else adc_timer = 0;


	uint8_t m = ADCL;
	uint8_t j = ADCH;
	uint16_t k = (j << 8) | m;
	k -= 0x0200;

	if (!adc_timer)
	{
		if (splow.fht_count<FHT_N)
		{
			//splow.fht_input[splow.fht_count++] = k;
			fht_input_low[splow.fht_count++] = k;
		}
		else
		{
			splow.state=WAIT_FHT;
			spfast.fht_count=0;
			splow.fht_count=0;
			ADCSRA &= ~(1<<ADATE);
		}
	}
	if (!spfast.state)
	{
		if (spfast.fht_count<FHT_N )
		{
			//spfast.fht_input[spfast.fht_count++] = k;
			fht_input[spfast.fht_count++] = k;
		}
		else
		{
			spfast.fht_count =0;
			spfast.state=WAIT_FHT;
			//ADCSRA &= ~(1<<ADATE);
		}
	}

};

void getSpData(volatile spectrum *sp)
{
	uint16_t i;
	if (sp->state==WAIT_FHT)
	{
		if (splow.state==WAIT_FHT)
		{
			for (i = 0 ; i < FHT_N ; i++)
			{
				fht_input[i] = fht_input_low[i];
			}
		}
		
		//getValues();
		fht_window(); // window the data for better frequency response
		fht_reorder(); // reorder the data before doing the fht
		fht_run(); // process the data in the fht
		fht_mag_log(); // take the output of the fht
		sp->state = WAIT_RUN;
		for (i = 0; i < N_DB ; i++) 
		{
			if (fht_log_out[i]<40)fht_log_out[i]=0;
			fht_log_out[i]/=6;
			sp->fht_out[i] = fht_log_out[i];
			//if(i<3) fht_log_out[i]>>=1;
			//(fht_log_out[i] > fallSpeed) ? (fht_log_out[i] -= fallSpeed) : (fht_log_out[i] = 0);
		}
	}
}