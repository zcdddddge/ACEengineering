#ifndef __GRASP_TASK_H_
#define __GRASP_TASK_H_
#include "GraspMotor.h"
#include "Grasp_Fsm.h"
#include "RobotAction.h" 
#include "BoardCommuni.h"



/*夹取总结构体*/
typedef __packed struct
{
	Gr_t Gr;
	Board_Communi_t *Can2_RC;
	FSM_t *Fsm; 
	void (*Motor_Init)(Gr_t *Gr); // 夹取电机初始化
	void (*Auto_Grasp)(Gr_t *Gr, u8 box);
	void (*Grasp_Poweroff)(Gr_t *Gr);
	void (*Grasp_Reset)(Gr_t *Gr); 
	void (*Grasp_Fsm_Init)(void);
	void (*Bullet_Supply) (Gr_t *Gr, Motor_t *motor, int8_t dire) ;
	void (*RC_Ctrl)(Gr_t *Gr,RC_ctrl_t *rc);
	void (*Send_Crtl)(int16_t ch0); 
}Grasp_t;

void Grasp_Task(void *pvParameters);
	
#endif
