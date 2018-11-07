#include "SWITCH.H"
#include "TIME_COUNTING.H"

extern u16 GTR;
extern bit PUSH_BUTTON;

sbit	switch_pin  =  P3^5;


void SWITCH_INIT( void )
{
	P3M0 &= ~0x20;																		//P3.5按钮设置为准双向弱上拉；
	P3M1 &= ~0x20;
}

bit SCAN_SWITCH( void )
{
	if( switch_pin == 0 )
	{
		return SET_MARK;
	}
	else
	{
		return RSE_MARK;
	}
}




