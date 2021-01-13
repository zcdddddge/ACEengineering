
#ifndef __REMOTE_TASK_H_
#define __REMOTE_TASK_H_
#include "RemoteDeal.h"
#include "GraspMotor.h"
#include "BoardCommuni.h"
#include "RobotAction.h"

/*************************模式切换 置1双机通信************************************/
#define RC_SWITCH 0
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
}Test_t;

void Remote_Task(void *pvParameters);

#endif
