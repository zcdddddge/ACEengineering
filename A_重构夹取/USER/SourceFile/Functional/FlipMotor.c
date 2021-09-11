#include "FlipMotor.h"
#define GRASP_FLIP_S_P   	4.0f
#define GRASP_FLIP_S_I   	0.0f
#define GRASP_FLIP_S_D   	0.05f
#define GRASP_FLIP_P_P  	100.0f
#define	GRASP_FLIP_P_I   	0.0f
#define GRASP_FLIP_P_D   	50.0f

Gr_t *Gr;

/*************************************************************************************************
*����:	Flip_Motor_Init
*����:	�����ʼ��
*�β�: 	G_t *G
*����:	��
*˵��:	��
*************************************************************************************************/
void Flip_Motor_Init(Gr_t *Gr)
{
	Gr->Can_Send_Grasp_2 = CAN1_205_To_208_SEND;
	
	
	Gr->GraspMotor[4].Encoder	=	Return_Can1_201_208_Encoder(5);
	
	Gr->GraspMotor[4].ID = 5;
	
  PID_INIT(&Gr->GraspMotor[4].PPID, GRASP_FLIP_P_P, GRASP_FLIP_P_I, GRASP_FLIP_P_D, 0, 8500);							//λ�û���ʼ��
  PID_INIT(&Gr->GraspMotor[4].SPID, GRASP_FLIP_S_P, GRASP_FLIP_S_I, GRASP_FLIP_S_D, 0, 16000);							//�ٶȻ���ʼ��
	
	Gr->GraspMotor[4].ExpRadian							= Gr->GraspMotor[4].Encoder->Radian;						//��ʼ�������Ƕ�
	Gr->GraspMotor[4].Encoder->Init_Radian 	= Gr->GraspMotor[4].Encoder->Radian;    				//��ʼ����ֵ��ֵ
	Gr->GraspMotor[4].Encoder->Lock_Radian 	= Gr->GraspMotor[4].Encoder->Radian;	    			//��ʼ�������Ƕ�
	Gr->GraspMotor[4].Encoder->Total_Radian = Gr->GraspMotor[4].Encoder->Radian;	    			//��ʼ���ܽǶ�
	Gr->GraspMotor[4].Radio = 19;																														//��ʼ����ȡ������ٱ�
	Gr->GraspMotor[4].ResetFlag = DisFinish ;
	Gr->GraspMotor[4].debug = 0;
	
}

void flip_motor(Motor_t *filp, float exp, float limit, u8 dire)
{
    static int16_t clock = 0;

    if (dire == 1)
    {

        filp->ExpRadian -= 0.4f;

        if(filp->ExpRadian <= filp->Encoder->Init_Radian - exp)
        {
            //     x <= 6-182 =
            filp->ExpRadian = filp->Encoder->Init_Radian - exp;
        }

        /*��ɱ�־*/
        // ƽ�У� -151 <=18-exp  ===>exp = 169
        // ��ֱ�� -225 + exp -6 <=6  ===> exp = 237
        if(float_abs(filp->Encoder->Total_Radian + exp - filp->Encoder->Init_Radian) <= limit)
        {
            filp->state = Finish;
            filp->ExpRadian = filp->Encoder->Init_Radian - exp;
        }

    }
    else if (dire == 2)
    {
        filp->ExpRadian += 0.3f;


        /*��ɱ�־*/
        if (float_abs(filp->Encoder->Total_Radian - filp->Encoder->Init_Radian) <= limit)
        {
            clock ++;
        }

        if (clock >= 20)
        {
            filp->ExpRadian = filp->Encoder->Total_Radian;
            filp->state = Finish;
            clock = 0;
        }
    }
    else
    {
        filp->ExpRadian = filp->Encoder->Total_Radian;
    }
		
    PID_DEAL(&filp->PPID, filp->ExpRadian, filp->Encoder->Total_Radian);
    PID_DEAL(&filp->SPID, filp->PPID.Out, filp->Encoder->Speed[1]);
		
		Gr->Can_Send_Grasp_2(filp->SPID.Out,0,0,0);
}






