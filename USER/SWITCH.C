#include "SWITCH.H"
#include "TIME_COUNTING.H"

#define direction_group_hysteresis_num ( 0x01 << DIRECTION_HYSTERESIS_BIT_SIZE )
#define direction_group_hysteresis_mark ( direction_group_hysteresis_num - 1 )

extern u16 GTR;
extern u16 SWITCH_SCAN_DELAY;
DIRECTION_TYPE PUBLIC_DIRECTION;
sbit	switch_pin  =  P3^5;


void SWITCH_INIT( void )
{
	P3M0 |= 0x20 ;																	//P3.5设置为开漏输出
	P3M1 |= 0x20 ;
}

void SCAN_SWITCH( void )
{
	static u8 hysteresis_group_ptr;
	bit befor_pull , after_pull;
	DIRECTION_TYPE hysteresis_group[ direction_group_hysteresis_num ];
	
	if( SWITCH_SCAN_DELAY < SWITCH_SCAN_PERIOD )
	{
		return;
	}
	SWITCH_SCAN_DELAY = 0;
	befor_pull = switch_pin;
	switch_pin = RSE_MARK;
	GTR = SWITCH_DISCHARGE_PERIOD;
	while( GTR-- );
	switch_pin = SET_MARK;
	GTR = SWITCH_CHARGE_PERIOD;
	while( GTR-- );
	after_pull = switch_pin;
	hysteresis_group_ptr++;
	hysteresis_group_ptr &= ( direction_group_hysteresis_mark );
	if( after_pull != SET_MARK )
	{
		if( befor_pull != SET_MARK )												//检测到没串电容的按钮接通
		{
			hysteresis_group[ hysteresis_group_ptr ] = POS;
		}
		else
		{
			hysteresis_group[ hysteresis_group_ptr ] = NEG;							//检测到有串电容的按钮接通
		}
	}
	else
	{
		hysteresis_group[ hysteresis_group_ptr ] = NON;
	}
	for( GTR = 0 ; GTR < direction_group_hysteresis_mark ; GTR++ )
	{
		if( hysteresis_group[ GTR ] != hysteresis_group[ GTR + 1 ] )
		{
			return;
		}
	}
	PUBLIC_DIRECTION = hysteresis_group[ 0 ];
}




