#include "MOTO_DRIVER.H"
#include "TIME_COUNTING.H"
#include "SWITCH.H"

#define POS_ROTATE( x ) ((( x ) >> 1 ) | (( x ) << 3 ))
#define NEG_ROTATE( x ) ((( x ) << 1 ) | (( x ) >> 3 ))

u16 GTR;
static u8 next_delay_times;
static u8 moto_phase[2];
extern DIRECTION_TYPE PUBLIC_DIRECTION;
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

void accelerating_driver( DIRECTION_TYPE direction )
{
	static bit step;
	if( direction == POS )
	{
		moto_phase[ step ] = POS_ROTATE( moto_phase[ step ] );
	}
	else
	{
		moto_phase[ step ] = NEG_ROTATE( moto_phase[ step ] );
	}
	step = ~step;
	SET_PHASE( moto_phase[ 0 ] | moto_phase[ 1 ] );
	SOFT_DELAY( next_delay_times );
}

void ANGULAR_ACCELERATING_CHECK( void )
{
	static u8 freq;
	static DIRECTION_TYPE last_direction;
	SCAN_SWITCH();
	while( PUBLIC_DIRECTION != NON )
	{
		SCAN_SWITCH();
		if( last_direction != PUBLIC_DIRECTION )
		{
			moto_phase[ 1 ] = moto_phase[ 0 ];
			freq = DELAY_FREQ_INITIAL_VALUE;
			last_direction = PUBLIC_DIRECTION;
		}
		accelerating_driver( PUBLIC_DIRECTION );
		if( ACCELERAT_PERIOD < ACCELERATION_DELAY )
		{
			ACCELERATION_DELAY = 0;
			next_delay_times = ANTI_JAGGIES / freq;
			if( ACCELERATION_MAX_FREQ < freq++ )
				freq = ACCELERATION_MAX_FREQ;
		}
	}
}










//u16 hard_delay( u16 freq )
//{
//	static u16 j;
//	u16 i , return_freq = freq;
//	u16 t = ANTI_JAGGIES / freq;
//	for( i = 0 ; i < t ; i++ )
//	{
//		if( j < ANGULAR_ACCELERATION )
//		{
//			j++;
//		}
//		else
//		{
//			j = 0;
//			return_freq++;
//			if( FREQ_LIMIT < return_freq )
//				return_freq = FREQ_LIMIT;
//		}
//	}
//	return return_freq;
//}

//void MOTO_CONTROL( void )
//{
//	static bit step;
//	static u16 next_freq;
//	DIRECTION_TYPE now_direction;
//	static DIRECTION_TYPE last_direction;
//	if( TF0 != RSE_MARK )
//	{
//		now_direction = SCAN_SWITCH();
//		if( last_direction != now_direction )
//		{
//			moto_phase[ 1 ] = moto_phase[ 0 ];
//			next_freq = DELAY_INITIAL_VALUE;
//			last_direction = now_direction;
//		}
//		TL0 = 0x00;				//设置定时初值
//		TH0 = 0x00;				//设置定时初值
//		TF0 = RSE_MARK;			//清除TF0标志														
//	}
//	if( last_direction != NON )
//	{
//		if( last_direction == POS )
//		{
//			moto_phase[ step ] = POS_ROTATE( moto_phase[ step ] );
//		}
//		else
//		{
//			moto_phase[ step ] = NEG_ROTATE( moto_phase[ step ] );
//		}
//		step = ~step;
//		SET_PHASE( moto_phase[ 0 ] | moto_phase[ 1 ] );
//		next_freq = hard_delay( next_freq );
//	}
//	else
//	{
//		if( TIMER2_INTERRUPT_FLAG == SET_MARK )
//		{
//			SET_PHASE( moto_phase[ 0 ] );
//			moto_phase[ 0 ] = POS_ROTATE( moto_phase[ 0 ] );
//			TIMER2_INTERRUPT_FLAG = RSE_MARK;
//			hard_delay( ONE_PULSE_FREQ );
//			SET_PHASE( 0 );
//			if( ( moto_phase[ 0 ] & 0x10 ) == SET_MARK )
//			{
//				moto_phase[ 0 ] = 0x11;
//			}
//		}
//	}	
//}




