#include "INCLUDE.H"
#include "MOTO_DRIVER.H"
#include "SWITCH.H"

void USER_SYS_INIT( void )
{
	MOTO_DRIVER_INIT();
}

void main( void )
{
	USER_SYS_INIT();
	while( 1 )
	{
		ONE_PULSE_DRIVING_CHECK();
		ANGULAR_ACCELERATING_CHECK();
	}
}
		