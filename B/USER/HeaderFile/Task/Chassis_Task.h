#ifndef __CHASSIS_TASK_H_
#define __CHASSIS_TASK_H_
#include "ChassisMotor.h"
#include "RemoteDeal.h"
#include "Chassis_Fsm.h"

typedef __packed struct
{
	/*���ݱ���*/
	C_t C;
	REMOTE_t *RC;
	FSM_t *Fsm;
	
	/*����ָ�����*/
	void (*Fsm_Init)(void);															//״̬����ʼ��
	void (*Wheel_Init)(C_t *C);													//���ӵ����ʼ��
	void (*Rescue_Init)(C_t *C);												//��Ԯ�����ʼ��
	void (*Indepen)(C_t*,float ,float ,float ,int16_t);	//���̸���
	void (*Wiggle)(C_t*,float ,float ,float);						//����ҡ��
	void (*Poweroff)(C_t*);															//���̶ϵ�
}Chassis_t;

void Chassis_Task(void *pvParameters);

#endif
