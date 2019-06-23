#ifndef __CONFIG_H_
#define __CONFIG_H_

#define CLOCK_DIVISION									144							//中断时钟分频
#define ONE_PULSE_PERIOD								100							//两次正常走时之间的时间间隔
#define ACCELERAT_PERIOD								50							//两次频率调整间的时间间隔
#define ONE_PULSE_WIDTH									5000							//单脉冲走时模式，脉冲持续时间,定性，多减少加。
#define FREQ_INITIAL_VALUE							1								//刚加速旋转时的频率
#define ANTI_JAGGIES										255							//角加速抗锯齿化，提高分辨率						
#define ACCELERATION_MAX_FREQ						50							//最大旋转速度限制
#define NEG_DIR_HYS											1024						//反向旋转回滞量

#define MI_BIT_SIZE				14														//微观计数的低~位
#define MI_DIF_SIZE				( 0x01 << 14 ) 								//微观计数总量
#define MI_DIF_MASK				( MI_DIF_SIZE - 1 )						//微观有效位掩码
#define HALF_DAY_DIF			( 64 * 64 * 4 * 12 )					//半天总共包含~个计数
#define HALF_DAY_MA_DIF		( HALF_DAY_DIF >> 14 )				//半天的宏观计数
#define HALF_DAY_MI_DIF		( HALF_DAY_DIF & MI_DIF_MASK )
#define SIX_HOUR_MA_DIF		( HALF_DAY_DIF << 2 )

#endif
