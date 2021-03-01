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
 *            佛祖保佑       永不宕机     永无BUG
 * 
RC_X-->ch1
RC_Y-->ch0
RC_Z-->ch2
 */

#include "ChassisMotor.h"
#include "MathLib.h"




/*************************************************************************************************
*名称:	Wheel_Motor_Init
*功能:	底盘轮子电机初始化
*形参: 	C_t *C
*返回:	无
*说明:	无
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
			
		/*函数映射*/
		C->Can_Send_Rescue 		= CAN1_205_To_208_SEND;
		C->Can_Send_Wheel	 		= CAN1_201_To_204_SEND;
		C->Get_Encoder				=	Return_Can1_201_208_Encoder;
		C->Get_PYR_t					= Return_PYR_t;
		
		/*清零处理*/
		MotorValZero(&C->WheelMotor[0]);
		MotorValZero(&C->WheelMotor[1]);
		MotorValZero(&C->WheelMotor[2]);
		MotorValZero(&C->WheelMotor[3]);
	
		for(CMi = 0;CMi < 4;CMi ++)
		{
			C->WheelMotor[CMi].ID = CMi + 1;
			C->WheelMotor[CMi].Encoder	=	C->Get_Encoder(CMi+1);
			PID_INIT(&C->WheelMotor[CMi].SPID,Spid[CMi][0],Spid[CMi][1],Spid[CMi][2],15000,16000);	//速度环初始化
			C->WheelMotor[CMi].MotorType = CHASSIS_M;																								//初始化电机种类
			C->WheelMotor[CMi].Radio = 19;																													//初始化底盘电机减速比
		}
		
		/*获取底盘陀螺仪数据*/
		C->gyro = C->Get_PYR_t();
		
		/*初始化底盘Yaw轴pid*/
		PID_INIT(&C->Yaw_Pid,Yaw_P,Yaw_I,Yaw_D,0,660);
}



/*************************************************************************************************
*名称:	Chassis_Indepen_Drive
*功能:	底盘独立模式
*形参: 	C_t *C,float X_IN,float Y_IN,float Z_IN
*返回:	无
*说明:	无
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
		
		/*底盘远动分解*/
		C->WheelMotor[0].ExpSpeed = (X_IN + Y_IN + Z_IN);		
		C->WheelMotor[1].ExpSpeed = -(X_IN - Y_IN - Z_IN);
		C->WheelMotor[2].ExpSpeed = (X_IN - Y_IN + Z_IN);
		C->WheelMotor[3].ExpSpeed = -(X_IN + Y_IN - Z_IN);
		
		/*速度增益*/
		C->WheelMotor[0].ExpSpeed *= 10;
		C->WheelMotor[1].ExpSpeed *= 10;
		C->WheelMotor[2].ExpSpeed *= 10;
		C->WheelMotor[3].ExpSpeed *= 10;
	
		/*PID处理*/
		for(ID_C = 0;ID_C < 4;ID_C ++)
	 {
			SPID_OUT[ID_C] = PID_DEAL(&C->WheelMotor[ID_C].SPID , C->WheelMotor[ID_C].ExpSpeed , C->WheelMotor[ID_C].Encoder->Speed[1]);		//得到输出量
			Val[ID_C] = C->WheelMotor[ID_C].Encoder->Speed[1];																														    							//记录电机实时速度值
	 }
	
	/*底盘电机运动失真处理，处理的前提是将PID输出值与速度值认为有线性关系*/
	 MAX = RETURN_MAX(Val,4);									//获取最大速度
	 if(MAX == 0 || MAX <= 8750)	 						//最大速度为0或小于电机最大速度，正常赋值
	 {
			C->WheelMotor[0].SPID.Out = SPID_OUT[0];			 
			C->WheelMotor[1].SPID.Out = SPID_OUT[1];
			C->WheelMotor[2].SPID.Out = SPID_OUT[2];
			C->WheelMotor[3].SPID.Out = SPID_OUT[3];
	 }
	 else																		 //否则进行运动失真处理
	 {
			/*确保速度不为0时，才进行处理*/
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
	 
	 /*CAN1通信发送执行*/
	 C->Can_Send_Wheel(Wheel_Output);
	 C->Can_Send_Rescue(Rescue_Output);
}



