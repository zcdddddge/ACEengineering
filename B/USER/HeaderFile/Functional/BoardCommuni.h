#ifndef __BOARDCOMMUNI_H_
#define __BOARDCOMMUNI_H_
#include "stm32f4xx.h"
#include "REMOTE_ISR.h"

/*���ͨ�ų�ʼ��*/
void BoardCommuni_Init(void);

/*����ң������*/
void Send_RC_To_Board(void);

/*���Ϳ���ָ��������*/
void Send_Ctrl_To_Board(unsigned char boxs,unsigned char Magazine);
#endif
