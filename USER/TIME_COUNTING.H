#ifndef __TIME_COUNTING_H_
#define __TIME_COUNTING_H_

#include "INCLUDE.H"

typedef struct
{
	u16 AT ;																						//MACRO_TIME 宏观计数，保留计数的高（16-TIME_SHITF）位元
	u8 IT ;																							//MICRO_TIME 微观计数，保留计数的低TIME_SHITF位元
}TIME_TYPE ;

void TIME_COUNT_INIT( void );
void SOFT_DELAY( u16 times );

#endif
