
#ifndef __REMOTE_TASK_H_
#define __REMOTE_TASK_H_
#include "RemoteDeal.h"
#include "GraspMotor.h"
#include "BoardCommuni.h"
#include "RobotAction.h"


#define SupplyOpen           bulletSupply(&Clip.Gr,&Clip.Gr.GraspMotor[6],-1);	 
#define SupplyClose          bulletSupply(&Clip.Gr , &Clip.Gr.GraspMotor[6],1);	 
/**
 * RC_SWITCH置0,使用串口通信,直接接收遥控数据(用于测试)
 * RC_SWITCH置1,使用can通信,遥控数据由底盘板发送(实际使用)
 * 改代码只要改RC_SWITCH即可
 **/ 


#define RC_SWITCH 1
/**************Can通信,主从版**********************/
#if RC_SWITCH
	#define CAN_RC
#endif
/***********串口通信*****************************/
#if (RC_SWITCH == 0)
	#define UART_RC
#endif




typedef __packed struct
{
	
	Gr_t Gr;
#ifdef UART_RC
	REMOTE_t*Remote;
#endif
#ifdef CAN_RC
	Board_Communi_t *Can2_RC;
	
#endif
}Clip_t;

void Remote_Task(void *pvParameters);
#endif
