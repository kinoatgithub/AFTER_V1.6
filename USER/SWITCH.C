#include "SWITCH.H"
#include "TIME_COUNTING.H"

extern u16 GTR;
extern bit PUSH_BUTTON;

sbit	switch_pin  =  P3^5;


void SWITCH_INIT( void )
{
	P3M0 |= 0x20 ;																	//P3.5设置为开漏输出
	P3M1 |= 0x20 ;
}

bit SCAN_SWITCH( void )
{
	static bit last_switch_status;
	if( switch_pin == last_switch_status )
	{
		if( switch_pin == 0 )
		{
			last_switch_status = SET_MARK;
		}
		else
		{
			last_switch_status = RSE_MARK;
		}
	}
	return last_switch_status;
}




