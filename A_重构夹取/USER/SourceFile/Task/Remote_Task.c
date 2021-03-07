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
	
    /*****************ң�ؿ��Ƽ�ȡ************************************/
		#if 0 
		if(Grasp.Can2_RC->Can_RC.s1 == 3 && Grasp.Can2_RC->Can_RC.s2==3) {
			if(lock == 1){
				Auto_Ctrl(&Grasp.Gr,1);  // û����������ƴ����,������֤����
				lock = 2;
			}
			else{
				Auto_Ctrl(&Grasp.Gr,0);
			}
		}
		else if(Grasp.Can2_RC->Can_RC.s1 == 3 && Grasp.Can2_RC->Can_RC.s2 == 2){
 			Reset_Grasp(&Grasp.Gr);
		}
		else{
			lock = 1;
			Poweroff_Ctrl(&Grasp.Gr);
		}
		#endif 
		
		 
    /****************���̿��Ƶ���**********************************/ 
		#if 0 
		if(Grasp.Can2_RC->state.Magazine==2) {
				SupplyOpen;
		}else if(Grasp.Can2_RC->state.Magazine==1){
				SupplyClose;
		}
		#endif 
		
		#if 0 
		/**************���̿��Ƽ�ȡ***********************************/ 
		if(Grasp.Can2_RC->state.Auto_Clamp) 
		{
			if(keyBoardLock == 1){
				Auto_Ctrl(&Grasp.Gr,1);
				keyBoardLock = 2;
			}else{
				Auto_Ctrl(&Grasp.Gr,0);
			}
		}
		else{
			keyBoardLock = 1;
			Poweroff_Ctrl(&Grasp.Gr);
		}
		#endif 
		vTaskDelay(2);
	}
}

