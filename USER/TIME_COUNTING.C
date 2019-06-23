#include "TIME_COUNTING.H"
#include "MOTO_DRIVER.H"

#define T0_reload_value		( 0x10000 - CLOCK_DIVISION )
#define	T0H_reload_value	( T0_reload_value >> 8 )
#define T0L_reload_value	( T0_reload_value & 0x00FF )

u8 ONE_PULSE_DELAY , ACCELERATION_DELAY;
bit SWTITCH_SCAN_TIMESUP_FLAG ;
u8	MA_DIF ;																			//宏观计数差 记录微观计数的进位
u16 MI_DIF ;																			//微观计数差 记录总计数的低14位
u8  TIME_COUNT ;

void TIME_STEP( DIR_TYPE dir )
{ 
	if( dir != NEG_DIR )
	{
		MI_DIF++ ;
		if(( MI_DIF & MI_DIF_MASK ) == 0 )
		{
			MA_DIF++ ;
			if( HALF_DAY_MA_DIF <= MA_DIF )
				MA_DIF -= HALF_DAY_MA_DIF ;
		}
	}
	else
	{
		if(( MI_DIF & MI_DIF_MASK ) == 0 )
		{
			if( MA_DIF == 0 )
				MA_DIF = HALF_DAY_MA_DIF ;
			MA_DIF-- ;
		}
		MI_DIF-- ;
	}
}

void TIME_SYNC( DIR_TYPE dir , u16 num )
{
	do
	{
		TIME_STEP( POS_DIR ) ;
	}while( --TIME_COUNT ) ;
	do
	{
		TIME_STEP( dir ) ;
	}while( --num ) ;
}


void T0_DIVIER( void ) interrupt 1
{
	ACCELERATION_DELAY++ ;
	ONE_PULSE_DELAY++ ;
	if( ONE_PULSE_PERIOD <= ONE_PULSE_DELAY )
	{
		ONE_PULSE_DELAY = 0 ;
		SWTITCH_SCAN_TIMESUP_FLAG = SET_MARK ;
		TIME_COUNT++ ;
	}
}

void TIME_COUNT_INIT(void)
{
	P3M1 |= 0x10;							//设置时钟输入端口为高阻输入模式
	AUXR |= 0x80;							//定时器时钟1T模式
	TMOD = 0x04;							//设置T0为外部脉冲计数模式
	TL0 = T0L_reload_value;		//设置定时初值
	TH0 = T0H_reload_value;		//设置定时初值
	TR0 = SET_MARK;						//定时器0开始计时
	ET0 = SET_MARK;
	EA	= SET_MARK;
}

