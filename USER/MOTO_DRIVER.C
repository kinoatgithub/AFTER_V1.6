#include "MOTO_DRIVER.H"
#include "TIME_COUNTING.H"
#include "SWITCH.H"

DIR_TYPE COUNT_DIR ;
u8 phase_step ;
u8 freq ;
static u16 next_delay_times;
extern u8 ONE_PULSE_DELAY , ACCELERATION_DELAY;
u8 phase_mark[ 8 ] ;

//void DEBUG_LED( void )
//{
//	P30 = ~P30 ;
//}

void set_phase( u8 phase )
{
	u8 temp = P3 ;
	temp = ( temp & 0xF0 ) | phase ;
	P3 = temp ;
}

void rotate( DIR_TYPE dir )
{
	if( dir != NEG_DIR )
		phase_step++ ;
	else
		phase_step-- ;
	set_phase( phase_mark[ phase_step ] )	;
	TIME_SYNC( !dir , 1 ) ;
}

void MOTO_DRIVER_INIT( void )
{
	set_phase( 0 );	
	phase_mark[ 0 ] = 0x01 ;
	phase_mark[ 1 ] = 0x03 ;
	phase_mark[ 2 ] = 0x02 ;
	phase_mark[ 3 ] = 0x06 ;
	phase_mark[ 4 ] = 0x04 ;
	phase_mark[ 5 ] = 0x0C ;
	phase_mark[ 6 ] = 0x08 ;
	phase_mark[ 7 ] = 0x09 ;
	P3M0 |= 0x0F;														//驱动达林顿管必须设置P3.0 ~ P3.3为推挽输出
}

void RESET_PHASE( void )
{
	phase_step = phase_step - ( phase_step % 2 ) ;
	freq = FREQ_INITIAL_VALUE ;
	next_delay_times = ONE_PULSE_WIDTH ;
}

void MOTO_DRIVING( DIR_TYPE dir )
{
	rotate( dir ) ;
	while( next_delay_times-- ) ;
	set_phase( 0 ) ;
	if( ACCELERAT_PERIOD < ACCELERATION_DELAY )
	{
		if( freq++ < ACCELERATION_MAX_FREQ )
			next_delay_times = ( ANTI_JAGGIES / freq ) * 100 ;
		ACCELERATION_DELAY = 0 ;
	}
}



