#ifndef __MOTORTACTION_H_
#define __MOTORTACTION_H_
	
#include "motor.h"
#include "MathLib.h"
#include "vL53L0.h"
#include "GPIO_DEAL.h"


/*电机id-名字宏定义*/
#define UPLIFT          Gr->GraspMotor[0]
#define RALL            Gr->GraspMotor[1]
#define TELESCOPING     Gr->GraspMotor[2]
#define CLIP            Gr->GraspMotor[3]
#define FLIP_1          Gr->GraspMotor[4]
#define FLIP_2          Gr->GraspMotor[5]
#define BULLYSUPPLY     Gr->GraspMotor[6]
#define ROTATE          Gr->GraspMotor[7]

/****************************电机速度定义*****************************************/
static const int16_t Cilp_Speed 				= 5500;
static const int16_t Rall_Speed 				= 1000;
static const int16_t UpLift_Speed 			= 3000;
static const int16_t Telescoping_Speed	    = 3000;
static const int16_t Rotate_Speed           = 2000 ;





void uplift(Motor_t *uplift,VL53L0_t *vl53l0,float dis,u8 dire);
void Telescoping(Motor_t *telescoping,u8 dire);
void flip(Motor_t *filp1,Motor_t *filp2,float exp,float limit,u8 dire);
void clip(Motor_t *clip,int16_t exp,u8 dire);
void rotate(Motor_t *rotate);
void rail(Motor_t *rall,Sensor_t*val,u8 dire); 
#endif 


