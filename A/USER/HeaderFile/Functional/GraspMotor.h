#ifndef __GRASPMOTOR_H_
#define __GRASPMOTOR_H_
#include "motor.h"
#include "REMOTE_ISR.h"
#include "GPIO_DEAL.h"
#include "vL53L0.h"
#include "stm32f4xx.h"
#include "MathLib.h"
#include "FilterLib.h"

/****************************传感器结构体*****************************************/
#define Update_Sensor_Val 			Get_Sensor_Val
#define Update_VL53L0_Val				VL53L0_Data_Deal

/****************************发送电机数值*****************************************/
#define MotorOutput_201_204		Gr->GraspMotor[0].SPID.Out,Gr->GraspMotor[1].SPID.Out,Gr->GraspMotor[2].SPID.Out,Gr->GraspMotor[3].SPID.Out
#define MotorOutput_205_208		Gr->GraspMotor[4].SPID.Out,Gr->GraspMotor[5].SPID.Out,0,0


//#define MotorOutput_201_204		0,0,0,0
//#define MotorOutput_205_208		0,0,0,0

/********************************电机状态************************************************/
static const u8 Finish 		= 1;
static const u8 DisFinish = 0;

/*******************************电机pid***********************************************/
/*翻转*/
#define GRASP_SWING_S_P   	4.0f
#define GRASP_SWING_S_I   	0.0f
#define GRASP_SWING_S_D   	0.05f
#define GRASP_SWING_P_P  		100.0f
#define	GRASP_SWING_P_I   	0.0f
#define GRASP_SWING_P_D   	50.0f
/*夹紧*/
#define GRASP_CLAMP_S_P   	3.5f
#define GRASP_CLAMP_S_I   	0.0f
#define GRASP_CLAMP_S_D   	0.02f
#define GRASP_CLAMP_P_P  		20.7f
#define	GRASP_CLAMP_P_I   	0.0f
#define GRASP_CLAMP_P_D   	10.0f
/*传送*/
#define GRASP_TRANS_S_P   	1.22f
#define GRASP_TRANS_S_I   	0.0f
#define GRASP_TRANS_S_D   	0.02f
#define GRASP_TRANS_P_P  		100.5f
#define	GRASP_TRANS_P_I   	0.0f
#define GRASP_TRANS_P_D   	0.0f
/*平移*/
#define GRASP_SMOOTH_S_P   	4.2f
#define GRASP_SMOOTH_S_I   	0.0f
#define GRASP_SMOOTH_S_D   	0.01f
#define GRASP_SMOOTH_P_P  	120.0f
#define	GRASP_SMOOTH_P_I   	0.0f
#define GRASP_SMOOTH_P_D   	20.0f
/*抬升*/
#define GRASP_SLIDE_S_P   	4.0f
#define GRASP_SLIDE_S_I   	0.0f
#define GRASP_SLIDE_S_D   	0.2f
#define GRASP_SLIDE_P_P  		350.5f
#define	GRASP_SLIDE_P_I   	0.0f
#define GRASP_SLIDE_P_D   	100.0f
/*交接弹丸*/
#define  GRASP_SUPPLY_S_P   1.22f
#define  GRASP_SUPPLY_S_I   0.0f
#define  GRASP_SUPPLY_S_D   0.02f
#define  GRASP_SUPPLY_P_P   100.5f
#define  GRASP_SUPPLY_P_I   0.0f
#define  GRASP_SUPPLY_P_D   0.1f

/*旋转*/
#define GRASP_ROTATE_S_P  	1.22f
#define	GRASP_ROTATE_S_I   	0.0f
#define GRASP_ROTATE_S_D   	0.2f
#define GRASP_ROTATE_P_P   	100.0f
#define GRASP_ROTATE_P_I   	0.0f
#define GRASP_ROTATE_P_D   	0.0f

/****************************电机速度定义*****************************************/

/*夹子速度值*/
static const int16_t Cilp_Speed 				= 5500;
static const int16_t Rall_Speed 				= 1000;
static const int16_t UpLift_Speed 			= 3000;
static const int16_t Telescoping_Speed	= 1000;
/*夹取结构体*/
typedef __packed struct
{
	Motor_t GraspMotor[7];
	Sensor_t *sensor;
	VL53L0_t *vL53L0;
	u8 state[4];
	u8 reset[8];
	void (*Can_Send_Grasp_1)(int16_t,int16_t,int16_t,int16_t);
	void (*Can_Send_Grasp_2)(int16_t,int16_t,int16_t,int16_t);
	Encoder_t*(*Get_Encoder)(u8);
	Sensor_t* (*Get_Sensor_t)(void);
	VL53L0_t* (*Get_VL53L0_t)(void);
}Gr_t;



/*夹取主部分初始化*/
void Grasp_Motor_Init(Gr_t *Gr);

/*遥控控制*/
void RC_Ctrl(Gr_t *Gr,RC_ctrl_t *rc);

/*断电控制*/
void Poweroff_Ctrl(Gr_t *Gr);

/*复位夹取*/
u8 ResetGrasp(Gr_t *Gr);

#endif
