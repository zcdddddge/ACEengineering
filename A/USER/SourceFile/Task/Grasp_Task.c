#include "Grasp_Task.h"
#include "FreeRTOS.h"
#include "task.h"



Grasp_t Grasp;

/*Grasp_Task��ʼ��*/
static void Grasp_Init(void)
{
	/*����ӳ��*/
	Grasp.Motor_Init					= Grasp_Motor_Init;
	/*���ݳ�ʼ��*/
	Grasp.Motor_Init(&Grasp.Gr);	//��ȡ�����ʼ��
}

void Grasp_Task(void *pvParameters)
{
	vTaskDelay(500);
	Grasp_Init();
	
	while(1)
	{
		vTaskDelay(2);
	}
}
