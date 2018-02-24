/*
 * MAX72xxMatrix.h
 *
 * Created: 06.02.2018 20:21:58
 *  Author: AnToI
 */ 


#ifndef MAX72XXMATRIX_H_
#define MAX72XXMATRIX_H_

#include <inttypes.h>

#define true					1
#define false					0
#define uc						unsigned char

#define displays				4
//#define maxDev					4
#define OP_NOOP					0
#define OP_DIGIT0				1
#define OP_DIGIT1				2
#define OP_DIGIT2				3
#define OP_DIGIT3				4
#define OP_DIGIT4				5
#define OP_DIGIT5				6
#define OP_DIGIT6				7
#define OP_DIGIT7				8
#define OP_DISPLAYTEST			0x0F
#define OP_SCANLIMIT			0x0B
#define OP_DECODEMODE			0x09
#define OP_SHUTDOWN				0x0C
#define OP_INTENSITY			0x0A

#define buff_size		8
//unsigned char spiData [displays*2];
unsigned char bitmap [displays*8];
uint8_t bitmapbuff [displays*8][buff_size];

void Matrix_init ();
void fillScreen ();
void spiTransfer(unsigned char opcode, uc data);
void matrix_write();
void shutdown(uc b);
void setIntensity(uc intensity);

#endif /* MAX72XXMATRIX_H_ */