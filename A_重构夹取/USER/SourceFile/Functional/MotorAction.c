/*
 * @Date: 2021-02-24 11:34:45
 * @LastEditTime: 2021-04-01 06:58:35
 * @LastEditors: wingchi-leung 
 * @Description: 工程夹取单个电机的控制逻辑
 */

#include "MotorAction.h"

/*测试版:3.27 测试通过*/
//flip2(&Grasp.Gr.GraspMotor[4], 182.0 ,6, 1 );

void flip2(Motor_t *filp1, float exp, float limit, u8 dire)
{
	static int16_t clock = 0; 
	if (dire == 1)
	{
	
		filp1->ExpRadian -= 0.6f;
		if(filp1->ExpRadian <= filp1->Encoder->Init_Radian - exp)
		{
		//     x <= 6-182 = 
				filp1->ExpRadian = filp1->Encoder->Init_Radian - exp;
		}
		/*完成标志*/
		// 平行： -160 + exp -6 <=6  ===> exp = 172 
		// 垂直： -225 + exp -6 <=6  ===> exp = 237 
		if(float_abs(filp1->Encoder->Total_Radian + exp - filp1->Encoder->Init_Radian) <= limit)
		{          
			filp1->state = Finish; 
			filp1->ExpRadian = filp1->Encoder->Init_Radian - exp;
		}
	
	}
	else if (dire == 2)
	{
		filp1->ExpRadian += 0.4f;
		

		/*完成标志*/
		if (float_abs(filp1->Encoder->Total_Radian - filp1->Encoder->Init_Radian) <= limit)
		{
			clock ++;
		}
	
		if (clock >= 20)
		{
			filp1->ExpRadian = filp1->Encoder->Total_Radian;
			filp1->state = Finish;
			clock= 0;
		}
	}
	else
	{
		filp1->ExpRadian = filp1->Encoder->Total_Radian;
	}
	
	
}

#if 1
/**
  * @description: 使得电机转过绝对角度	
  * @param {Motor_t} *filp1
  * @param {Motor_t} *filp2
  * @param {float} exp
  * @param {float} limit
  * @param {u8} dire 1-前翻  2-后翻
  * @return {*}
  */
void flip(Motor_t *filp1, Motor_t *filp2, float exp, float limit, u8 dire)
{
	static int16_t clock[2] = {0,0};
	if(dire == 1)
	{
		filp1->ExpRadian += 0.6f;   
		filp2->ExpRadian -= 0.6f;
		/*限制*/
		if(filp1->ExpRadian >= filp1->Encoder->Init_Radian + exp)
		{
				filp1->ExpRadian = filp1->Encoder->Init_Radian + exp;
		}
		if(filp2->ExpRadian <= filp2->Encoder->Init_Radian - exp)
		{
				filp2->ExpRadian = filp2->Encoder->Init_Radian - exp;
		}
		/*完成标志*/
		if(float_abs(filp1->Encoder->Total_Radian - exp - filp1->Encoder->Init_Radian) <= limit)
		{
			filp1->state = Finish;  
			filp1->ExpRadian = filp1->Encoder->Init_Radian - exp;
		}
		if(float_abs(filp2->Encoder->Total_Radian + exp - filp2->Encoder->Init_Radian) <= limit)
		{          
			filp2->state = Finish; 
			filp1->ExpRadian = filp1->Encoder->Init_Radian - exp;
		}
	}
	else if(dire == 2) 
	{ 
		filp1->ExpRadian -= 0.4f;
		filp2->ExpRadian += 0.4f;	
		
		/*完成标志*/
		if(float_abs(filp1->Encoder->Total_Radian - filp1->Encoder->Init_Radian) <= limit){ 
			clock[0] ++;
		}
		if(float_abs(filp2->Encoder->Total_Radian - filp2->Encoder->Init_Radian) <= limit){     
			clock[1] ++;
		}
		if(clock[0] >= 20 && clock[1] >= 20){
			filp1->ExpRadian = filp1->Encoder->Total_Radian;    
			filp2->ExpRadian = filp2->Encoder->Total_Radian;  
			filp1->state = Finish;
			filp2->state = Finish;
			clock[0] = 0;
			clock[1] = 0;
		}
	}
	else{
		filp1->ExpRadian = filp1->Encoder->Total_Radian;
		filp2->ExpRadian = filp2->Encoder->Total_Radian;
	}
	
}
#endif



 /**
  * @description: 夹子    
  * @param {Motor_t} *clip 
  * @param {int16_t} exp  : expect Speed 
  * @param {u8} dire      : 1 夹紧   2 松开
  * @return {*}
  */
void clip(Motor_t *clip,int16_t exp,u8 dire)
{
	static int16_t clock = 0;
	clip->debug ++ ;
	if(dire == 1 || dire == 2)
	{
		clip->ExpSpeed = (3 - dire*2)*exp;
		/*结束判断*/
		if(int16_t_abs(clip->Encoder->Speed[1]) <= 50)
		{
			clock ++;
			if(clock > 20)
			{
				clock = 0;
				clip->Encoder->Lock_Radian = clip->Encoder->Radian;
				clip->state = Finish;
			}
		}	
	}
	else{
		clip->Encoder->Lock_Radian = clip->Encoder->Radian;
	}
}

