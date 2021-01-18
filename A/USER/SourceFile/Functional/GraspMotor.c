#include "GraspMotor.h"
/*************************************************************************************************
*����:	Grasp_Motor_Init
*����:	��ȡ�����ֳ�ʼ��
*�β�: 	Gr_t *Gr
*����:	��
*˵��:	201-̧��	202-ƽ��	203-����	204-����	205-��ת	206-��ת	207-����
*************************************************************************************************/
void Grasp_Motor_Init(Gr_t *Gr)
{		
		u8 i = 0;
		float Spid[7][3] = {
			{GRASP_SLIDE_S_P,GRASP_SLIDE_S_I,GRASP_SLIDE_S_D},
			{GRASP_SMOOTH_S_P,GRASP_SMOOTH_S_I,GRASP_SMOOTH_S_D},
			{GRASP_TRANS_S_P,GRASP_TRANS_S_I,GRASP_TRANS_S_D},
			{GRASP_CLAMP_S_P,GRASP_CLAMP_S_I,GRASP_CLAMP_S_D},			
			{GRASP_SWING_S_P,GRASP_SWING_S_I,GRASP_SWING_S_D},  
		  {GRASP_SWING_S_P,GRASP_SWING_S_I,GRASP_SWING_S_D},
			{GRASP_SUPPLY_S_P,GRASP_SUPPLY_S_I,GRASP_SUPPLY_S_D}
		};
		float Ppid[7][3] = {
			{GRASP_SLIDE_P_P,GRASP_SLIDE_P_I,GRASP_SLIDE_P_D},
			{GRASP_SMOOTH_P_P,GRASP_SMOOTH_S_I,GRASP_SMOOTH_P_D},
			{GRASP_TRANS_P_P,GRASP_TRANS_P_I,GRASP_TRANS_P_D},
			{GRASP_CLAMP_P_P,GRASP_CLAMP_P_I,GRASP_CLAMP_P_D},			
			{GRASP_SWING_P_P,GRASP_SWING_P_I,GRASP_SWING_P_D},  
		  {GRASP_SWING_P_P,GRASP_SWING_P_I,GRASP_SWING_P_D},
			{GRASP_SUPPLY_P_P,GRASP_SUPPLY_P_I,GRASP_SUPPLY_P_D}
   };
		
		/*����ӳ��*/
		Gr->Can_Send_Grasp_1			= CAN1_201_To_204_SEND;
		Gr->Can_Send_Grasp_2	 		= CAN1_205_To_208_SEND;
		Gr->Get_Encoder						=	Return_Can1_201_208_Encoder;
		Gr->Get_Sensor_t					= Return_Sensor_t;
	  Gr->Get_VL53L0_t					= Return_VL53L0_t;
		
		for(i = 0;i < 7;i ++)
		{	
				/*���㴦��*/
				MotorValZero(&Gr->GraspMotor[i]);
				/*ID��ֵ*/
				Gr->GraspMotor[i].ID = i + 1;
				/*���̸�ֵ*/
				Gr->GraspMotor[i].Encoder = Gr->Get_Encoder(i+1);
				PID_INIT(&Gr->GraspMotor[i].PPID,Ppid[i][0],Ppid[i][1],Ppid[i][2],0,8500);							//λ�û���ʼ��
				PID_INIT(&Gr->GraspMotor[i].SPID,Spid[i][0],Spid[i][1],Spid[i][2],0,16000);							//�ٶȻ���ʼ��
				Gr->GraspMotor[i].ExpRadian							= Gr->GraspMotor[i].Encoder->Radian;						//��ʼ�������Ƕ�
				Gr->GraspMotor[i].Encoder->Init_Radian 	= Gr->GraspMotor[i].Encoder->Radian;    				//��ʼ����ֵ��ֵ
				Gr->GraspMotor[i].Encoder->Lock_Radian 	= Gr->GraspMotor[i].Encoder->Radian;	    			//��ʼ�������Ƕ�
				Gr->GraspMotor[i].Encoder->Total_Radian = Gr->GraspMotor[i].Encoder->Radian;	    			//��ʼ���ܽǶ�
				Gr->GraspMotor[i].Radio = 19;																														//��ʼ����ȡ������ٱ�
				Gr->GraspMotor[i].ResetFlag =DisFinish ;
		}
				PID_INIT(&Gr->GraspMotor[2].PPID,Ppid[2][0],Ppid[2][1],Ppid[2][2],0,14700);							//λ�û���ʼ��
				PID_INIT(&Gr->GraspMotor[6].PPID,Ppid[6][0],Ppid[6][1],Ppid[6][2],0,14700);							//λ�û���ʼ��
		
				Gr->GraspMotor[0].Radio = 27;																														//��ʼ��̧��������ٱ�
				Gr->GraspMotor[2].Radio = 36;																														//��ʼ������������ٱ�
				Gr->GraspMotor[3].Radio = 27;																														//��ʼ����ȡ������ٱ�
				Gr->GraspMotor[6].Radio = 36;																														//��ʼ������������ٱ�
				
			
		
				/*״ֵ̬��ʼ��*/
				Gr->state[0] = Gr->state[1] = Gr->state[2] = Gr->state[3] = DisFinish;
				/*��λֵ��ʼ��*/
				Gr->reset[0] = Gr->reset[1] = Gr->reset[2] = Gr->reset[3] \
					= Gr->reset[4] = Gr->reset[5] = Gr->reset[6] = DisFinish;
		
				/*��������ʼ��*/
				Gr->sensor	= Gr->Get_Sensor_t();
				Gr->sensor->Smooth_L = Gr->sensor->Smooth_R = Gr->sensor->Stretch_F = 2;
		
				/*VL53L0����ʼ��*/
				Gr->vL53L0 = Gr->Get_VL53L0_t();
				
}



