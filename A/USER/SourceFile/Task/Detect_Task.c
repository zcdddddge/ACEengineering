/*
 * @Author: your name
 * @Date: 2021-01-13 09:46:09
 * @LastEditTime: 2021-01-13 10:07:34
 * @LastEditors: your name
 * @Description: In User Settings Edit
 * @FilePath: \PROJECTd:\RMware\A\USER\SourceFile\Task\Detect_Task.c
 */
#include "Detect_Task.h"
#include "FreeRTOS.h"
#include "task.h"
#include "stm32f4xx.h"
#include "Remote_Task.h"

#ifdef UART_RC
extern Test_t Text;
#define right 7
static int16_t feed = 0;
#endif 

void Detect_Task(void *pvParameters)
{
	vTaskDelay(500);
	while(1)
	{
#ifdef UART_RC
		if(Text.Remote->RC_ctrl->ch0 >= -660 && Text.Remote->RC_ctrl->ch0 <= 660)
		{
			feed ++;
		}
		if(Text.Remote->RC_ctrl->ch1 >= -660 && Text.Remote->RC_ctrl->ch1 <= 660)
		{
			feed ++;
		}
		if(Text.Remote->RC_ctrl->ch3 >= -660 && Text.Remote->RC_ctrl->ch3 <= 660)
		{
			feed ++;
		}
		if(Text.Remote->RC_ctrl->ch2 >= -660 && Text.Remote->RC_ctrl->ch2 <= 660)
		{
			feed ++;
		}
		if(Text.Remote->RC_ctrl->sw >= -660 && Text.Remote->RC_ctrl->sw <= 660)
		{
			feed ++;
		}
		if(Text.Remote->RC_ctrl->s1 >= 1 && Text.Remote->RC_ctrl->s1 <= 3)
		{
			feed ++;
		}
		if(Text.Remote->RC_ctrl->s2 >= 1 && Text.Remote->RC_ctrl->s2 <= 3)
		{
			feed ++;
		}
		
		if(feed == right || 1)
		{
			IWDG_ReloadCounter();
			feed = 0;
		}
		else
		{
		}
#endif
		GPIO_ToggleBits(GPIOE,GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4);
		GPIO_ToggleBits(GPIOB,GPIO_Pin_7);
		vTaskDelay(200);
	}
}
