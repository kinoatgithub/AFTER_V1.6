#include "INCLUDE.H"
#include "MOTO_DRIVER.H"
#include "TIME_COUNTING.H"
#include "SWITCH.H"

DIR_TYPE now_dir ;
bit hys_mark ;																								//计时寄存器注入过回滞量的标志
SYS_STATUS_TYPE SYS_STATUS ;
extern u8 MA_DIF ;
extern u16 MI_DIF ;

void USER_SYS_INIT( void )
{
	MOTO_DRIVER_INIT();
	TIME_COUNT_INIT();
	SWITCH_PIN_INIT();
}

void main( void )
{
	USER_SYS_INIT();
	while( 1 )
	{
		SYS_STATUS = SCAN_SWITCH( SYS_STATUS ) ;
		switch ( SYS_STATUS )
		{
			case NORMAL:	
				CLK_DIV = 0x00 ;														//CPU主时钟频率/1
				if( SIX_HOUR_MA_DIF <= MA_DIF )
					now_dir = NEG_DIR ;
				else
					now_dir = POS_DIR ;			
				if(( hys_mark != RSE_MARK ) | ( now_dir != POS_DIR ))
				{
					TIME_SYNC( now_dir , NEG_DIR_HYS ) ;
					hys_mark = ~hys_mark ;
				}
				RESET_PHASE() ;
				SYS_STATUS = AUTO_ACCELERATE ;
				break ;
			case BUTTON_ACT:
				MA_DIF = 0 ;
				MI_DIF = 3 ;
			case AUTO_ACCELERATE:
				if( 2 < (( MA_DIF << 2 ) | ( MI_DIF & MI_DIF_MASK )))				//是否计数差值大于2
				{
					MOTO_DRIVING( now_dir ) ;
				}
				else
				{
					if( now_dir != NEG_DIR )
						SYS_STATUS = STILL_NORMAL ;
					else
						SYS_STATUS = NORMAL ;
				}
				break ;
			case STILL_NORMAL:
				if(( MI_DIF & MI_DIF_MASK ) != 0 )
				{
					MOTO_DRIVING( POS_DIR ) ;
					RESET_PHASE() ;
				}
				break ;
			case POWER_DOWN:
				TIME_SYNC( POS_DIR , 0 ) ;
				CLK_DIV = 0x07 ;														//CPU主时钟频率/128
				break ;				
		}
	}
}
		