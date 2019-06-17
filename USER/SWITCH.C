#include "SWITCH.H"
#include "TIME_COUNTING.H"

#define switch_pin_mask				5										//P3.5需要往左偏移5位
#define switch_pin_bit_set		( 0x01 << switch_pin_mask )
#define switch_pin_bit_rse		( ~ switch_pin_bit_set )

extern bit PUSH_BUTTON ;
extern bit SWTITCH_SCAN_TIMESUP_FLAG ;

sbit	switch_pin  =  P3^5 ;

void SWITCH_PIN_INIT( void )
{
	P3M0 &= switch_pin_bit_rse ;													 //按钮引脚设置为准双向或高阻态	
}

SYS_STATUS_TYPE SCAN_SWITCH( void )
{
	static	bit last_step ;
	static	bit floating_pin_status , pullup_pin_status ;
	static	SYS_STATUS_TYPE 
					last_button_status , 
					now_button_status ,
					temp_button_status ;
	if( SWTITCH_SCAN_TIMESUP_FLAG != RSE_MARK )
	{
		if( last_step != SET_MARK )
		{
			floating_pin_status = switch_pin ;
			P3M1 &= switch_pin_bit_rse ;											 //按钮引脚设置为准双向	
			if( floating_pin_status != SET_MARK )
			{
				if( pullup_pin_status != SET_MARK )
					now_button_status = BUTTON_ACT ;
				else
					now_button_status = POWER_DOWN ;
			}
			else
				now_button_status = NORMAL ;
		}
		else
		{
			pullup_pin_status = switch_pin ;
			P3M1 |= switch_pin_bit_set ;											//按钮引脚设置为高阻输入
		}
		last_step = ~last_step ;
		SWTITCH_SCAN_TIMESUP_FLAG = RSE_MARK ;
		if( last_button_status != now_button_status )
		{
			temp_button_status = last_button_status ;
			last_button_status = now_button_status ;
			return temp_button_status ;
		}
	}
	return last_button_status ;
}
		


	
	





