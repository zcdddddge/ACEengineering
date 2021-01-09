#include "ChassisMotor.h"
#include "MathLib.h"

//#define Wheel_Output	 C->WheelMotor[0].SPID.Out,C->WheelMotor[1].SPID.Out,C->WheelMotor[2].SPID.Out,C->WheelMotor[3].SPID.Out
//#define Rescue_Output	 C->RescueMotor.SPID.Out,0,0,0



#define Wheel_Output	 0,0,0,0
#define Rescue_Output	 C->RescueMotor.SPID.Out,0,0,0

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
	/*��Ԯ*/
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

/*************************************************************************************************
*����:	Wheel_Motor_Init
*����:	�������ӵ����ʼ��
*�β�: 	C_t *C
*����:	��
*˵��:	��
*************************************************************************************************/
void Wheel_Motor_Init(C_t *C)
{		
		u8 CMi = 0;
		float Spid[4][3] = {
		{WHEEL_MOTOR1_P,WHEEL_MOTOR1_I,WHEEL_MOTOR1_D},
		{WHEEL_MOTOR2_P,WHEEL_MOTOR2_I,WHEEL_MOTOR2_D},
		{WHEEL_MOTOR3_P,WHEEL_MOTOR3_I,WHEEL_MOTOR3_D},
		{WHEEL_MOTOR4_P,WHEEL_MOTOR4_I,WHEEL_MOTOR4_D},
		};
			
		/*����ӳ��*/
		C->Can_Send_Rescue 		= CAN1_205_To_208_SEND;
		C->Can_Send_Wheel	 		= CAN1_201_To_204_SEND;
		C->Get_Encoder				=	Return_Can1_201_208_Encoder;
		C->Get_PYR_t					= Return_PYR_t;
		
		/*���㴦��*/
		MotorValZero(&C->WheelMotor[0]);
		MotorValZero(&C->WheelMotor[1]);
		MotorValZero(&C->WheelMotor[2]);
		MotorValZero(&C->WheelMotor[3]);
	
		for(CMi = 0;CMi < 4;CMi ++)
		{
				C->WheelMotor[CMi].ID = CMi + 1;
				C->WheelMotor[CMi].Encoder	=	C->Get_Encoder(CMi+1);
				PID_INIT(&C->WheelMotor[CMi].SPID,Spid[CMi][0],Spid[CMi][1],Spid[CMi][2],15000,16000);	//�ٶȻ���ʼ��
				C->WheelMotor[CMi].MotorType = CHASSIS_M;																								//��ʼ���������
				C->WheelMotor[CMi].Radio = 19;																													//��ʼ�����̵�����ٱ�
		}
		
		/*��ȡ��������������*/
		C->gyro = C->Get_PYR_t();
		
		/*��ʼ������Yaw��pid*/
		PID_INIT(&C->Yaw_Pid,Yaw_P,Yaw_I,Yaw_D,0,660);
}



/*************************************************************************************************
*����:	CHASSIS_TEXT_DRIVE
*����:	�������̵������ģʽ
*�β�: 	C_t *C
*����:	��
*˵��:	��
*************************************************************************************************/
void Chassis_Text_Drive(C_t *C)
{
	
}