/**
 * @description: 抬升电机的位置环 
 * @param {Motor_t} *lift 电机
 * @param {float} limit   限制
 * @param {u8} dire       方向
 * @param {float} exp     期望角度
 * @note 最后可以封装为一个电机的位置环  未测试
 */
void lift (Motor_t *lift, float limit, u8 dire, float exp ) 
{
	static int16_t clock=0;
	if(dire==1) 
	{
		lift->ExpRadian += 0.6f; 
		if(float_abs(lift->Encoder->Total_Radian - exp - lift->Encoder->Init_Radian ) <= limit) 
		{
			lift->state= Finish ;
			lift->ExpRadian = lift->Encoder->Init_Radian - exp;
		}
	}
	else if(dire==2) {
		lift ->ExpRadian -= 0.4f;
		if (float_abs(lift->Encoder->Total_Radian - lift->Encoder->Init_Radian) <= limit)
		{
			clock++;
		}
		if(clock>=20) {
			lift->ExpRadian = lift->Encoder->Total_Radian;
			lift->state =Finish; 
			clock=0 ;
		}
	}
	else 
	{
		lift->ExpRadian = lift->Encoder->Total_Radian ;
	}
}



/**
 * @description: 抬升
 * @param {*Motor_t *uplift,u8 dire :1 抬升  2 放下}
 * @return {*}
 */
 void uplift(Motor_t *uplift,VL53L0_t *vl53l0,float dis,u8 dire)
{
	
	if(dire == 1)																													//向上
	{
		uplift->ExpSpeed = UpLift_Speed; 																		//初速度赋值
		if(vl53l0->distance - vl53l0->InitDistance >= dis)//结束标志
		{   
			uplift->state = Finish;
			uplift->Encoder->Lock_Radian = uplift->Encoder->Total_Radian;	
		}
	}
	else if(dire == 2)																										//向下
	{
		uplift->ExpSpeed = -UpLift_Speed; //初速度赋值 测试
		if(vl53l0->distance - vl53l0->InitDistance <= dis)									//结束标志
		{     
			uplift->state = Finish;
			uplift->Encoder->Lock_Radian = uplift->Encoder->Total_Radian;
		}
	}
	else
		uplift->ExpSpeed = 0;	
}




/**
 * @description: 伸缩
 * @param {Motor_t} *telescoping
 * @param {u8} dire
 * @return {*}
 * @note 靠堵转试试    2--前伸  1--后伸
 */ 
void Telescoping(Motor_t *telescoping,u8 dire)
{
	static uint8_t clock=0 ;
	if(dire == 1 || dire == 2)
	{
		telescoping->ExpSpeed = (dire*2 - 3)*Telescoping_Speed;
		if (int16_t_abs(telescoping->Encoder->Speed[1]) <= 50)
		{
			clock++;
			if (clock > 10)
			{
				clock = 0;
				telescoping->Encoder->Lock_Radian = telescoping->Encoder->Radian;
				telescoping->state = Finish;
			}
		}
	}
	else
	{
		telescoping->Encoder->Lock_Radian = telescoping->Encoder->Radian;
	}
}




#if 0 
/**
 * @description: 旋转电机
 * @param {Motor_t} *rotate
 * @return {*}
 * @note 只跑速度环
 */
void rotate(Motor_t *rotate) {
	rotate->ExpSpeed = Rotate_Speed; 
	rotate->debug ++ ;
	rotate->ResetFlag++ ;
	if(rotate->ResetFlag >=10) {
		rotate->state=Finish;
	}

}
#endif

void rotate(Motor_t *rotate,float exp, float limit ) {
//	static int16_t clock ;
//	rotate->ExpRadian -= 0.6f ;
//	if (float_abs(rotate->Encoder->Total_Radian - exp - rotate->Encoder->Init_Radian) <= limit) {
//		rotate->state=Finish ;
//		rotate -> ExpRadian = rotate->Encoder->Init_Radian - exp ;
//	}
		rotate->state = Finish ;
}

	/*******************************************************************************************************************************
*名称:	rail
*功能:	导轨
*形参: 	Motor_t *rall,u8 dire
*返回:	无
*说明:	LEFT	RIGHT
*******************************************************************************************************************************/
	void rail(Motor_t *rall, Sensor_t *val, u8 dire)
{
	/*速度赋值*/
	if(dire == 1 || dire == 2){
		rall->ExpSpeed = (dire*2 - 3)*Rall_Speed;
	}
	else{
		rall->SPID.Out = 0;
	}
	/*完成标志*/
	if( (val->Smooth_L == 1) && (val->Smooth_R == 1) ){
		rall->state = Finish;
		rall->Encoder->Lock_Radian = rall->Encoder->Total_Radian;
	}
}


