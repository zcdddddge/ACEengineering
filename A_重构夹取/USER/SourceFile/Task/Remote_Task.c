#include "Remote_Task.h"
#include "FreeRTOS.h"
#include "task.h"
#include "Grasp_Task.h"

extern Grasp_t Grasp;
static u8 keyBoardLock =1 ; 

void Remote_Task(void *pvParameters)
{
	vTaskDelay(500);
	
	Grasp.Can2_RC = Return_Board_Communi();
	while(1)
	{
		flip2(&Grasp.Gr.GraspMotor[4], 237.0 ,6, 1 );
		pid_Cala(&Grasp.Gr); 
		
		vTaskDelay(2);
	}			
}

