#ifndef __CHASSISMOTOR_H_
#define __CHASSISMOTOR_H_
#include "motor.h"
#include "PYR_ISR.h"

/*���̽ṹ��*/
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



/*�������ӳ�ʼ��*/
void Wheel_Motor_Init(C_t *C);


/*���̲���*/
void Chassis_Text_Drive(C_t *C);


/*���̶���ģʽ*/
void Chassis_Indepen_Drive(C_t *C,float X_IN,float Y_IN,float Z_IN,int16_t ExpRescue);


/*����ֱ��ģʽ*/
void Chassis_Straight_Drive(C_t *C,int16_t speed);


/*����Ť��ģʽ*/
void Chassis_Wiggle_Drive(C_t *C,float X_IN,float Y_IN,float Z_IN);


/*���̶ϵ�*/
void Chassis_Poweroff_Drive(C_t *C);


/*��Ԯ�����ʼ��*/
void Rescue_Motor_Init(C_t *C);


/*������Ԯ���*/
void Rescue_Ctrl(C_t *C,int8_t choice);
	

#endif
