/*
 * @Date: 2021-02-24 11:34:45
 * @LastEditTime: 2021-04-01 06:58:35
 * @LastEditors: wingchi-leung 
 * @Description: ���̼�ȡ��������Ŀ����߼�
 */

#include "MotorAction.h"

/*���԰�:3.27 ����ͨ��*/
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
		/*��ɱ�־*/
		// ƽ�У� -160 + exp -6 <=6  ===> exp = 172 
		// ��ֱ�� -225 + exp -6 <=6  ===> exp = 237 
		if(float_abs(filp1->Encoder->Total_Radian + exp - filp1->Encoder->Init_Radian) <= limit)
		{          
			filp1->state = Finish; 
			filp1->ExpRadian = filp1->Encoder->Init_Radian - exp;
		}
	
	}
	else if (dire == 2)
	{
		filp1->ExpRadian += 0.4f;
		

		/*��ɱ�־*/
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
  * @description: ʹ�õ��ת�����ԽǶ�	
  * @param {Motor_t} *filp1
  * @param {Motor_t} *filp2
  * @param {float} exp
  * @param {float} limit
  * @param {u8} dire 1-ǰ��  2-��
  * @return {*}
  */
void flip(Motor_t *filp1, Motor_t *filp2, float exp, float limit, u8 dire)
{
	static int16_t clock[2] = {0,0};
	if(dire == 1)
	{
		filp1->ExpRadian += 0.6f;   
		filp2->ExpRadian -= 0.6f;
		/*����*/
		if(filp1->ExpRadian >= filp1->Encoder->Init_Radian + exp)
		{
				filp1->ExpRadian = filp1->Encoder->Init_Radian + exp;
		}
		if(filp2->ExpRadian <= filp2->Encoder->Init_Radian - exp)
		{
				filp2->ExpRadian = filp2->Encoder->Init_Radian - exp;
		}
		/*��ɱ�־*/
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
		
		/*��ɱ�־*/
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
  * @description: ����    
  * @param {Motor_t} *clip 
  * @param {int16_t} exp  : expect Speed 
  * @param {u8} dire      : 1 �н�   2 �ɿ�
  * @return {*}
  */
void clip(Motor_t *clip,int16_t exp,u8 dire)
{
	static int16_t clock = 0;
	clip->debug ++ ;
	if(dire == 1 || dire == 2)
	{
		clip->ExpSpeed = (3 - dire*2)*exp;
		/*�����ж�*/
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
 * @description: ̧�������λ�û� 
 * @param {Motor_t} *lift ���
 * @param {float} limit   ����
 * @param {u8} dire       ����
 * @param {float} exp     �����Ƕ�
 * @note �����Է�װΪһ�������λ�û�  δ����
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
 * @description: ̧��
 * @param {*Motor_t *uplift,u8 dire :1 ̧��  2 ����}
 * @return {*}
 */
 void uplift(Motor_t *uplift,VL53L0_t *vl53l0,float dis,u8 dire)
{
	
	if(dire == 1)																													//����
	{
		uplift->ExpSpeed = UpLift_Speed; 																		//���ٶȸ�ֵ
		if(vl53l0->distance - vl53l0->InitDistance >= dis)//������־
		{   
			uplift->state = Finish;
			uplift->Encoder->Lock_Radian = uplift->Encoder->Total_Radian;	
		}
	}
	else if(dire == 2)																										//����
	{
		uplift->ExpSpeed = -UpLift_Speed; //���ٶȸ�ֵ ����
		if(vl53l0->distance - vl53l0->InitDistance <= dis)									//������־
		{     
			uplift->state = Finish;
			uplift->Encoder->Lock_Radian = uplift->Encoder->Total_Radian;
		}
	}
	else
		uplift->ExpSpeed = 0;	
}




/**
 * @description: ����
 * @param {Motor_t} *telescoping
 * @param {u8} dire
 * @return {*}
 * @note ����ת����    2--ǰ��  1--����
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
 * @description: ��ת���
 * @param {Motor_t} *rotate
 * @return {*}
 * @note ֻ���ٶȻ�
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
*����:	rail
*����:	����
*�β�: 	Motor_t *rall,u8 dire
*����:	��
*˵��:	LEFT	RIGHT
*******************************************************************************************************************************/
	void rail(Motor_t *rall, Sensor_t *val, u8 dire)
{
	/*�ٶȸ�ֵ*/
	if(dire == 1 || dire == 2){
		rall->ExpSpeed = (dire*2 - 3)*Rall_Speed;
	}
	else{
		rall->SPID.Out = 0;
	}
	/*��ɱ�־*/
	if( (val->Smooth_L == 1) && (val->Smooth_R == 1) ){
		rall->state = Finish;
		rall->Encoder->Lock_Radian = rall->Encoder->Total_Radian;
	}
}


