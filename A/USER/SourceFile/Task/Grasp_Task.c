#include "Grasp_Task.h"
#include "FreeRTOS.h"
#include "task.h"



Grasp_t Grasp;

/*Grasp_Task初始化*/
static void Grasp_Init(void)
{
	/*函数映射*/
	Grasp.Motor_Init					= Grasp_Motor_Init;
	/*数据初始化*/
	Grasp.Motor_Init(&Grasp.Gr);	//夹取电机初始化
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
