#ifndef __GRASP_TASK_H_
#define __GRASP_TASK_H_
#include "GraspMotor.h"
#include "BoardCommuni.h"

typedef __packed struct
{
	/*数据变量*/
	Gr_t Gr;
	
	/*函数指针变量*/
	void (*Motor_Init)(Gr_t *Gr);	//夹取电机初始化
}Grasp_t;

void Grasp_Task(void *pvParameters);
	
#endif