/*************************************************************************************************
*名称:	Chassis_Wiggle_Drive
*功能:	底盘扭腰模式
*形参: 	C_t *C,float X_IN,float Y_IN,float Z_IN
*返回:	无
*说明:	扭腰模式也可以控制它运动  
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
		/*使用库函数 |  弧度= 角度*PI/180*=============*/
		
		if(X_IN != 0)
		{
			X_IN = X_IN * float_abs(cosf(-(C->gyro->Yaw_Var)/180*3.14159216f));
		}
		if(Y_IN != 0)
		{
			Y_IN = Y_IN * float_abs(cosf(-(C->gyro->Yaw_Var)/180*3.14159216f)); //改成cosf 
		}
		#endif 
		
	  #if 1
		//用查表法 
		if(X_IN != 0)
		{
			X_IN = X_IN * float_abs( cos_calculate(-(C->gyro->Yaw_Var))  );
			CurYaw = C->gyro->Yaw_Var ; //当前的角度
			 
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
		
	
		/*PID处理*/
		for(ID_C = 0;ID_C < 4;ID_C ++)
	 {
			SPID_OUT[ID_C] = PID_DEAL(&C->WheelMotor[ID_C].SPID , C->WheelMotor[ID_C].ExpSpeed , C->WheelMotor[ID_C].Encoder->Speed[1]);		//得到输出量
			Val[ID_C] = C->WheelMotor[ID_C].Encoder->Speed[1];																														    							//记录电机实时速度值
	 }
	
	/*底盘电机运动失真处理，处理的前提是将PID输出值与速度值认为有线性关系*/
	 MAX = RETURN_MAX(Val,4);									//获取最大速度
	 if(MAX == 0 || MAX <= 8750)	 						//最大速度为0或小于电机最大速度，正常赋值
	 {
			C->WheelMotor[0].SPID.Out = SPID_OUT[0];			 
			C->WheelMotor[1].SPID.Out = SPID_OUT[1];
			C->WheelMotor[2].SPID.Out = SPID_OUT[2];
			C->WheelMotor[3].SPID.Out = SPID_OUT[3];
	 }
	 else																		 //否则进行运动失真处理
	 {
			/*确保速度不为0时，才进行处理*/
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
	
	 /*CAN1通信发送执行*/
	 C->Can_Send_Wheel(Wheel_Output);
}


/*************************************************************************************************
*名称:	Chassis_Straight_Drive
*功能:	底盘直走模式
*形参: 	C_t *C,int16_t speed
*返回:	无
*说明:	应该是左右的--未测试 
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
	
	 /*CAN1通信发送执行*/
	 C->Can_Send_Wheel(Wheel_Output);
}




/*************************************************************************************************
*名称:	CHASSIS_POWEROFF
*功能:	底盘断电
*形参: 	C_t *C
*返回:	无
*说明:	无
*************************************************************************************************/
void Chassis_Poweroff_Drive(C_t *C)
{
	C->WheelMotor[0].SPID.Out = C->WheelMotor[1].SPID.Out = C->WheelMotor[2].SPID.Out = C->WheelMotor[3].SPID.Out = 0;
	C->Can_Send_Wheel(0,0,0,0);
}



/*************************************************************************************************
*名称:	Rescue_Motor_Init
*功能:	救援电机初始化
*形参: 	C_t *C
*返回:	无
*说明:	无
*************************************************************************************************/
void Rescue_Motor_Init(C_t *C)
{
	MotorValZero(&C->RescueMotor);
	C->RescueMotor.ID = 5;																//ID赋值
	
	C->RescueMotor.Encoder = C->Get_Encoder(5);			//获取救援电机码盘结构体
	
	PID_INIT(&C->RescueMotor.PPID,RESCUE_P_P,RESCUE_P_I,RESCUE_P_D,0.0f,0.0f);																				//外环初始化
	PID_INIT(&C->RescueMotor.SPID,RESCUE_S_P,RESCUE_S_I,RESCUE_S_D,0.0f,14700.0f);    																//内环初始化
		
	C->RescueMotor.ExpRadian						= C->RescueMotor.Encoder->Radian;					//初始化期望角度
	C->RescueMotor.Encoder->Init_Radian = C->RescueMotor.Encoder->Radian;    			//初始码盘值赋值
	C->RescueMotor.Encoder->Lock_Radian = C->RescueMotor.Encoder->Radian;	    		//初始化上锁角度
		
	C->RescueMotor.MotorType = RESCUE_M;																					//初始化电机种类
	C->RescueMotor.Radio = 36;																										//初始化底盘电机减速比
}



/**
 * @description: 底盘救援电机  s1=2  s2=1   救援卡前伸 s1=2  s2=2 	救援卡后缩
 * @param {C_t} *C
 * @param {int16_t} dire -1对应前伸  1对应后缩 对应电机速度的正负：往前伸速度小于0 
 * @return {*}
 * 说明:2020-11-19第二版：增加缩回去的功能，形参判断
 *               本函数只做堵转和速度环处理，由形参决定方向  
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
	if(last_dire != dire) {  //当前要求方向和上一次方向不同，状态值复位 
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
		  C->RescueMotor.SPID.Out = 0; // 直接让他停下来 
	}
	else if(lock==1){
		PID_DEAL(&C->RescueMotor.SPID,C->RescueMotor.ExpSpeed,C->RescueMotor.Encoder->Speed[1] ) ;
	}
	C->Can_Send_Rescue(Rescue_Output);
}
