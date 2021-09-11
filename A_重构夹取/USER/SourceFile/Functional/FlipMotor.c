#include "FlipMotor.h"
#define GRASP_FLIP_S_P   	4.0f
#define GRASP_FLIP_S_I   	0.0f
#define GRASP_FLIP_S_D   	0.05f
#define GRASP_FLIP_P_P  	100.0f
#define	GRASP_FLIP_P_I   	0.0f
#define GRASP_FLIP_P_D   	50.0f

Gr_t *Gr;

/*************************************************************************************************
*名称:	Flip_Motor_Init
*功能:	电机初始化
*形参: 	G_t *G
*返回:	无
*说明:	无
*************************************************************************************************/
void Flip_Motor_Init(Gr_t *Gr)
{
	Gr->Can_Send_Grasp_2 = CAN1_205_To_208_SEND;
	
	
	Gr->GraspMotor[4].Encoder	=	Return_Can1_201_208_Encoder(5);
	
	Gr->GraspMotor[4].ID = 5;
	
  PID_INIT(&Gr->GraspMotor[4].PPID, GRASP_FLIP_P_P, GRASP_FLIP_P_I, GRASP_FLIP_P_D, 0, 8500);							//位置环初始化
  PID_INIT(&Gr->GraspMotor[4].SPID, GRASP_FLIP_S_P, GRASP_FLIP_S_I, GRASP_FLIP_S_D, 0, 16000);							//速度环初始化
	
	Gr->GraspMotor[4].ExpRadian							= Gr->GraspMotor[4].Encoder->Radian;						//初始化期望角度
	Gr->GraspMotor[4].Encoder->Init_Radian 	= Gr->GraspMotor[4].Encoder->Radian;    				//初始码盘值赋值
	Gr->GraspMotor[4].Encoder->Lock_Radian 	= Gr->GraspMotor[4].Encoder->Radian;	    			//初始化上锁角度
	Gr->GraspMotor[4].Encoder->Total_Radian = Gr->GraspMotor[4].Encoder->Radian;	    			//初始化总角度
	Gr->GraspMotor[4].Radio = 19;																														//初始化夹取电机减速比
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

        /*完成标志*/
        // 平行： -151 <=18-exp  ===>exp = 169
        // 垂直： -225 + exp -6 <=6  ===> exp = 237
        if(float_abs(filp->Encoder->Total_Radian + exp - filp->Encoder->Init_Radian) <= limit)
        {
            filp->state = Finish;
            filp->ExpRadian = filp->Encoder->Init_Radian - exp;
        }

    }
    else if (dire == 2)
    {
        filp->ExpRadian += 0.3f;


        /*完成标志*/
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






