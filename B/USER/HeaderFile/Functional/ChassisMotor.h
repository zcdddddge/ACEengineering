#ifndef __CHASSISMOTOR_H_
#define __CHASSISMOTOR_H_
#include "motor.h"
#include "PYR_ISR.h"

/*底盘结构体*/
typedef __packed struct
{
	Motor_t WheelMotor[4];
	Motor_t	RescueMotor;
	PYR_t *gyro;
	PID_t Yaw_Pid;
	void (*Can_Send_Wheel)(int16_t,int16_t,int16_t,int16_t);
	void (*Can_Send_Rescue)(int16_t,int16_t,int16_t,int16_t);
	Encoder_t*(*Get_Encoder)(u8 ch);
	PYR_t* (*Get_PYR_t)(void);
}C_t;



/*底盘轮子初始化*/
void Wheel_Motor_Init(C_t *C);


/*底盘测试*/
void Chassis_Text_Drive(C_t *C);


/*底盘独立模式*/
void Chassis_Indepen_Drive(C_t *C,float X_IN,float Y_IN,float Z_IN,int16_t ExpRescue);


/*底盘直走模式*/
void Chassis_Straight_Drive(C_t *C,int16_t speed);


/*底盘扭腰模式*/
void Chassis_Wiggle_Drive(C_t *C,float X_IN,float Y_IN,float Z_IN);


/*底盘断电*/
void Chassis_Poweroff_Drive(C_t *C);


/*救援电机初始化*/
void Rescue_Motor_Init(C_t *C);


/*驱动救援电机*/
void Rescue_Ctrl(C_t *C,int8_t choice);
	

#endif
