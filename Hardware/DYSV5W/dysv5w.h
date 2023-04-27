#ifndef __DYSV5W_H	
#define __DYSV5W_H	 
#include "sys.h"

//该模块支持多种通信方式，本设计使用串口控制的方式，因此只需要初始化串口波特率为9600即可
//该模块在向flash传输音频文件时，哪怕插上串口也是无法进行语音播报的，要拔下数据线才能进行语音播报相关的调试

void flash_play(void);
void max_volume(void);
int pleasr_study(void);
int please_steady(void);


#endif
