#ifndef ADC_H
#define ADC_H

#include <inttypes.h>
#include <avr/pgmspace.h>
//#include "pins.h"

#define swap(x) (__builtin_avr_swap(x))		/*  Swaps nibbles in byte */

#define DC_CORR				128/* Raw value from ADC when no signal */

#define N_DB				32
#define SP_N				16

#define LOG_OUT 1 // use the log output function
#define FHT_N 256 // set to 256 point fht

typedef enum 
{
	READ_ADC=0,
	WAIT_FHT,
	WAIT_RUN,
}e_fht_state;

int fht_input_low[FHT_N];
//int fht_input_fast[FHT_N];

typedef struct 
{
	//int fht_input[FHT_N];
	uint16_t fht_count;
	uint8_t spadc;
	e_fht_state state;
	uint8_t fht_out[N_DB];
}spectrum;

volatile uint16_t adc_timer;
volatile spectrum splow,spfast;

#define LOW_COUNT			16


typedef struct 
{
	spectrum *sp;
	uint8_t s1;
	uint8_t s2;
}setremap_s;

//setremap_s setremaplow,setremapfast;

volatile static const setremap_s setremap[SP_N] /*PROGMEM */= 
{
	{&splow,3,3},
	{&splow,4,6},
	{&splow,7,9},
	{&splow,10,14},
	{&splow,15,17},
	{&splow,18,22},
	{&splow,23,25},
	{&splow,26,28},
	{&splow,29,31},
	{&spfast,4,6},
	{&spfast,7,10},
	{&spfast,11,15},
	{&spfast,16,20},
	{&spfast,21,24},
	{&spfast,25,28},
	{&spfast,29,31},
};

void adcInit();
void getSpData(volatile spectrum *sp);

void setDispBr(uint8_t br);

#endif /* ADC_H */
