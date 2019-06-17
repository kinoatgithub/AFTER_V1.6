#include "TIME_COUNTING.H"
#include "MOTO_DRIVER.H"

#define T0_reload_value		( 0x10000 - CLOCK_DIVISION )
#define	T0H_reload_value	( T0_reload_value >> 8 )
#define T0L_reload_value	( T0_reload_value & 0x00FF )

u16 TIME_INC ;
bit SWTITCH_SCAN_TIMESUP_FLAG ;
u16 ONE_PULSE_DELAY , ACCELERATION_DELAY ;
TIME_TYPE SV , PV ;

void T0_DIVIER( void ) interrupt 1
{
	ACCELERATION_DELAY++ ;
	ONE_PULSE_DELAY++ ;
	if( ONE_PULSE_PERIOD <= ONE_PULSE_DELAY )
	{
		TIME_INC++ ;
		ONE_PULSE_DELAY = 0 ;
		SWTITCH_SCAN_TIMESUP_FLAG = SET_MARK ;
	}
}

void TIME_COUNT_INIT(void)
{
	P3M1 |= 0x10;				//设置时钟输入端口为高阻输入模式
	AUXR |= 0x80;				//定时器时钟1T模式
	TMOD = 0x04;				//设置T0为外部脉冲计数模式
	TL0 = T0L_reload_value;		//设置定时初值
	TH0 = T0H_reload_value;		//设置定时初值
	TR0 = SET_MARK;				//定时器0开始计时
	ET0 = SET_MARK;
	EA	= SET_MARK;
}

void SOFT_DELAY( u16 times )
{
	u16 temp ;
	while( times-- )
	{
		temp = SOFT_DELAY_UNIT; 
		while( temp-- );
	}
}
