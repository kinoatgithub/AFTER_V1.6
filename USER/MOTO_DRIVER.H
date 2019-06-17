#ifndef __MOTO_DRIVER_H_
#define __MOTO_DRIVER_H_

#include "INCLUDE.H"

typedef enum
{
	POS_DIR ,
	NEG_DIR
}ROLL_DIR;

void MOTO_DRIVER_INIT( void ) ;
void MOTO_PHASE_INIT( void ) ;
void ONE_PULSE_DRIVING( ROLL_DIR dir ) ;
void ANGULAR_ACCELERATING( ROLL_DIR dir ) ;
//void DEBUG_LED( void );

#endif																	//__MOTO_DRIVER_H_
