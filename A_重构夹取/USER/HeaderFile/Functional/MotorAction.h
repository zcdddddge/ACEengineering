#ifndef __MOTORTACTION_H_
#define __MOTORTACTION_H_
	
#include "motor.h"
#include "MathLib.h"
#include "vL53L0.h"

	

	
/****************************电机速度定义*****************************************/
static const int16_t Cilp_Speed 				= 5500;
static const int16_t Rall_Speed 				= 1000;
static const int16_t UpLift_Speed 			= 3000;
static const int16_t Telescoping_Speed	= 1000;

/*************************电机动作宏定义********************************************/

#define  CLIP_TIGHT      CLIP(&Gr->GraspMotor[3], Cilp_Speed, 1)  // 夹紧
#define  CLIP_LOOSE      CLIP(&Gr->GraspMotor[3], Cilp_Speed, 2) //松夹

#define FLIP_UP         FLIP(&Gr->GraspMotor[4], &Gr->GraspMotor[5], 182.0f, 6.0f, 1);   // 夹取前翻
#define FLIP_DOWN       FLIP(&Gr->GraspMotor[4], &Gr->GraspMotor[5], -160.0f, 20.0f, 2); //夹取后翻

void UPLIFT(Motor_t *uplift,VL53L0_t *vl53l0,float dis,u8 dire);
void Telescoping(Motor_t *telescoping,u8 dire);
void FLIP(Motor_t *filp1,Motor_t *filp2,float exp,float limit,u8 dire);
void CLIP(Motor_t *clip,int16_t exp,u8 dire);
#endif 


