#ifndef __CONFIG_H_
#define __CONFIG_H_

#define CLOCK_DIVISION							576								//中断时钟分频
#define ONE_PULSE_PERIOD						50								//两次正常走时之间的时间间隔
#define SWITCH_SCAN_PERIOD					2									//两次按键扫描之间的时间间隔
#define SWITCH_DISCHARGE_PERIOD			5									//设置按键扫描时电容放电持续时间，定性，多减少加。
#define	SWITCH_CHARGE_PERIOD				10								//设置按键扫描时电容充电持续时间，定性，多减少加。
#define ACCELERAT_PERIOD						3									//两次频率调整间的时间间隔
#define SOFT_DELAY_UNIT							100								//最小软延时单元
#define ONE_PULSE_WIDTH							200								//单脉冲走时模式，脉冲持续时间,定性，多减少加。
#define DELAY_FREQ_INITIAL_VALUE		1									//刚加速旋转时的频率
#define ANTI_JAGGIES								255								//角加速抗锯齿化，提高分辨率						
#define ACCELERATION_MAX_FREQ				50								//最大旋转速度限制
#define PULSE_PER_HOUR							( 64 * 64 * 4 )		//分针以8拍转动一圈所需脉冲数
#define TIME_SHIFT									2									//12小时的脉冲数总共占用18位元，只取高16位作为宏观时间。
#define CCW_HYSTERESIS_VALUE				1365							//~不能大于65532，否则时间溢出			
//#define ROLL_SAFE_SIZE						100								//增减脉冲数时防止溢出的安全回滞量，

#define SIX_HOUR_MACRO_MARK							(( PULSE_PER_HOUR >> TIME_SHIFT ) * 6 )				//~等价于6小时的宏观计数
#define HALF_DAY_MACRO_MARK							( SIX_HOUR_MACRO_MARK * 2 )										//~等价于12小时的宏观计数
#define MICRO_LSB_ACT_MARK							(( 0x01 << TIME_SHIFT ) - 1 )									//~微观计数有效位掩码

#endif
