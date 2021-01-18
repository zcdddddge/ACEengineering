#ifndef __CHASSIS_TASK_H_
#define __CHASSIS_TASK_H_
#include "ChassisMotor.h"
#include "RemoteDeal.h"
#include "Chassis_Fsm.h"

typedef __packed struct
{
	/*数据变量*/
	C_t C;
	REMOTE_t *RC;
	FSM_t *Fsm;
	
	/*函数指针变量*/
	void (*Fsm_Init)(void);															//状态机初始化
	void (*Wheel_Init)(C_t *C);													//轮子电机初始化
	void (*Rescue_Init)(C_t *C);												//救援电机初始化
	void (*Indepen)(C_t*,float ,float ,float ,int16_t);	//底盘跟随
	void (*Wiggle)(C_t*,float ,float ,float);						//底盘摇摆
	void (*Rescue)(C_t *,int16_t );                     //救援电机救援 
	void (*Poweroff)(C_t*);	                          	//底盘断电
	void (*BoardCommuni_Update)(int16_t *speed);                  //夹取控制底盘
}Chassis_t;

void Chassis_Task(void *pvParameters);

#endif