/*************************************************************************************************
*����:	RC_Ctrl
*����:	ң�ؿ���
*�β�: 	Gr_t *Gr,RC_ctrl_t *rc
*����:	��
*˵��:	201-̧��	202-ƽ��	203-����	204-����	205-��ת	206-��ת	207-����
*************************************************************************************************/
void RC_Ctrl(Gr_t *Gr,RC_ctrl_t *rc)
{
	static int16_t clock = 0; 
	static int8_t dire = 0;
	static int8_t lock = 1;

	//Gr->GraspMotor[0].ExpSpeed   = ((float)(rc->ch1) * 10.0f);
	Gr->GraspMotor[1].ExpSpeed   = ((float)(rc->ch0) * 10.0f);
	Gr->GraspMotor[2].ExpSpeed   = ((float)(rc->ch3) * 10.0f);
  Gr->GraspMotor[3].ExpSpeed   = ((float)(rc->ch2) * 10.0f);
	Gr->GraspMotor[4].ExpRadian += ((float)(rc->sw) / 800.0f);
	Gr->GraspMotor[5].ExpRadian += ((float)(-rc->sw) / 800.0f);
	Gr->GraspMotor[6].ExpSpeed  = ((float)(rc->ch1) * 10.0f);
	
	
	/****************************̧��***************************/
	if(Gr->GraspMotor[0].ExpSpeed == 0)
	{
		/*�⻷*/
		PID_DEAL(&Gr->GraspMotor[0].PPID,Gr->GraspMotor[0].Encoder->Init_Radian,Gr->GraspMotor[0].Encoder->Total_Radian);
		/*�ڻ�*/
		PID_DEAL(&Gr->GraspMotor[0].SPID,Gr->GraspMotor[0].PPID.Out,Gr->GraspMotor[0].Encoder->Speed[1]);
	}
	else
	{
			PID_DEAL(&Gr->GraspMotor[0].SPID,Gr->GraspMotor[0].ExpSpeed,Gr->GraspMotor[0].Encoder->Speed[1]);
			Gr->GraspMotor[0].Encoder->Init_Radian = Gr->GraspMotor[0].Encoder->Total_Radian;
	}
	
	/***************************ƽ��**************************/
	if(Gr->GraspMotor[1].ExpSpeed == 0)
	{
		/*�⻷*/
		PID_DEAL(&Gr->GraspMotor[1].PPID,Gr->GraspMotor[1].Encoder->Init_Radian,Gr->GraspMotor[1].Encoder->Total_Radian);
		/*�ڻ�*/
		PID_DEAL(&Gr->GraspMotor[1].SPID,Gr->GraspMotor[1].PPID.Out,Gr->GraspMotor[1].Encoder->Speed[1]);
	}
	else
	{
			PID_DEAL(&Gr->GraspMotor[1].SPID,Gr->GraspMotor[1].ExpSpeed,Gr->GraspMotor[1].Encoder->Speed[1]);
			Gr->GraspMotor[1].Encoder->Init_Radian = Gr->GraspMotor[1].Encoder->Total_Radian;
	}
	
	/***************************����**************************/
	if(Gr->GraspMotor[2].ExpSpeed == 0)
	{
		/*�⻷*/
		PID_DEAL(&Gr->GraspMotor[2].PPID,Gr->GraspMotor[2].Encoder->Init_Radian,Gr->GraspMotor[2].Encoder->Total_Radian);
		/*�ڻ�*/
		PID_DEAL(&Gr->GraspMotor[2].SPID,Gr->GraspMotor[2].PPID.Out,Gr->GraspMotor[2].Encoder->Speed[1]);
	}
	else
	{
			PID_DEAL(&Gr->GraspMotor[2].SPID,Gr->GraspMotor[2].ExpSpeed,Gr->GraspMotor[2].Encoder->Speed[1]);
			Gr->GraspMotor[2].Encoder->Init_Radian = Gr->GraspMotor[2].Encoder->Total_Radian;
	}	
	/************************����*******************************/
	if(Gr->GraspMotor[6].ExpSpeed == 0)
	{
		/*�⻷*/
		PID_DEAL(&Gr->GraspMotor[6].PPID,Gr->GraspMotor[6].Encoder->Init_Radian,Gr->GraspMotor[6].Encoder->Total_Radian);
		/*�ڻ�*/
		PID_DEAL(&Gr->GraspMotor[6].SPID,Gr->GraspMotor[6].PPID.Out,Gr->GraspMotor[6].Encoder->Speed[1]);
	}
	else
	{
		PID_DEAL(&Gr->GraspMotor[6].SPID,Gr->GraspMotor[6].ExpSpeed,Gr->GraspMotor[6].Encoder->Speed[1]);
		Gr->GraspMotor[6].Encoder->Init_Radian = Gr->GraspMotor[6].Encoder->Total_Radian;
	}

	#if 0 
	if((Gr->GraspMotor[6].Encoder->Speed[1] <= 100) && (Gr->GraspMotor[6].Encoder->Speed[1] >= -100) && (Gr->GraspMotor[6].ExpSpeed != 0)){
		Gr->GraspMotor[6].clock++ ;
		if(Gr->GraspMotor[6].clock>=50) {
			if(Gr->GraspMotor[6].clock == 50) {
				Gr->GraspMotor[6].lock=2 ;
				if(Gr->GraspMotor[6].ExpSpeed > 0) 
				{
					 Gr->GraspMotor[6].dire=1 ;
				}
				else 
				{
					Gr->GraspMotor[6].dire =-1 ;
				}
			}
			if((Gr->GraspMotor[6].dire == 1 && Gr->GraspMotor[3].ExpSpeed < 0) || (Gr->GraspMotor[6].dire == -1 && Gr->GraspMotor[6].ExpSpeed > 0) )								//����ֵ�෴��ֹͣ��ת����
			{
				Gr->GraspMotor[6].clock = 0;																																																				//����ֹͣ����ת�ж�������0
				Gr->GraspMotor[6].lock= 1;
			}

		}
	}
	
	if(Gr->GraspMotor[6].lock==2) {
		Gr->GraspMotor[6].SPID.Out=0 ;
	}else {
		PID_DEAL(&Gr->GraspMotor[6].SPID,Gr->GraspMotor[6].ExpSpeed,Gr->GraspMotor[6].Encoder->Speed[1]);
	}
	#endif 
	
	
	
	
	
	/**����**/
	/*�趨ֵ��Ϊ0�������ٶ�С��50(��ת)*/
	if((Gr->GraspMotor[3].Encoder->Speed[1] <= 100) && (Gr->GraspMotor[3].Encoder->Speed[1] >= -100) && (Gr->GraspMotor[3].ExpSpeed != 0))
	{
		clock ++;
		if(clock >= 50)
		{
			if(clock == 50)					//�����Ƕȳ�ʼ��
			{
				Gr->GraspMotor[3].Encoder->Lock_Radian = Gr->GraspMotor[3].Encoder->Total_Radian;
				lock = 2;
				if(Gr->GraspMotor[3].ExpSpeed > 0)									//�����ٶȷ����ж�
					dire = 1;
				else 
					dire = -1;
			}
			if( (dire == 1 && Gr->GraspMotor[3].ExpSpeed < 0) || (dire == -1 && Gr->GraspMotor[3].ExpSpeed > 0) )								//����ֵ�෴��ֹͣ��ת����
			{
				clock = 0;																																																				//����ֹͣ����ת�ж�������0
				lock = 1;
			}																																																			
		}
	}
	/*�趨ֵΪ0������ֹͣ*/
	if(lock == 2)
	{
		PID_DEAL(&Gr->GraspMotor[3].PPID,Gr->GraspMotor[3].Encoder->Lock_Radian,Gr->GraspMotor[3].Encoder->Total_Radian);	//�⻷
		PID_DEAL(&Gr->GraspMotor[3].SPID,Gr->GraspMotor[3].PPID.Out,Gr->GraspMotor[3].Encoder->Speed[1]);									//�ڻ�
	}
	/*�趨ֵ��Ϊ0����ʼ����*/
	else if(lock == 1)
	{
			PID_DEAL(&Gr->GraspMotor[3].SPID,Gr->GraspMotor[3].ExpSpeed,Gr->GraspMotor[3].Encoder->Speed[1]);
			Gr->GraspMotor[3].Encoder->Lock_Radian = Gr->GraspMotor[3].Encoder->Total_Radian;
	}
	
	/**��ת**/
	/*�⻷*/
	PID_DEAL(&Gr->GraspMotor[4].PPID,Gr->GraspMotor[4].ExpRadian,Gr->GraspMotor[4].Encoder->Total_Radian);
	/*�ڻ�*/
	PID_DEAL(&Gr->GraspMotor[4].SPID,Gr->GraspMotor[4].PPID.Out,Gr->GraspMotor[4].Encoder->Speed[1]);
	/*�⻷*/
	PID_DEAL(&Gr->GraspMotor[5].PPID,Gr->GraspMotor[5].ExpRadian,Gr->GraspMotor[5].Encoder->Total_Radian);
	/*�ڻ�*/
	PID_DEAL(&Gr->GraspMotor[5].SPID,Gr->GraspMotor[5].PPID.Out,Gr->GraspMotor[5].Encoder->Speed[1]);
	
	Gr->GraspMotor[4].SPID.Out=0;
	Gr->Can_Send_Grasp_1(MotorOutput_201_204);
	Gr->Can_Send_Grasp_2(MotorOutput_205_208);
}


