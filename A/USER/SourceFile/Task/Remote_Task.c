#include "Remote_Task.h"
#include "FreeRTOS.h"
#include "task.h"

extern Clip_t Clip;
Clip_t Clip;
static u8 lock = 1;
static u8 keyBoardLock =1 ; 

void Remote_Task(void *pvParameters)
{
	vTaskDelay(500);
	Grasp_Motor_Init(&Clip.Gr);
#ifdef CAN_RC
	Clip.Can2_RC = Return_Board_Communi();
#endif 
#ifdef UART_RC
	Remote_Data_Init();
	Clip.Remote = Return_RemoteDeal_Point();
#endif
	
	while(1)
	{
#ifdef UART_RC
		Remote_Data_Deal();
		if(Clip.Remote->RC_ctrl->s1 == 1 && Clip.Remote->RC_ctrl->s2 == 1){
			if(lock == 1){
				Auto_Ctrl(&Clip.Gr,2);
				lock = 2;
			}
			else{
				Auto_Ctrl(&Clip.Gr,0);
			}
		}
		else if(Clip.Remote->RC_ctrl->s1 == 1 && Clip.Remote->RC_ctrl->s2 == 3){
			RC_Ctrl(&Clip.Gr,Clip.Remote->RC_ctrl);
			/*¸´Î»Öµ³õÊ¼»¯*/
			Clip.Gr.reset[0] = Clip.Gr.reset[1] = Clip.Gr.reset[2] = Clip.Gr.reset[3] \
				= Clip.Gr.reset[4] = Clip.Gr.reset[5] = Clip.Gr.reset[6] = 0;	
		}
		else if(Clip.Remote->RC_ctrl->s1 == 1 && Clip.Remote->RC_ctrl->s2 == 2){
			ResetGrasp(&Clip.Gr);
		}
		else{
			lock = 1;
			Poweroff_Ctrl(&Clip.Gr);
		}
	}
#endif
		
#ifdef CAN_RC
    /*****************Ò£¿Ø¿ØÖÆ¼ÐÈ¡************************************/
		if(Clip.Can2_RC->Can_RC.s1 == 3 && Clip.Can2_RC->Can_RC.s2==3) {
			if(lock == 1){
				Auto_Ctrl(&Clip.Gr,1);
				lock = 2;
			}
			else{
				Auto_Ctrl(&Clip.Gr,0);
			}
		}
		else if(Clip.Can2_RC->Can_RC.s1 == 3 && Clip.Can2_RC->Can_RC.s2 == 2){
 			ResetGrasp(&Clip.Gr);
		}
		else{
			lock = 1;
			Poweroff_Ctrl(&Clip.Gr);
		}
    /****************¼üÅÌ¿ØÖÆµ¯²Ö**********************************/ 
		#if 0 
		if(Clip.Can2_RC->state.Magazine==2) {
				SupplyOpen;
		}else if(Clip.Can2_RC->state.Magazine==1){
				SupplyClose;
		}
		#endif 
		
		/**************¼üÅÌ¿ØÖÆ¼ÐÈ¡***********************************/ 
		if(Clip.Can2_RC->state.Auto_Clamp) 
		{
			if(keyBoardLock == 1){
				Auto_Ctrl(&Clip.Gr,1);
				keyBoardLock = 2;
			}else{
				Auto_Ctrl(&Clip.Gr,0);
			}
		}
		else{
			keyBoardLock = 1;
			Poweroff_Ctrl(&Clip.Gr);
		}
		vTaskDelay(2);
	}
#endif 
}
