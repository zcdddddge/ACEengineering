#include "GimbalMotor.h"


/*Pitch轴*/
#define PITCH_P_P  1.0f
#define PITCH_P_I  0.0f
#define PITCH_P_D  0.0f
#define PITCH_S_P  4.0f
#define PITCH_S_I  0.0f
#define PITCH_S_D  0.0f
/*Yaw轴*/
#define YAW_P_P    40.0f  //401.0 
#define YAW_P_I	 	 0.0f
#define YAW_P_D    0.0f
#define YAW_S_P    3.0f
#define YAW_S_I	 	 0.0f
#define YAW_S_D	 	 0.01f


/*************************************************************************************************
*名称:	PY_Motor_Init
*功能:	云台PY轴电机初始化
*形参: 	G_t *G
*返回:	无
*说明:	无
*************************************************************************************************/
void PY_Motor_Init(G_t *G)
{		
		/*函数映射*/
		G->Can_Send_Gimbal				=	CAN1_205_To_208_SEND;
		G->Can_Send_Y             = CAN1_SEND_6020_7;

		/*清零处理*/
		MotorValZero(&G->PitchMotor);
		MotorValZero(&G->YawMotor);
	
		/*码盘赋值*/
		G->PitchMotor.Encoder = Return_Can1_201_208_Encoder(8);
		G->YawMotor.Encoder		=	Return_Can1_201_208_Encoder(7);
	
		/*PITCH*/
		G->PitchMotor.ID = 8;
		PID_INIT(&G->PitchMotor.PPID , PITCH_P_P , PITCH_P_I , PITCH_P_D , 0.0f , 0.0f);		//外环参数初始化
		PID_INIT(&G->PitchMotor.SPID , PITCH_S_P , PITCH_S_I , PITCH_S_D , 0.0f , 0.0f);		//内环参数初始化
		G->PitchMotor.ExpRadian							= G->PitchMotor.Encoder->Radian;					//初始化期望角度
		G->PitchMotor.Encoder->Init_Radian 	= G->PitchMotor.Encoder->Radian;    			//初始码盘值赋值
		G->PitchMotor.Encoder->Lock_Radian 	= G->PitchMotor.Encoder->Radian;	    		//初始化上锁角度
		G->PitchMotor.Radio = 19;																										//初始化底盘电机减速比
		
		/*YAW*/
		G->YawMotor.ID = 7;
		PID_INIT(&G->YawMotor.PPID , YAW_P_P , YAW_P_I , YAW_P_D , 0.0f , 0.0f);		//外环参数初始化
		PID_INIT(&G->YawMotor.SPID , YAW_S_P , YAW_S_I , YAW_S_D , 0.0f , 0.0f);		//内环参数初始化
		G->YawMotor.ExpRadian							= G->YawMotor.Encoder->Radian;					//初始化期望角度
		G->YawMotor.Encoder->Init_Radian 	= G->YawMotor.Encoder->Radian;    			//初始码盘值赋值
		G->YawMotor.Encoder->Lock_Radian 	= G->YawMotor.Encoder->Radian;	    		//初始化上锁角度
		G->YawMotor.Radio = 1;																										//初始化底盘电机减速比

}