/*************************************************************************************************
*����:	ResetGrasp
*����:	��λ��ȡ
*�β�: 	Gr_t *Gr
*����:	��
*˵��:	201-̧��	202-ƽ��	203-����	204-����	205-��ת	206-��ת	207-����
				��ʱ���ԣ�ʵ���Ͽ��ܲ���ȡ����λ��ֱ�ӷ�0
*************************************************************************************************/
void ResetGrasp(Gr_t *Gr)
{
	static int16_t clock[7] = {0,0,0,0,0,0,0};
	/*̧�������λ*/
	if(Gr->reset[0] == DisFinish)
	{
		Gr->GraspMotor[0].SPID.Out = 0;
		if( (Gr->GraspMotor[0].Encoder->Speed[1] <= 5) && (Gr->GraspMotor[0].Encoder->Speed[1] >= -5) )
		{ //��ɱ�־ 
			clock[0] ++;    
			if(clock[0] >= 100)
			{
				clock[0] = 0;
				Gr->reset[7] ++;   
				Gr->reset[0] = Finish;
			}
		}
	}
	/*ƽ�Ƶ����λ*/
	if(Gr->reset[1] == DisFinish)
	{  
		Gr->GraspMotor[1].ExpSpeed = 0;
		if( (Gr->GraspMotor[1].Encoder->Speed[1] <= 50) && (Gr->GraspMotor[1].Encoder->Speed[1] >= -50) )
		{
			clock[1] ++;
			if(clock[1] >= 4)
			{
				clock[1] = 0; 
				Gr->reset[7] ++;
				Gr->reset[1] = Finish;
				Gr->GraspMotor[1].Encoder->Lock_Radian = Gr->GraspMotor[1].Encoder->Total_Radian;
				Gr->GraspMotor[1].Encoder->Init_Radian = Gr->GraspMotor[1].Encoder->Total_Radian;
				Gr->GraspMotor[1].ExpRadian 					 = Gr->GraspMotor[1].Encoder->Total_Radian;
			}
		}
		PID_DEAL(&Gr->GraspMotor[1].SPID,Gr->GraspMotor[1].ExpSpeed,Gr->GraspMotor[1].Encoder->Speed[1]);
	}
	else{
		PID_DEAL(&Gr->GraspMotor[1].PPID,Gr->GraspMotor[1].ExpRadian,Gr->GraspMotor[1].Encoder->Radian);
		PID_DEAL(&Gr->GraspMotor[1].SPID,Gr->GraspMotor[1].PPID.Out,Gr->GraspMotor[1].Encoder->Speed[1]);
	}
	/*���ӵ����λ*/
	if(Gr->reset[3] == DisFinish)
	{
		Gr->GraspMotor[3].ExpSpeed = -2000;
		PID_DEAL(&Gr->GraspMotor[3].SPID,Gr->GraspMotor[3].ExpSpeed,Gr->GraspMotor[3].Encoder->Speed[1]);
		if( (Gr->GraspMotor[3].Encoder->Speed[1] <= 50) && (Gr->GraspMotor[3].Encoder->Speed[1] >= -50) )
		{
			clock[3] ++;
			if(clock[3] >= 5)
			{
				Gr->GraspMotor[3].ExpSpeed = 0;
				clock[3] = 0;
				Gr->reset[7] ++;
				Gr->reset[3] = Finish;
				Gr->GraspMotor[3].SPID.Out = 0;
			}
		}
		
	}
	
	/*��ת�����λ*/
	if(Gr->reset[4] == DisFinish)
	{
		Gr->GraspMotor[4].ExpSpeed = 0;
		Gr->GraspMotor[5].SPID.Out = 0;
		if( (Gr->GraspMotor[4].Encoder->Speed[1] <= 30) && (Gr->GraspMotor[4].Encoder->Speed[1] >= -30) )
		{
			clock[4] ++;
			if(clock[4] >=4 )
			{
				clock[4] = 0;
				Gr->reset[7] += 2;
				Gr->reset[4] = Finish;
				Gr->reset[5] = Finish;
				Gr->GraspMotor[4].Encoder->Lock_Radian = Gr->GraspMotor[4].Encoder->Total_Radian;
				Gr->GraspMotor[4].Encoder->Init_Radian = Gr->GraspMotor[4].Encoder->Total_Radian;
				Gr->GraspMotor[4].ExpRadian 					 = Gr->GraspMotor[4].Encoder->Total_Radian;
				Gr->GraspMotor[5].Encoder->Lock_Radian = Gr->GraspMotor[5].Encoder->Total_Radian;
				Gr->GraspMotor[5].Encoder->Init_Radian = Gr->GraspMotor[5].Encoder->Total_Radian;
				Gr->GraspMotor[5].ExpRadian 					 = Gr->GraspMotor[5].Encoder->Total_Radian;
			}
		}
		PID_DEAL(&Gr->GraspMotor[4].SPID,Gr->GraspMotor[4].ExpSpeed,Gr->GraspMotor[4].Encoder->Speed[1]);
	}
	else
	{
		PID_DEAL(&Gr->GraspMotor[4].PPID,Gr->GraspMotor[4].ExpRadian,Gr->GraspMotor[4].Encoder->Radian);
		PID_DEAL(&Gr->GraspMotor[4].SPID,Gr->GraspMotor[4].PPID.Out,Gr->GraspMotor[4].Encoder->Speed[1]);
		PID_DEAL(&Gr->GraspMotor[5].PPID,Gr->GraspMotor[5].ExpRadian,Gr->GraspMotor[5].Encoder->Radian);
		PID_DEAL(&Gr->GraspMotor[5].SPID,Gr->GraspMotor[5].PPID.Out,Gr->GraspMotor[5].Encoder->Speed[1]);
	}
	
	/*���е����λ���*/
	if(Gr->reset[7] == 5) 
	{ 
			Gr->vL53L0->InitDistance = Gr->vL53L0->distance;
		  Gr->reset[7] = DisFinish;
			Gr->Can_Send_Grasp_1(0,0,0,0);						
			Gr->Can_Send_Grasp_2(0,0,0,0);
			
	}
	
	Gr->Can_Send_Grasp_1(MotorOutput_201_204);
	Gr->Can_Send_Grasp_2(MotorOutput_205_208);

	
}

