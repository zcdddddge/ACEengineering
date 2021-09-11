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
					/* 翻转电机 */
//							flip_motor(&Gr.GraspMotor[4], 90, 10, 1);//一定要看准数据正确才能开
				        vTaskDelay(2);
			}
}











