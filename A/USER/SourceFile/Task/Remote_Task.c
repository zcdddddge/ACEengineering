#include "Remote_Task.h"
#include "FreeRTOS.h"
#include "task.h"

extern Test_t Text;
Test_t Text;
static u8 lock = 1;

/*测试时使用的任务*/
void Remote_Task(void *pvParameters)
{
	vTaskDelay(500);
	Grasp_Motor_Init(&Text.Gr);
	Text.Can2_RC = Return_Board_Communi();
#ifdef UART_RC
	Remote_Data_Init();
	Text.Remote = Return_RemoteDeal_Point();
#endif
	
	while(1)
	{
#ifdef UART_RC
		Remote_Data_Deal();
		if(Text.Remote->RC_ctrl->s1 == 1 && Text.Remote->RC_ctrl->s2 == 1)
		{
			if(lock == 1)
			{
				Auto_Ctrl(&Text.Gr,2);
				lock = 2;
			}
			else
			{
				Auto_Ctrl(&Text.Gr,0);
			}
		}
		else if(Text.Remote->RC_ctrl->s1 == 1 && Text.Remote->RC_ctrl->s2 == 3)
		{
			RC_Ctrl(&Text.Gr,Text.Remote->RC_ctrl);
			/*复位值初始化*/
			Text.Gr.reset[0] = Text.Gr.reset[1] = Text.Gr.reset[2] = Text.Gr.reset[3] \
				= Text.Gr.reset[4] = Text.Gr.reset[5] = Text.Gr.reset[6] = 0;
			
		}
		else if(Text.Remote->RC_ctrl->s1 == 1 && Text.Remote->RC_ctrl->s2 == 2)
		{
			ResetGrasp(&Text.Gr);
		}
		else
		{
			lock = 1;
			Poweroff_Ctrl(&Text.Gr);
		}
#endif
	if(Text.Can2_RC->Can_RC.s1 == 3 && Text.Can2_RC->Can_RC.s2==3) 
		{
			if(lock == 1)
			{
				Auto_Ctrl(&Text.Gr,1);
				lock = 2;
			}
			else
			{
				Auto_Ctrl(&Text.Gr,0);
			}
		}
		else if(Text.Can2_RC->Can_RC.s1 == 3 && Text.Can2_RC->Can_RC.s2 == 2)
		{
 			ResetGrasp(&Text.Gr);
		}
		else
		{
			lock = 1;
			Poweroff_Ctrl(&Text.Gr);
		}
		vTaskDelay(2);
	}
}
