#ifndef __INCLUDE_H_
#define __INCLUDE_H_
#include "STC15.H"
#include "CONFIG.H"

#define	u8	unsigned char
#define u16	unsigned int

#define NG_MARK		0
#define OK_MARK		1

#define RSE_MARK	0
#define SET_MARK	1

typedef enum
{
	ERROR = 0	,
	SUCCESS		,
}RETURN_STATUS ;

typedef enum 
{
	NORMAL ,
	AUTO_ACCELERATE ,
	STILL_NORMAL ,
	POWER_DOWN ,
	STILL_POWER_DOWN ,
	BUTTON_ACT ,
	STILL_BUTTON_ACT
}SYS_STATUS_TYPE;


#endif																										//__INCLUDE_H_