/*************************************************************************************************
*����:	Chassis_Indepen_Drive
*����:	���̶���ģʽ
*�β�: 	C_t *C,float X_IN,float Y_IN,float Z_IN
*����:	��
*˵��:	��
*************************************************************************************************/
void Chassis_Indepen_Drive(C_t *C,float X_IN,float Y_IN,float Z_IN,int16_t ExpRescue)
{
		u8 ID_C = 0;
		int16_t Val[4] = {0,0,0,0};
		int16_t MAX = 0;
		float SPID_OUT[4];
		
		if(Z_IN == 0 && ( (X_IN != 0) || (Y_IN != 0) ) )
		{
			Z_IN = PID_DEAL(&C->Yaw_Pid,0,(-C->gyro->Yaw_Var) );
		}
		else
		{
			C->gyro->Yaw_Lock = C->gyro->Yaw;
		}
		
		/*����Զ���ֽ�*/
		C->WheelMotor[0].ExpSpeed = (X_IN + Y_IN + Z_IN);		
		C->WheelMotor[1].ExpSpeed = -(X_IN - Y_IN - Z_IN);
		C->WheelMotor[2].ExpSpeed = (X_IN - Y_IN + Z_IN);
		C->WheelMotor[3].ExpSpeed = -(X_IN + Y_IN - Z_IN);
		
		/*�ٶ�����*/
		C->WheelMotor[0].ExpSpeed *= 10;
		C->WheelMotor[1].ExpSpeed *= 10;
		C->WheelMotor[2].ExpSpeed *= 10;
		C->WheelMotor[3].ExpSpeed *= 10;
	
		/*PID����*/
		for(ID_C = 0;ID_C < 4;ID_C ++)
	 {
			SPID_OUT[ID_C] = PID_DEAL(&C->WheelMotor[ID_C].SPID , C->WheelMotor[ID_C].ExpSpeed , C->WheelMotor[ID_C].Encoder->Speed[1]);		//�õ������
			Val[ID_C] = C->WheelMotor[ID_C].Encoder->Speed[1];																														    							//��¼���ʵʱ�ٶ�ֵ
	 }
	
	/*���̵���˶�ʧ�洦�������ǰ���ǽ�PID���ֵ���ٶ�ֵ��Ϊ�����Թ�ϵ*/
	 MAX = RETURN_MAX(Val,4);									//��ȡ����ٶ�
	 if(MAX == 0 || MAX <= 8750)	 						//����ٶ�Ϊ0��С�ڵ������ٶȣ�������ֵ
	 {
			C->WheelMotor[0].SPID.Out = SPID_OUT[0];			 
			C->WheelMotor[1].SPID.Out = SPID_OUT[1];
			C->WheelMotor[2].SPID.Out = SPID_OUT[2];
			C->WheelMotor[3].SPID.Out = SPID_OUT[3];
	 }
	 else																		 //��������˶�ʧ�洦��
	 {
			/*ȷ���ٶȲ�Ϊ0ʱ���Ž��д���*/
			if(Val[0] != 0)
				C->WheelMotor[0].SPID.Out = SPID_OUT[0] * int16_t_abs(Val[0]) / MAX;
			else
				C->WheelMotor[0].SPID.Out = SPID_OUT[0];
			if(Val[1] != 0)
				C->WheelMotor[1].SPID.Out = SPID_OUT[1] * int16_t_abs(Val[1]) / MAX;
			else
				C->WheelMotor[1].SPID.Out = SPID_OUT[1];
			if(Val[2] != 0)
				C->WheelMotor[2].SPID.Out = SPID_OUT[2] * int16_t_abs(Val[2]) / MAX;
			else
				C->WheelMotor[2].SPID.Out = SPID_OUT[2];
			if(Val[3] != 0)
				C->WheelMotor[3].SPID.Out = SPID_OUT[3] * int16_t_abs(Val[3]) / MAX;
			else
				C->WheelMotor[3].SPID.Out = SPID_OUT[3];
	 }
		
	 C->RescueMotor.ExpSpeed = (float)ExpRescue * 5;
	 PID_DEAL(&C->RescueMotor.SPID,C->RescueMotor.ExpSpeed,C->RescueMotor.Encoder->Speed[1]);
	 
	 /*CAN1ͨ�ŷ���ִ��*/
	 C->Can_Send_Wheel(Wheel_Output);
	 C->Can_Send_Rescue(Rescue_Output);
}



