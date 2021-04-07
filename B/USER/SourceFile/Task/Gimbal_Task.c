#include "Gimbal_Task.h"
#include "FreeRTOS.h"
#include "task.h"

G_t  G ;


void Gimbal_Task(void *pvParameters)
{
	vTaskDelay(500);
	PY_Motor_Init(&G);
	while(1)
	{		

		//PY_Encoder_DRIVE(&G,100,100,100) ;
		//G->Can_Send_Gimbal(100,100,100,100);
		vTaskDelay(2);
	}
}