/*************************************************************************************************
*名称:	PY_Encoder_DRIVE
*功能:	驱动云台PY轴电机码盘模式
*形参: 	G_t *G,float P,float Y,float Y_SPEED
*返回:	无
*说明:	未经过输入值超转处理！！！
*************************************************************************************************/
void PY_Encoder_DRIVE(G_t *G,float P,float Y,float Y_SPEED)
{
	
	//G->YawMotor.ExpRadian 				= Y;
	//G->YawMotor.Encoder->Speed[1] = Y_SPEED;
	G->PitchMotor.ExpRadian 			= P;
	
//	G->PitchMotor.PPID.Out = PID_DEAL_OVERSHOOT(&G->PitchMotor.PPID, G->PitchMotor.ExpRadian , G->PitchMotor.Encoder->Radian);
//	G->PitchMotor.SPID.Out = PID_DEAL(&G->PitchMotor.SPID, G->PitchMotor.PPID.Out , G->PitchMotor.Encoder->Speed[1]); //Set  Ref 
	
//	G->YawMotor.PPID.Out = PID_DEAL_OVERSHOOT(&G->YawMotor.PPID, G->YawMotor.ExpRadian , G->YawMotor.Encoder->Radian);
	G->YawMotor.SPID.Out = PID_DEAL(&G->YawMotor.SPID,Y_SPEED , G->YawMotor.Encoder->Speed[1]);
	
	G->YawMotor.SPID.Out =2000;
	G->Can_Send_Gimbal(100,100,G->YawMotor.SPID.Out,100);
}



/*************************************************************************************************
*名称:	Gimbal_Poweroff
*功能:	云台断电
*形参: 	G_t *G
*返回:	无
*说明:	无
*************************************************************************************************/
void Gimbal_Poweroff(G_t *G)
{
	G->PitchMotor.SPID.Out = G->YawMotor.SPID.Out = 0;
	G->Can_Send_Gimbal(0,0,0,0); 
}




#if 0 
/*************************************************************************************************
*名称:	Ammuniti_Motor_Init
*功能:	拨弹电机初始化
*形参: 	G_t *G
*返回:	无
*说明:	无
*************************************************************************************************/
void Ammuniti_Motor_Init(G_t *G)
{
	MotorValZero(&G->AmmunitiMotor);
	G->AmmunitiMotor.ID = 8;																//ID赋值	
	
	PID_INIT(&G->AmmunitiMotor.PPID,AMMUNITI_P_P,AMMUNITI_P_I,AMMUNITI_P_D,0.0f,0.0f);																				//外环初始化
	PID_INIT(&G->AmmunitiMotor.SPID,AMMUNITI_S_P,AMMUNITI_S_I,AMMUNITI_S_D,0.0f,0.0f);    																		//内环初始化
	
	/*码盘赋值*/
	G->AmmunitiMotor.Encoder = G->Get_Ammuniti_Encoder();
	
	G->AmmunitiMotor.ExpRadian						= G->AmmunitiMotor.Encoder->Radian;					//初始化期望角度
	G->AmmunitiMotor.Encoder->Init_Radian = G->AmmunitiMotor.Encoder->Radian;    			//初始码盘值赋值
	G->AmmunitiMotor.Encoder->Lock_Radian = G->AmmunitiMotor.Encoder->Radian;	    		//初始化上锁角度
		
	G->AmmunitiMotor.MotorType = AMMUNITI_M;																					//初始化电机种类
	G->AmmunitiMotor.Radio = 36;																											//初始化底盘电机减速比
}



/*************************************************************************************************
*名称:	Ammuniti_Ctrl
*功能:	拨弹电机
*形参: 	G_t *G,int8_t choice
*返回:	无
*说明:	无
*************************************************************************************************/
void Ammuniti_Ctrl(G_t *G,int8_t SpeedGain)
{
	if((SpeedGain > 0 && SpeedGain < 11) || (SpeedGain < 0 && SpeedGain > -11))
	{
		G->AmmunitiMotor.ExpSpeed = SpeedGain*500;
		
	}
	else
	{
		G->AmmunitiMotor.ExpSpeed = 0;
	}
	
	G->AmmunitiMotor.SPID.Out = PID_DEAL(&G->AmmunitiMotor.SPID, G->AmmunitiMotor.ExpSpeed , G->AmmunitiMotor.Encoder->Speed[1]);
	
	G->Can_Send_Gimbal(G->PitchMotor.SPID.Out,G->YawMotor.SPID.Out,G->AmmunitiMotor.SPID.Out,0);
}
#endif 
