#ifndef __GRASP_FSM_H_
#define __GRASP_FSM_H_

#include "fsm.h" 

#define MAZAGINE_CLOSE    Grasp.Bullet_Supply(&Grasp.Gr, &Grasp.Gr.GraspMotor[6],-1)
#define MAZAGINE_OPEN     Grasp.Bullet_Supply(&Grasp.Gr, &Grasp.Gr.GraspMotor[6],1) 




void Chassis_FSM_Init(void);

FSM_t *Return_Chassis_Fsm(void) ;

#endif 
