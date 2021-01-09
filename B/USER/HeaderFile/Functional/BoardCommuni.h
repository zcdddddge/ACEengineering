#ifndef __BOARDCOMMUNI_H_
#define __BOARDCOMMUNI_H_
#include "stm32f4xx.h"
#include "REMOTE_ISR.h"

/*板间通信初始化*/
void BoardCommuni_Init(void);

/*发送遥控数据*/
void Send_RC_To_Board(void);

/*发送控制指令至板子*/
void Send_Ctrl_To_Board(unsigned char boxs,unsigned char Magazine);
#endif
