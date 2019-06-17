#include "MOTO_DRIVER.H"
#include "TIME_COUNTING.H"
#include "SWITCH.H"

#define POS_ROTATE( x ) ((( x ) >> 1 ) | (( x ) << 3 ))
#define NEG_ROTATE( x ) ((( x ) << 1 ) | (( x ) >> 3 ))

//void DEBUG_LED( void )
//{
//	P30 = ~P30;
//}

u8 moto_phase[2] ;
u8 freq ;
bit step ;
u8 next_delay_times ;
extern u16 ONE_PULSE_DELAY , ACCELERATION_DELAY ;

void set_phase( u8 phase )
{
	P3 &= ( P3 & ( phase | 0xF0 ) | 0x20 );
	P3 |= phase & 0x0F;
}

void MOTO_PHASE_INIT( void )
{
	extern u8 freq ;
	step = RSE_MARK ;
	freq = DELAY_FREQ_INITIAL_VALUE ;											//初始化初始转速
	next_delay_times = ANTI_JAGGIES / freq ;							//初始化初始转速
	moto_phase[ 1 ] = moto_phase[ 0 ] ;										//相位初始化
}

void MOTO_DRIVER_INIT( void )
{
	moto_phase[ 0 ] = 0x11;
	TIME_COUNT_INIT();
	set_phase( 0 );	
	P3M0 |= 0x0F;									//驱动达林顿管必须设置P3.0 ~ P3.3为推挽输出
}

void ONE_PULSE_DRIVING( ROLL_DIR dir )
{
	if( dir != NEG_DIR )
		moto_phase[ 0 ] = POS_ROTATE( moto_phase[ 0 ] );
	else
		moto_phase[ 0 ] = NEG_ROTATE( moto_phase[ 0 ] );
	set_phase( moto_phase[ 0 ] );
	SOFT_DELAY( ONE_PULSE_WIDTH );
	set_phase( 0 );
}

void ANGULAR_ACCELERATING( ROLL_DIR dir )
{
	if( dir != SET_MARK )
		moto_phase[ step ] = POS_ROTATE( moto_phase[ step ] );
	else
		moto_phase[ step ] = NEG_ROTATE( moto_phase[ step ] );
	set_phase( moto_phase[ 0 ] | moto_phase[ 1 ] );
	step = ~step ;
	SOFT_DELAY( next_delay_times );
	if( ACCELERAT_PERIOD < ACCELERATION_DELAY )
	{
		ACCELERATION_DELAY = 0 ;
		next_delay_times = ANTI_JAGGIES / freq ;
		if( ACCELERATION_MAX_FREQ < freq++ )
			freq = ACCELERATION_MAX_FREQ ;
	}
}






