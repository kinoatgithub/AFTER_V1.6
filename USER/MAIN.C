#include "INCLUDE.H"
#include "MOTO_DRIVER.H"
#include "SWITCH.H"
#include "TIME_COUNTING.H"

extern u16 TIME_INC ;
extern TIME_TYPE SV , PV ;

void USER_SYS_INIT( void )
{
	SWITCH_PIN_INIT() ;
	MOTO_DRIVER_INIT() ;
}

void get_time( ROLL_DIR dir , u16 num )
{
	u16 temp = TIME_INC ;
  SV.IT += temp ;
  TIME_INC -= temp ;
  temp = ( SV.IT >> TIME_SHIFT ) ;
	SV.IT -= ( temp << TIME_SHIFT ) ;
	SV.AT += temp ;
	if( HALF_DAY_MACRO_MARK <= SV.AT )
		SV.AT -= HALF_DAY_MACRO_MARK ;
	temp = ( PV.IT >> TIME_SHIFT ) ;
  if( num != RSE_MARK )
  {
    if( dir != NEG_DIR )								      			//正向旋转
    {
      PV.IT += num ;
      PV.AT += ( PV.IT >> TIME_SHIFT ) ;
      if( HALF_DAY_MACRO_MARK <= PV.AT )
        PV.AT -= HALF_DAY_MACRO_MARK ;
      PV.IT &= MICRO_LSB_ACT_MARK ; 
    }
    else																						//反向旋转
    {
      temp = num >> TIME_SHIFT ;
      if( PV.IT < ( num & MICRO_LSB_ACT_MARK ))
      {
        temp++ ;
        PV.IT = PV.IT + ( 0x01 << TIME_SHIFT ) ;
      }
      if( PV.AT < temp )
        PV.AT += HALF_DAY_MACRO_MARK ;
      PV.AT -= temp ;
      PV.IT -= num & MICRO_LSB_ACT_MARK ;
    }
  }
}

ROLL_DIR get_dir( void )
{
	if(( PV.AT == SV.AT ) && ( SV.IT < PV.IT ))
			return NEG_DIR ;																			//捷径方向为反方向。
	if(( PV.AT + SIX_HOUR_MACRO_MARK ) < SV.AT )
		return NEG_DIR ;																				//捷径方向为反方向。
	if(( SV.AT < PV.AT ) && ( PV.AT < ( SV.AT + SIX_HOUR_MACRO_MARK )))
		return NEG_DIR ;																				//捷径方向为反方向。
	return POS_DIR ;																					//捷径方向为正方向。
}
	


void main( void )
{
	USER_SYS_INIT();
	while( 1 )
	{
		static	SYS_STATUS_TYPE 
				last_switch_status ,
				now_switch_status ,
				sys_handle_status ;
		static	ROLL_DIR moto_roll_dir ;
		now_switch_status = SCAN_SWITCH() ;
		if( now_switch_status != last_switch_status )
		{
			last_switch_status = now_switch_status ;
			sys_handle_status = now_switch_status ;
		}
		switch ( sys_handle_status )
		{
			case BUTTON_ACT:
			{
				MOTO_PHASE_INIT() ;
				sys_handle_status = STILL_BUTTON_ACT ;
			}
			case STILL_BUTTON_ACT:
			{
				ANGULAR_ACCELERATING( POS_DIR ) ;
				get_time( POS_DIR , 0 ) ;
				PV.AT = SV.AT ;
				PV.IT = SV.IT ;
			}
			break ;
			case POWER_DOWN:
			{
				sys_handle_status = STILL_POWER_DOWN ;
			}
			case STILL_POWER_DOWN:
			{
				PCON |= 0x01 ;																						//CPU进入空闲模式，任何中断都会唤醒CPU。
				get_time( moto_roll_dir , 0 ) ;
			}
			break ;
			case NORMAL:
			{
				get_time( POS_DIR , 0 ) ;
				if( SV.AT != PV.AT )
				{
					MOTO_PHASE_INIT() ;
					moto_roll_dir = get_dir() ;
					if( moto_roll_dir == NEG_DIR )
						get_time( POS_DIR , CCW_HYSTERESIS_VALUE ) ;	//逆时针转时多转一些以覆盖时针的回滞。
					sys_handle_status = AUTO_ACCELERATE ;
				}
				else
					sys_handle_status = STILL_NORMAL ;
			}
			break ;
			case AUTO_ACCELERATE:
			{
				ANGULAR_ACCELERATING( moto_roll_dir ) ;
				get_time( moto_roll_dir , 1 ) ;
				if( moto_roll_dir != POS_DIR )
				{
					if( get_dir() != NEG_DIR )
					{
						get_time( NEG_DIR , CCW_HYSTERESIS_VALUE ) ;	//抵消回滞偏移
						sys_handle_status = NORMAL ;
					}
				}
				else
				{
					if( SV.AT <= PV.AT )
						if( SV.IT <= PV.IT )
							sys_handle_status = STILL_NORMAL ;
				}
			}
			break ;
			case STILL_NORMAL:
			{
				if( get_dir() != NEG_DIR )
				{
					ONE_PULSE_DRIVING( POS_DIR );
					get_time( POS_DIR , 2 ) ;
				}
			}
			break ;
			default:
				break ;
		}
	}
}
		