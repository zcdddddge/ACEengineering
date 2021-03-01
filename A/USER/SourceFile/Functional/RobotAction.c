#include "RobotAction.h"

 
/*************************************************************************************************
*����:	Auto_Ctrl
*����:	�Զ����Ƽ�ȡ
*�β�: 	Gr_t *Gr,u8 box
*����:	��
*˵��:	201-̧��	202-ƽ��	203-����	204-����	205-��ת	206-��ת	207-����
*************************************************************************************************/
void Auto_Ctrl(Gr_t *Gr,u8 box)
{
	static u8 box_lock = 1;
	static u8 boxs = 0;
	u8 i;
	/*���������������¸�ֵ����(��ֹ�����������)*/
	if(box_lock == 1){
		boxs = box;
		box_lock = 2;
		if(boxs != 0)
			Gr->GraspMotor[0].state = DisFinish;//Ԥ��̧��
	}
	/*������ĿΪ1*/
	if(boxs == 1){
		if(Gr->state[0] != 4){
			UPLIFT(&Gr->GraspMotor[0],Gr->vL53L0,10.0f,1);		//��ȡǰ̧��׼��
			if(Gr->GraspMotor[0].state == Finish){
				Gr->GraspMotor[4].ResetFlag=DisFinish;  //1.10 9:26 
				Auto_One_Box(Gr);
			}
		}
		if(Gr->state[0] == 4){
			UPLIFT(&Gr->GraspMotor[0],Gr->vL53L0,1.0f,2);		//��ȡ��Ż�
			if(Gr->GraspMotor[0].state == Finish){
				Gr->GraspMotor[3].state = DisFinish;
				Gr->GraspMotor[4].state = DisFinish;
				Gr->GraspMotor[5].state = DisFinish;
				Gr->GraspMotor[3].ExpSpeed = 0;
				box_lock = 1;
				boxs = 0;
				Gr->state[0] = 0;   
			}
		}
	}
	else if(boxs == 2)/*������ĿΪ2*/
	{
		if(Gr->state[1] != 4){
			UPLIFT(&Gr->GraspMotor[0],Gr->vL53L0,13.5f,1);		//��ȡǰ̧��׼��
			if(Gr->GraspMotor[0].state == Finish){
				Auto_Two_Box(Gr);	
			}
		}
		else if(Gr->state[1] == 4)
		{
			UPLIFT(&Gr->GraspMotor[0],Gr->vL53L0,4.5f,2);		//��ȡ��Ż�
			if(Gr->GraspMotor[0].state == Finish)
			{
				Gr->GraspMotor[1].ExpSpeed = 0;							//ƽ���ٶ���0
				Gr->GraspMotor[1].state = DisFinish;				//ƽ�Ʊ��Ϊδ���
				Gr->GraspMotor[3].ExpSpeed = 0;
				Gr->GraspMotor[3].state = DisFinish;
				Gr->GraspMotor[4].state = DisFinish;
				Gr->GraspMotor[5].state = DisFinish;
				box_lock = 1;                             // ���ӽ��� 
				boxs = 0;
				Gr->state[1] = 0;
				//Gr->state[3] = 7;														 //��ʱ�����жϽ���
			}
		}
	}
	else if(boxs == 0)
	{
		box_lock = 1;
	}
	
	/*̧��-ƽ��-����-����*/
	for(i = 0;i < 4;i ++)
	{
		if(Gr->GraspMotor[i].state == DisFinish)																				//δ������ٶȻ�
		{
			PID_DEAL(&Gr->GraspMotor[i].SPID,Gr->GraspMotor[i].ExpSpeed,Gr->GraspMotor[i].Encoder->Speed[1]); 
		}
		else if(Gr->GraspMotor[i].state == Finish)																			//������λ�û�
		{
			PID_DEAL(&Gr->GraspMotor[i].PPID,Gr->GraspMotor[i].Encoder->Lock_Radian,Gr->GraspMotor[i].Encoder->Total_Radian);										//�⻷
			PID_DEAL(&Gr->GraspMotor[i].SPID,Gr->GraspMotor[i].PPID.Out,Gr->GraspMotor[i].Encoder->Speed[1]);																		//�ڻ�
		}
		else
		{
			Gr->GraspMotor[i].SPID.Out = 0;
		}
	}	
	/*��ת*/
	PID_DEAL(&Gr->GraspMotor[4].PPID,Gr->GraspMotor[4].ExpRadian,Gr->GraspMotor[4].Encoder->Total_Radian);
	PID_DEAL(&Gr->GraspMotor[5].PPID,Gr->GraspMotor[5].ExpRadian,Gr->GraspMotor[5].Encoder->Total_Radian);
	PID_DEAL(&Gr->GraspMotor[4].SPID,Gr->GraspMotor[4].PPID.Out,Gr->GraspMotor[4].Encoder->Speed[1]);
	PID_DEAL(&Gr->GraspMotor[5].SPID,Gr->GraspMotor[5].PPID.Out,Gr->GraspMotor[5].Encoder->Speed[1]);
	
	Gr->Can_Send_Grasp_1(MotorOutput_201_204);
	Gr->Can_Send_Grasp_2(MotorOutput_205_208);
}


