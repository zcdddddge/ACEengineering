#include "Flip_Task.h"
#include "FreeRTOS.h"
#include "task.h"

Gr_t Gr;


void Flip_Task(void *pvParameters)
{
    vTaskDelay(500);
//		Flip_Motor_Init(&Gr);
	
			while(1)
			{
					/* ��ת��� */
//							flip_motor(&Gr.GraspMotor[4], 90, 10, 1);//һ��Ҫ��׼������ȷ���ܿ�
				        vTaskDelay(2);
			}
}











