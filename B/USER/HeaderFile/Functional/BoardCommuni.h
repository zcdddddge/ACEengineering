#ifndef __BOARDCOMMUNI_H_
#define __BOARDCOMMUNI_H_
#include "stm32f4xx.h"
#include "REMOTE_ISR.h"




/*板间通信初始化*/
void BoardCommuni_Init(void);

/*发送遥控数据*/
void Send_RC_To_Board(void);

/*发送控制指令至板子*/
void Send_Ctrl_To_Board(unsigned char boxs, unsigned char Magazine ,unsigned char Up_motor ,unsigned char Flex_motor ,unsigned char Flip_motor ,unsigned char Clamp_motor);

/*底盘接收夹取控制指令*/
void  BoardCommuni_DataUpdate(int16_t *speed) ;
#endif
