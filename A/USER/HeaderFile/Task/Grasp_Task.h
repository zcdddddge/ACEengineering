#ifndef __GRASP_TASK_H_
#define __GRASP_TASK_H_
#include "GraspMotor.h"
#include "BoardCommuni.h"

typedef __packed struct
{
	/*���ݱ���*/
	Gr_t Gr;
	
	/*����ָ�����*/
	void (*Motor_Init)(Gr_t *Gr);	//��ȡ�����ʼ��
}Grasp_t;

void Grasp_Task(void *pvParameters);
	
#endif