void bulletSupply (Gr_t *Gr,Motor_t *Supply,int8_t dire) {
	static int8_t last_dire = 0; 
	if( (dire!=-1) && (dire!=1)) return ; 
	if(last_dire==0) 
	{
		last_dire=dire ; //��һ�θ�ֵ 
	}
	if(last_dire!=dire)
	{
		Supply->clock=0;
		Supply->state=DisFinish; 
		last_dire = dire ;
		Supply->dire= dire ;
	}
	
	Supply->ExpSpeed= Supply_Speed *dire ; 
	/*�������*/
	if(int16_t_abs(Supply->Encoder->Speed[1]) <= 20){
		Supply->clock ++ ;
		if(Supply->clock>40) {
			Supply->state =Finish; 
		}
	}
	
	if(Supply->state ==DisFinish) 
		PID_DEAL(&Supply->SPID,Supply->ExpSpeed,Supply->Encoder->Speed[1]) ;
	else Supply->SPID.Out=0 ;
	Gr->Can_Send_Grasp_2(MotorOutput_205_208);
}


/*************************************************************************************************
*����:	Poweroff_Ctrl
*����:	�ϵ����
*�β�: 	Gr_t *Gr
*����:	��
*˵��:	201-̧��	202-ƽ��	203-����	204-����	205-��ת	206-��ת	207-����
*************************************************************************************************/
void Poweroff_Ctrl(Gr_t *Gr)
{
	Gr->GraspMotor[0].Encoder->Init_Radian = Gr->GraspMotor[0].Encoder->Total_Radian;
	Gr->GraspMotor[1].Encoder->Init_Radian = Gr->GraspMotor[1].Encoder->Total_Radian;
	Gr->GraspMotor[2].Encoder->Init_Radian = Gr->GraspMotor[2].Encoder->Total_Radian;
	Gr->GraspMotor[3].Encoder->Init_Radian = Gr->GraspMotor[3].Encoder->Total_Radian;
	Gr->Can_Send_Grasp_1(0,0,0,0);
	Gr->Can_Send_Grasp_2(0,0,0,0);
}