/*************************************************************************************************
*����:	Chassis_Wiggle_Drive
*����:	����Ť��ģʽ
*�β�: 	C_t *C,float X_IN,float Y_IN,float Z_IN
*����:	��
*˵��:	��
*************************************************************************************************/
void Chassis_Wiggle_Drive(C_t *C,float X_IN,float Y_IN,float Z_IN)
{
		u8 ID_C = 0;
		int16_t Val[4] = {0,0,0,0};
		int16_t MAX = 0;
		static u8 dir = 0;
		float SPID_OUT[4];
			
		if(Z_IN == 0.0f)
		{
			if(dir == 0)
			{
				Z_IN = PID_DEAL(&C->Yaw_Pid,30,(-C->gyro->Yaw_Var) );
				if((-C->gyro->Yaw_Var) >= 20)
				{
					dir = 1;
				}
			}
			else if(dir == 1)
			{
				Z_IN = PID_DEAL(&C->Yaw_Pid,-30,(-C->gyro->Yaw_Var) );
				if((-C->gyro->Yaw_Var) <= -20)
				{
					dir = 0;
				}
			}
		}
		else
		{
			C->gyro->Yaw_Lock = C->gyro->Yaw;
		}
		
		/*����Զ���ֽ�*/
		if(X_IN != 0)
		{
			X_IN = X_IN * float_abs(cosf(-(C->gyro->Yaw_Var)/180*3.14159216f));
		}
		if(Y_IN != 0)
		{
			Y_IN = Y_IN * float_abs(sinf(-(C->gyro->Yaw_Var)/180*3.14159216f));
		}
		
		
		C->WheelMotor[0].ExpSpeed = 	(X_IN*10 + Y_IN*10 + Z_IN*2);		
		C->WheelMotor[1].ExpSpeed = -	(X_IN*10 - Y_IN*10 - Z_IN*2);
		C->WheelMotor[2].ExpSpeed = 	(X_IN*10 - Y_IN*10 + Z_IN*2);
		C->WheelMotor[3].ExpSpeed = -	(X_IN*10 + Y_IN*10 - Z_IN*2);
		
	
		/*PID����*/
		for(ID_C = 0;ID_C < 4;ID_C ++)
	 {
			SPID_OUT[ID_C] = PID_DEAL(&C->WheelMotor[ID_C].SPID , C->WheelMotor[ID_C].ExpSpeed , C->WheelMotor[ID_C].Encoder->Speed[1]);		//�õ������
			Val[ID_C] = C->WheelMotor[ID_C].Encoder->Speed[1];																														    							//��¼���ʵʱ�ٶ�ֵ
	 }
	
	/*���̵���˶�ʧ�洦�������ǰ���ǽ�PID���ֵ���ٶ�ֵ��Ϊ�����Թ�ϵ*/
	 MAX = RETURN_MAX(Val,4);									//��ȡ����ٶ�
	 if(MAX == 0 || MAX <= 8750)	 						//����ٶ�Ϊ0��С�ڵ������ٶȣ�������ֵ
	 {
			C->WheelMotor[0].SPID.Out = SPID_OUT[0];			 
			C->WheelMotor[1].SPID.Out = SPID_OUT[1];
			C->WheelMotor[2].SPID.Out = SPID_OUT[2];
			C->WheelMotor[3].SPID.Out = SPID_OUT[3];
	 }
	 else																		 //��������˶�ʧ�洦��
	 {
			/*ȷ���ٶȲ�Ϊ0ʱ���Ž��д���*/
			if(Val[0] != 0)
				C->WheelMotor[0].SPID.Out = SPID_OUT[0] * int16_t_abs(Val[0]) / MAX;
			else
				C->WheelMotor[0].SPID.Out = SPID_OUT[0];
			if(Val[1] != 0)
				C->WheelMotor[1].SPID.Out = SPID_OUT[1] * int16_t_abs(Val[1]) / MAX;
			else
				C->WheelMotor[1].SPID.Out = SPID_OUT[1];
			if(Val[2] != 0)
				C->WheelMotor[2].SPID.Out = SPID_OUT[2] * int16_t_abs(Val[2]) / MAX;
			else
				C->WheelMotor[2].SPID.Out = SPID_OUT[2];
			if(Val[3] != 0)
				C->WheelMotor[3].SPID.Out = SPID_OUT[3] * int16_t_abs(Val[3]) / MAX;
			else
				C->WheelMotor[3].SPID.Out = SPID_OUT[3];
	 }
	
	 /*CAN1ͨ�ŷ���ִ��*/
	 C->Can_Send_Wheel(Wheel_Output);
}