/*******************************************************************************************************************************
*����:	FLIP
*����:	��ת
*�β�: 	Motor_t *filp1,Motor_t *filp2,float exp,float limit,u8 dire
*����:	��
*˵��:	ʹ�õ��ת�����ԽǶ�			
-160.0f,20.0f,2*******************************************************************************************************************************/
static void FLIP(Motor_t *filp1,Motor_t *filp2,float exp,float limit,u8 dire)
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

/*******************************************************************************************************************************
*����:	CLIP
*����:	����
*�β�: 	Motor_t *clip	u8 streng
*����:	��
*˵��:	1 �н�  2  �ɿ�
*******************************************************************************************************************************/
static void CLIP(Motor_t *clip,int16_t exp,u8 dire)
{
	static int16_t clock = 0;
	if(dire == 1 || dire == 2)
	{
		clip->ExpSpeed = (3 - dire*2)*exp;   
		if(int16_t_abs(clip->Encoder->Speed[1]) <= 50){/*�����ж�*/
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



/*******************************************************************************************************************************
*����:	RAIL
*����:	����
*�β�: 	Motor_t *rall,u8 dire
*����:	��
*˵��:	LEFT	RIGHT
*******************************************************************************************************************************/
static void RAIL(Motor_t *rall,Sensor_t*val,u8 dire)
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



/*******************************************************************************************************************************
*����:	UPLIFT
*����:	̧��
*�β�: 	Motor_t *uplift,u8 dire
*����:	��
*˵��:	UP  DOWM
*******************************************************************************************************************************/
 
static void UPLIFT(Motor_t *uplift,VL53L0_t *vl53l0,float dis,u8 dire)
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
	else//��������
		uplift->ExpSpeed = 0;	
}




/*******************************************************************************************************************************
*����:	Telescoping
*����:	����
*�β�: 	Motor_t *uplift,u8 dire
*����:	��
*˵��:	FORWARD	REVERSE
*******************************************************************************************************************************/
static void Telescoping(Motor_t *telescoping,u8 dire)
{
	if(dire == 1 || dire == 2)
	{
		telescoping->ExpSpeed = (dire*2 - 3)*Telescoping_Speed;
		/*�ڻ�*/
		PID_DEAL(&telescoping->SPID,telescoping->ExpSpeed,telescoping->Encoder->Speed[1]);
	}
	else
	{
		/*�⻷*/
		PID_DEAL_OVERSHOOT(&telescoping->PPID,telescoping->Encoder->Lock_Radian,telescoping->Encoder->Radian);
		/*�ڻ�*/
		PID_DEAL(&telescoping->SPID,telescoping->PPID.Out,telescoping->Encoder->Speed[1]);
	}
}


/*************************************************************************************************
*����:	Auto_One_Box
*����:	�Զ����Ƽ�ȡһ��
*�β�: 	Gr_t *Gr
*����:	��
*˵��:	201-̧��	202-ƽ��	203-����	204-����	205-��ת	206-��ת	207-����
*************************************************************************************************/
static void Auto_One_Box(Gr_t *Gr)
{
	switch (Gr->state[0])
	{
		/*ǰ��*/
		case 0:
		{
			FLIP(&Gr->GraspMotor[4],&Gr->GraspMotor[5],182.0f,6.0f,1);  // 180-->182  
			if(Gr->GraspMotor[4].state == Finish && Gr->GraspMotor[5].state == Finish)
			{
				Gr->GraspMotor[3].ExpSpeed = 0;
				Gr->GraspMotor[3].state = DisFinish;					//���ӱ�־Ϊδ���
				Gr->state[0] = 1;
			}
			break;
		}
		/*�н�*/
		case 1:
		{
			CLIP(&Gr->GraspMotor[3],Cilp_Speed,1);
			if(Gr->GraspMotor[3].state == Finish)
			{
				Gr->GraspMotor[4].state = DisFinish;					//��ת���Ϊδ���
				Gr->GraspMotor[5].state = DisFinish;
				Gr->state[0] = 2;
			}
			break;
		}
		/*��*/
		case 2:
		{
			FLIP(&Gr->GraspMotor[4],&Gr->GraspMotor[5],-160.0f,20.0f,2);   // -170-->-160  ���
			if(Gr->GraspMotor[4].state == Finish && Gr->GraspMotor[5].state == Finish)
			{
				Gr->GraspMotor[3].ExpSpeed = 0;
				Gr->GraspMotor[3].state = DisFinish;					//���ӱ�־Ϊδ���
				Gr->state[0] = 3;
			}
			break;
		}
		/*�ɼ�*/
		case 3:
		{
			CLIP(&Gr->GraspMotor[3],Cilp_Speed,2);
			if(Gr->GraspMotor[3].state == Finish)
			{
				Gr->GraspMotor[3].ExpSpeed = 0;
				Gr->GraspMotor[3].state = DisFinish;				//���ӱ��Ϊδ���
				Gr->GraspMotor[0].state = DisFinish;				//̧�����Ϊδ���
				Gr->state[0] = 4;
			}
			break;
		}
		default:
		{ 
			break;
		}
			
	}
}



static void Auto_Two_Box(Gr_t *Gr)
{
	switch (Gr->state[1])
	{
		/*ƽ��*/
		case 0:
		{
			RAIL(&Gr->GraspMotor[1],Gr->sensor,1);
			if(Gr->GraspMotor[1].state == Finish)
			{
				Gr->GraspMotor[4].state = DisFinish;					//��ת���Ϊδ���
				Gr->GraspMotor[5].state = DisFinish;
				Gr->state[0] = 0;															//��ȡһ��Ϊ���¿�ʼ
				Gr->state[1] = 1;
			}
			break;
		}
		/*��ȡһ��*/
		case 1:
		{
			Auto_One_Box(Gr);
			if(Gr->state[0] == 4)
			{
				Gr->GraspMotor[0].state = Finish;						//̧�����Ϊ���
				Gr->GraspMotor[1].state = DisFinish;				//ƽ�Ʊ��Ϊδ���
				Gr->state[1] = 2;
			}
			break;
		}
		/*ƽ��*/
		case 2:
		{
			RAIL(&Gr->GraspMotor[1],Gr->sensor,1);
			if(Gr->GraspMotor[1].state == Finish)
			{
				Gr->GraspMotor[4].state = DisFinish;					//��ת���Ϊδ���
				Gr->GraspMotor[5].state = DisFinish;
				Gr->state[0] = 0;															//��ȡһ��Ϊ���¿�ʼ
				Gr->state[1] = 3;
			}
			break;
		}
		/*��ȡһ��*/
		case 3:
		{
			Auto_One_Box(Gr);
			if(Gr->state[0] == 4)
			{
				Gr->GraspMotor[0].state = DisFinish;						//̧�����Ϊ���
				Gr->state[0] = 0;
				Gr->state[1] = 4;
			}
			break;
		}
		default:		
			break;

	}
}




/**
 * @description: ��ȡʧ�ܸ�λ:���ǵ�һ�㶼��ǰ��ʱ������-ִֻ�к�,������̧��,û�иı�����״̬
 * @param {Gr_t} *Gr
 * @return {*}
 */
void ResetAction(Gr_t *Gr) {

	filpReset(&Gr->GraspMotor[4],&Gr->GraspMotor[5],20) ;

	if(Gr->GraspMotor[0].state==Finish) {
		PID_DEAL(&Gr->GraspMotor[0].PPID,Gr->GraspMotor[0].Encoder->Lock_Radian,Gr->GraspMotor[0].Encoder->Total_Radian);										//�⻷
		PID_DEAL(&Gr->GraspMotor[0].SPID,Gr->GraspMotor[0].PPID.Out,Gr->GraspMotor[0].Encoder->Speed[0]);		
	}

	PID_DEAL(&Gr->GraspMotor[4].PPID,Gr->GraspMotor[4].ExpRadian,Gr->GraspMotor[4].Encoder->Total_Radian);
	PID_DEAL(&Gr->GraspMotor[5].PPID,Gr->GraspMotor[5].ExpRadian,Gr->GraspMotor[5].Encoder->Total_Radian);
	PID_DEAL(&Gr->GraspMotor[4].SPID,Gr->GraspMotor[4].PPID.Out,Gr->GraspMotor[4].Encoder->Speed[1]);
	PID_DEAL(&Gr->GraspMotor[5].SPID,Gr->GraspMotor[5].PPID.Out,Gr->GraspMotor[5].Encoder->Speed[1]);
	
	Gr->Can_Send_Grasp_1(MotorOutput_201_204);
	Gr->Can_Send_Grasp_2(MotorOutput_205_208);

}

static void filpReset(Motor_t *  filp1 , Motor_t *filp2 ,float limit ) {
	static int16_t clock[2] = {0,0} ;

	if(filp1->ResetFlag == DisFinish) {
		filp1->ExpRadian -=0.4f ;
		filp2->ExpRadian +=0.4f; 

		/*��ɱ�־*/
		if(float_abs(filp1->Encoder->Total_Radian - filp1->Encoder->Init_Radian) <= limit){ 
			clock[0] ++;
		}
		if(float_abs(filp2->Encoder->Total_Radian - filp2->Encoder->Init_Radian) <= limit){     
			clock[1] ++;
		}
		if(clock[0] >= 10 && clock[1] >= 10){
			filp1->ExpRadian = filp1->Encoder->Total_Radian;    
			filp2->ExpRadian = filp2->Encoder->Total_Radian;  
			filp1->ResetFlag = Finish;
			filp2->ResetFlag= Finish;
			clock[0] = 0;
			clock[1] = 0;
		}
	}
	else {
		filp1->ExpRadian = filp1->Encoder->Total_Radian;
		filp2->ExpRadian = filp2->Encoder->Total_Radian;
	}


}

