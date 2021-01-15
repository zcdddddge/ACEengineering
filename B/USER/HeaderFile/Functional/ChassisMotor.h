/*
 * @Author: your name
 * @Date: 2021-01-09 15:32:34
 * @LastEditTime: 2021-01-11 15:33:01
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \PROJECTd:\RMware\B\USER\HeaderFile\Functional\ChassisMotor.h
 */
#ifndef __CHASSISMOTOR_H_
#define __CHASSISMOTOR_H_
#include "motor.h"
#include "PYR_ISR.h"


/**********************************Can������ֵ************************************************/
#define Wheel_Output	 C->WheelMotor[0].SPID.Out,C->WheelMotor[1].SPID.Out,C->WheelMotor[2].SPID.Out,C->WheelMotor[3].SPID.Out
#define Rescue_Output	 C->RescueMotor.SPID.Out,0,0,0
//#define Wheel_Output	 0,0,0,0
/*********************************���̵��pid**************************************************/
/*���������ٶȻ�����*/
#define WHEEL_MOTOR1_P   	4.0f
#define WHEEL_MOTOR1_I   	0.0f
#define WHEEL_MOTOR1_D   0.02f
#define WHEEL_MOTOR2_P  	4.0f
#define	WHEEL_MOTOR2_I   	0.0f
#define WHEEL_MOTOR2_D   0.02f
#define WHEEL_MOTOR3_P   	4.0f
#define WHEEL_MOTOR3_I   	0.0f
#define WHEEL_MOTOR3_D   0.02f
#define WHEEL_MOTOR4_P   	4.0f
#define WHEEL_MOTOR4_I   	0.0f
#define WHEEL_MOTOR4_D   0.02f
/*���̾�Ԯ�������*/
#define  RESCUE_S_P   4.0f
#define  RESCUE_S_I   0.0f
#define  RESCUE_S_D   1.0f
#define  RESCUE_P_P   0.0f
#define  RESCUE_P_I   0.0f
#define  RESCUE_P_D   0.0f
/*Yaw�ջ�pid*/
#define Yaw_P					40.0f
#define Yaw_I					0.0f
#define Yaw_D					10.0f
/**************************************************�ٶȶ���**********************************/
static const int16_t Rescue_Speed    =5500 ;   //��Ԯ����ٶ�

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
void Chassis_Rescue(C_t *C ,int16_t dire) ;
#endif
