#ifndef __CONFIG_H_
#define __CONFIG_H_

#define CLOCK_DIVISION						576				//中断时钟分频
#define ONE_PULSE_PERIOD					100				//两次正常走时之间的时间间隔
#define SWITCH_SCAN_PERIOD					2					//两次按键扫描之间的时间间隔
#define SWITCH_DISCHARGE_PERIOD				5					//设置按键扫描时电容放电持续时间，定性，多减少加。
#define	SWITCH_CHARGE_PERIOD				10					//设置按键扫描时电容充电持续时间，定性，多减少加。
#define ACCELERAT_PERIOD					3					//两次频率调整间的时间间隔
#define SOFT_DELAY_UNIT						100					//最小软延时单元
#define ONE_PULSE_WIDTH						200					//单脉冲走时模式，脉冲持续时间,定性，多减少加。
#define DIRECTION_HYSTERESIS_BIT_SIZE		1           	    //方向回滞状态存储位数，重复确认次数以减少误触发,~表示有2^~个历史记录;
#define DELAY_FREQ_INITIAL_VALUE			1					//刚加速旋转时的频率
#define ANTI_JAGGIES						255					//角加速抗锯齿化，提高分辨率						
#define ACCELERATION_MAX_FREQ				50					//最大旋转速度限制

#endif
