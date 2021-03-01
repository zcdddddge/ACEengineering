/*
 *                        _oo0oo_
 *                       o8888888o
 *                       88" . "88
 *                       (| -_- |)
 *                       0\  =  /0
 *                     ___/`---'\___
 *                   .' \\|     |// '.
 *                  / \\|||  :  |||// \
 *                 / _||||| -:- |||||- \
 *                |   | \\\  - /// |   |
 *                | \_|  ''\---/''  |_/ |
 *                \  .-\__  '-'  ___/-. /
 *              ___'. .'  /--.--\  `. .'___
 *           ."" '<  `.___\_<|>_/___.' >' "".
 *          | | :  `- \`.;`\ _ /`;.`/ - ` : | |
 *          \  \ `_.   \_ __\ /__ _/   .-` /  /
 *      =====`-.____`.___ \_____/___.-`___.-'=====
 *                        `=---='
 *      ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *            ���汣��       ����崻�     ����BUG
 * 
RC_X-->ch1
RC_Y-->ch0
RC_Z-->ch2
 */

#include "ChassisMotor.h"
#include "MathLib.h"




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
			Z_IN = PID_DEAL(&C->Yaw_Pid,0,(-C->gyro->Yaw_Var)); // set=0 ref=-C->gyro->Yaw_Var  
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
*˵��:	Ť��ģʽҲ���Կ������˶�  
*************************************************************************************************/
void Chassis_Wiggle_Drive(C_t *C,float X_IN,float Y_IN,float Z_IN)
{
		u8 ID_C = 0;
		int16_t Val[4] = {0,0,0,0};
		int16_t MAX = 0;
		static u8 dir = 0;
		static float CurYaw =0 ;
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
		
		#if 0  
		/*ʹ�ÿ⺯�� |  ����= �Ƕ�*PI/180*=============*/
		
		if(X_IN != 0)
		{
			X_IN = X_IN * float_abs(cosf(-(C->gyro->Yaw_Var)/180*3.14159216f));
		}
		if(Y_IN != 0)
		{
			Y_IN = Y_IN * float_abs(cosf(-(C->gyro->Yaw_Var)/180*3.14159216f)); //�ĳ�cosf 
		}
		#endif 
		
	  #if 1
		//�ò�� 
		if(X_IN != 0)
		{
			X_IN = X_IN * float_abs( cos_calculate(-(C->gyro->Yaw_Var))  );
			CurYaw = C->gyro->Yaw_Var ; //��ǰ�ĽǶ�
			 
		}
		if(Y_IN != 0)
		{
			Y_IN = Y_IN * float_abs( cos_calculate (-(C->gyro->Yaw_Var)));
		}
		#endif 
		
		
		/************1.13/12:04 ***********************************/
		
		
		
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
*˵��:	Ӧ�������ҵ�--δ���� 
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



/**
 * @description: ���̾�Ԯ���  s1=2  s2=1   ��Ԯ��ǰ�� s1=2  s2=2 	��Ԯ������
 * @param {C_t} *C
 * @param {int16_t} dire -1��Ӧǰ��  1��Ӧ���� ��Ӧ����ٶȵ���������ǰ���ٶ�С��0 
 * @return {*}
 * ˵��:2020-11-19�ڶ��棺��������ȥ�Ĺ��ܣ��β��ж�
 *               ������ֻ����ת���ٶȻ��������βξ�������  
 */
void Chassis_Rescue(C_t *C ,int16_t dire) 
{
	static int16_t clock =0;
	static int8_t  lock =1; 
	static int8_t  last_dire= 0; 
	if( (dire!=-1) && (dire!=1))
		return ;
	
	if(last_dire==0) {
		last_dire = dire ;
	}
	if(last_dire != dire) {  //��ǰҪ�������һ�η���ͬ��״ֵ̬��λ 
		lock 	=1; 
		clock =0;
		last_dire = dire;
	}
	C->RescueMotor.ExpSpeed = dire * Rescue_Speed; 
	if(C->RescueMotor.Encoder->Speed[1]<=100 &&C->RescueMotor.Encoder->Speed[1]>=-100)  
	{
		clock++;
		if(clock>=60){
			lock=2;  
		}
	}
	if(lock==2){
		  C->RescueMotor.SPID.Out = 0; // ֱ������ͣ���� 
	}
	else if(lock==1){
		PID_DEAL(&C->RescueMotor.SPID,C->RescueMotor.ExpSpeed,C->RescueMotor.Encoder->Speed[1] ) ;
	}
	C->Can_Send_Rescue(Rescue_Output);
}
