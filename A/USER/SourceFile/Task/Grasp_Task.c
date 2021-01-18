#include "Grasp_Task.h"
#include "FreeRTOS.h"
#include "task.h"

Grasp_t Grasp;


/*Grasp_Task初始化*/
static void Grasp_Init(void)
{
	/*函数映射*/
	Grasp.Motor_Init					= Grasp_Motor_Init;
	Grasp.Auto_Grasp          = Auto_Ctrl;
	Grasp.Grasp_Reset         = ResetGrasp;
	Grasp.Grasp_Poweroff      = Poweroff_Ctrl;
	Grasp.Grasp_Fsm_Init      = Chassis_FSM_Init;
	Grasp.Bullet_Supply       = bulletSupply;
	Grasp.RC_Ctrl             = RC_Ctrl;
	Grasp.Send_Crtl           = Send_Crtl_To_Board; 
	/*数据初始化*/
	Grasp.Motor_Init(&Grasp.Gr);	//夹取电机初始化
	Grasp.Fsm                 = Return_Chassis_Fsm();
	Grasp.Grasp_Fsm_Init();
}

void Grasp_Task(void *pvParameters)
{
	vTaskDelay(500);
	Grasp_Init();
	while(1)
	{
	  FSM_Deal(Grasp.Fsm,Grasp.Can2_RC->Can_RC.s1,Grasp.Can2_RC->Can_RC.s2);
		vTaskDelay(2);
	}
}