/*************************************************************************************************
*����:	Chassis_Straight_Drive
*����:	����ֱ��ģʽ
*�β�: 	C_t *C,int16_t speed
*����:	��
*˵��:	��
*************************************************************************************************/
void Chassis_Straight_Drive(C_t *C,int16_t speed)
{
		u8 i;
		static int16_t Y_offset = 0;
		
		Y_offset = PID_DEAL(&C->Yaw_Pid,0,(-C->gyro->Yaw_Var) );
	
		C->WheelMotor[0].ExpSpeed =   speed + Y_offset;		
		C->WheelMotor[1].ExpSpeed =  -speed + Y_offset;
		C->WheelMotor[2].ExpSpeed = 	speed + Y_offset;
		C->WheelMotor[3].ExpSpeed =  -speed + Y_offset;
		
		for(i = 0;i < 4;i ++)
	{
		C->WheelMotor[i].SPID.Out = PID_DEAL(&C->WheelMotor[i].SPID , C->WheelMotor[i].ExpSpeed , C->WheelMotor[i].Encoder->Speed[1]);
	}
	
	 /*CAN1ͨ�ŷ���ִ��*/
	 C->Can_Send_Wheel(Wheel_Output);
}




/*************************************************************************************************
*����:	CHASSIS_POWEROFF
*����:	���̶ϵ�
*�β�: 	C_t *C
*����:	��
*˵��:	��
*************************************************************************************************/
void Chassis_Poweroff_Drive(C_t *C)
{
	C->WheelMotor[0].SPID.Out = C->WheelMotor[1].SPID.Out = C->WheelMotor[2].SPID.Out = C->WheelMotor[3].SPID.Out = 0;
	C->Can_Send_Wheel(0,0,0,0);
}



/*************************************************************************************************
*����:	Rescue_Motor_Init
*����:	��Ԯ�����ʼ��
*�β�: 	C_t *C
*����:	��
*˵��:	��
*************************************************************************************************/
void Rescue_Motor_Init(C_t *C)
{
	MotorValZero(&C->RescueMotor);
	C->RescueMotor.ID = 5;																//ID��ֵ
	
	C->RescueMotor.Encoder = C->Get_Encoder(5);			//��ȡ��Ԯ������̽ṹ��
	
	PID_INIT(&C->RescueMotor.PPID,RESCUE_P_P,RESCUE_P_I,RESCUE_P_D,0.0f,0.0f);																				//�⻷��ʼ��
	PID_INIT(&C->RescueMotor.SPID,RESCUE_S_P,RESCUE_S_I,RESCUE_S_D,0.0f,14700.0f);    																//�ڻ���ʼ��
		
	C->RescueMotor.ExpRadian						= C->RescueMotor.Encoder->Radian;					//��ʼ�������Ƕ�
	C->RescueMotor.Encoder->Init_Radian = C->RescueMotor.Encoder->Radian;    			//��ʼ����ֵ��ֵ
	C->RescueMotor.Encoder->Lock_Radian = C->RescueMotor.Encoder->Radian;	    		//��ʼ�������Ƕ�
		
	C->RescueMotor.MotorType = RESCUE_M;																					//��ʼ���������
	C->RescueMotor.Radio = 36;																										//��ʼ�����̵�����ٱ�
}


/*************************************************************************************************
*����:	Rescue_Ctrl
*����:	��Ԯ�������
*�β�: 	C_t *C,int8_t choice
*����:	��
*˵��:	��
*************************************************************************************************/
void Rescue_Ctrl(C_t *C,int8_t choice)
{
	if(choice)
	{
		
		
	}
	else
	{
		
	}
	
	C->Can_Send_Rescue(C->RescueMotor.SPID.Out,0,0,0);
}
