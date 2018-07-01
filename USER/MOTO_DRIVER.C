#include "MOTO_DRIVER.H"
#include "TIME_COUNTING.H"
#include "SWITCH.H"

#define POS_ROTATE( x ) ((( x ) >> 1 ) | (( x ) << 3 ))
#define NEG_ROTATE( x ) ((( x ) << 1 ) | (( x ) >> 3 ))

u16 GTR;
bit PUSH_BUTTON;
static u8 next_delay_times;
static u8 moto_phase[2];
extern bit ONE_PULSE_TIMESUP_FLAG;
extern u16 ONE_PULSE_DELAY , ACCELERATION_DELAY;

void DEBUG_LED( void )
{
	P30 = ~P30;
}

void SET_PHASE( u8 phase )
{
	P3 &= ( P3 & ( phase | 0xF0 ) | 0x20 );
	P3 |= phase & 0x0F;
}

void MOTO_DRIVER_INIT( void )
{
	moto_phase[ 0 ] = 0x11;
	TIME_COUNT_INIT();
	SWITCH_INIT();
	SET_PHASE( 0 );	
	P3M0 |= 0x0F;									//驱动达林顿管必须设置P3.0 ~ P3.3为推挽输出
}

void ONE_PULSE_DRIVING_CHECK( void )
{
	if( ONE_PULSE_TIMESUP_FLAG )
	{
		ONE_PULSE_TIMESUP_FLAG = RSE_MARK;
		moto_phase[ 0 ] = POS_ROTATE( moto_phase[ 0 ] );
		SET_PHASE( moto_phase[ 0 ] );
		SOFT_DELAY( ONE_PULSE_WIDTH );
		SET_PHASE( 0 );
	}
}

void accelerating_driver( void )
{
	static bit step;
	step = ~step;
	moto_phase[ step ] = POS_ROTATE( moto_phase[ step ] );
	SET_PHASE( moto_phase[ 0 ] | moto_phase[ 1 ] );
	SOFT_DELAY( next_delay_times );
}

void ANGULAR_ACCELERATING_CHECK( void )
{
	static u8 freq;
	freq = DELAY_FREQ_INITIAL_VALUE;
	while( SCAN_SWITCH() == SET_MARK )
	{
		moto_phase[ 1 ] = moto_phase[ 0 ];
		accelerating_driver(  );
		if( ACCELERAT_PERIOD < ACCELERATION_DELAY )
		{
			ACCELERATION_DELAY = 0;
			next_delay_times = ANTI_JAGGIES / freq;
			if( ACCELERATION_MAX_FREQ < freq++ )
				freq = ACCELERATION_MAX_FREQ;
		}
	}
}





