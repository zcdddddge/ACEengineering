#ifndef __CAN2_ISR_H_
#define __CAN2_ISR_H_
#include "stm32f4xx.h"


/*CAN2���ͺ���������Ϊ���ͱ�ʶ��Ϊstdid������CAN_DATA[8]*/
void CAN2_To_Board(u8*CAN_DATA,int16_t stdid);



/*����can2���յ��ģ��ⲿ���ӵ���ֵ������ȡ���ذ��״ֵ̬
	����˵������*/
u8 *Return_CAN2_Board_Data(void);



#endif
