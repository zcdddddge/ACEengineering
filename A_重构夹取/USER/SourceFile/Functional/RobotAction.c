/*
 * @Date: 2021-02-24 11:38:12
 * @LastEditTime: 2021-03-01 21:51:44
 * @LastEditors: Please set LastEditors
 * @Description: �����˹����Զ���ȡ���ƺ���,�ص���״̬���� 
 */

#include "RobotAction.h"



/**
 * @description: �Զ���ȡ��������,�ص�����״̬ 
 * @param {Gr_t} *Gr
 * @param {u8} box ������
 * @return {*}
 * @note 201-̧��	202-ƽ��	203-����	204-����	205-��ת	206-��ת	207-����
 */
void Auto_Ctrl(Gr_t *Gr,u8 box)
{
	static u8 box_lock = 1;
	static u8 boxs = 0;
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
				Auto_One_Box(Gr);
			}
		}
		else if(Gr->state[0] == 4){
			UPLIFT(&Gr->GraspMotor[0],Gr->vL53L0,1.0f,2);		//��ȡ��Ż�
			if(Gr->GraspMotor[0].state == Finish){
				Gr->GraspMotor[3].state = DisFinish;
				Gr->GraspMotor[4].state = DisFinish;
				Gr->GraspMotor[5].state = DisFinish;
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
																	 //��ʱ�����жϽ���
			}
		}
	}
	else if(boxs == 0)
	{
		box_lock = 1;
	}
	PID_CALA(Gr);
}








/**
 * @description: �Զ���ȡһ��
 * @param {*}
 * @return {*}
 */
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


static void PID_CALA(Gr_t *Gr)
{
	u8 i =0 ;
	for (i = 0; i < 4; i++)
	{
		if (Gr->GraspMotor[i].state == DisFinish) //δ������ٶȻ�
		{
			PID_DEAL(&Gr->GraspMotor[i].SPID, Gr->GraspMotor[i].ExpSpeed, Gr->GraspMotor[i].Encoder->Speed[1]);
		}
		else if (Gr->GraspMotor[i].state == Finish) //������λ�û�
		{
			PID_DEAL(&Gr->GraspMotor[i].PPID, Gr->GraspMotor[i].Encoder->Lock_Radian, Gr->GraspMotor[i].Encoder->Total_Radian); //�⻷
			PID_DEAL(&Gr->GraspMotor[i].SPID, Gr->GraspMotor[i].PPID.Out, Gr->GraspMotor[i].Encoder->Speed[1]);					//�ڻ�
		}
		else
		{
			Gr->GraspMotor[i].SPID.Out = 0;
		}
	}
	PID_DEAL(&Gr->GraspMotor[4].PPID, Gr->GraspMotor[4].ExpRadian, Gr->GraspMotor[4].Encoder->Total_Radian);
	PID_DEAL(&Gr->GraspMotor[5].PPID, Gr->GraspMotor[5].ExpRadian, Gr->GraspMotor[5].Encoder->Total_Radian);
	PID_DEAL(&Gr->GraspMotor[4].SPID, Gr->GraspMotor[4].PPID.Out, Gr->GraspMotor[4].Encoder->Speed[1]);
	PID_DEAL(&Gr->GraspMotor[5].SPID, Gr->GraspMotor[5].PPID.Out, Gr->GraspMotor[5].Encoder->Speed[1]);

	Gr->Can_Send_Grasp_1(MotorOutput_201_204);
	Gr->Can_Send_Grasp_2(MotorOutput_205_208);
}





/**
 * @description: ��ȡʧ�ܸ�λ:���ǵ�һ�㶼��ǰ��ʱ������-ִֻ�к�,������̧��,û�иı�����״̬
 * @param {Gr_t} *Gr
 * @return {*}
 * @note 3/1 ������
 */
void ResetAction(Gr_t *Gr)
{

	FLIP(&Gr->GraspMotor[4], &Gr->GraspMotor[5], -160.0f, 20.0f, 2); 

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





#if 0
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
#endif 



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
