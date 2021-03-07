/*
 * @Date: 2021-02-24 11:38:12
 * @LastEditTime: 2021-03-06 10:13:13
 * @LastEditors: Please set LastEditors
 * @Description: 定义了工程自动夹取控制函数,重点在状态整理 
 */

#include "RobotAction.h"

static void Auto_One_Box(Gr_t *Gr);
static void Auto_Two_Box(Gr_t *Gr);

/**
 * @description: 自动夹取控制流程,重点整理状态 
 * @param {Gr_t} *Gr
 * @param {u8} box 箱子数
 * @return {*}
 * @note 201-抬升	202-平移	203-伸缩	204-夹子	205-翻转	206-翻转	207-弹仓
 */
void Auto_Ctrl(Gr_t *Gr,u8 box)
{
	static u8 box_lock = 1;
	static u8 boxs = 0;
	/*箱子锁解锁，重新赋值箱子(防止操作手误操作)*/
	if(box_lock == 1){
		boxs = box;
		box_lock = 2;
		if(boxs != 0)
			Gr->GraspMotor[0].state = DisFinish;//预备抬升
	}

	/*箱子数目为1*/
	if(boxs == 1){
		if(Gr->state[0] != 4){
			uplift(&Gr->GraspMotor[0],Gr->vL53L0,10.0f,1);		//夹取前抬升准备
			if(UPLIFT.state == Finish)
			{
				Auto_One_Box(Gr);
			}
		}
		else if(Gr->state[0] == 4){
			uplift(&Gr->GraspMotor[0],Gr->vL53L0,1.0f,2);		//夹取后放回
			if(Gr->GraspMotor[0].state == Finish)
			{
				Gr->GraspMotor[3].state = DisFinish;
				Gr->GraspMotor[4].state = DisFinish;
				Gr->GraspMotor[5].state = DisFinish;
				box_lock = 1;
				boxs = 0;
				Gr->state[0] = 0;   
			}
		}
	}


	
	else if(boxs == 2)/*箱子数目为2*/
	{
		if(Gr->state[1] != 4){
			uplift(&Gr->GraspMotor[0],Gr->vL53L0,13.5f,1);		//夹取前抬升准备
			if(Gr->GraspMotor[0].state == Finish){
				Auto_Two_Box(Gr);	
			}
		}
		else if(Gr->state[1] == 4)
		{
			uplift(&Gr->GraspMotor[0],Gr->vL53L0,4.5f,2);		//夹取后放回
			if(Gr->GraspMotor[0].state == Finish)
			{
				Gr->GraspMotor[1].ExpSpeed = 0;							//平移速度置0
				Gr->GraspMotor[1].state = DisFinish;				//平移标记为未完成
				Gr->GraspMotor[3].ExpSpeed = 0;
				Gr->GraspMotor[3].state = DisFinish;
				Gr->GraspMotor[4].state = DisFinish;
				Gr->GraspMotor[5].state = DisFinish;
				box_lock = 1;                             // 箱子解锁 
				boxs = 0;
				Gr->state[1] = 0;
																	 //临时用于判断结束
			}
		}
	}
	else if(boxs == 0)
	{
		box_lock = 1;
	}
	pid_Cala(Gr);
}








/**
 * @description: 自动夹取一箱
 * @param {*}
 * @return {*}
 */
static void Auto_One_Box(Gr_t *Gr)
{
	switch (Gr->state[0])
	{
		/*前翻*/
		case 0:
		{
			flip(&Gr->GraspMotor[4],&Gr->GraspMotor[5],182.0f,6.0f,1);   
			if(Gr->GraspMotor[4].state == Finish && Gr->GraspMotor[5].state == Finish)
			{
				Gr->GraspMotor[3].ExpSpeed = 0;
				Gr->GraspMotor[3].state = DisFinish;					//夹子标志为未完成
				Gr->state[0] = 1;
			}
			break;
		}
		/*夹紧*/
		case 1:
		{
			clip(&Gr->GraspMotor[3],Cilp_Speed,1);
			if(Gr->GraspMotor[3].state == Finish)
			{
				Gr->GraspMotor[4].state = DisFinish;					//翻转标记为未完成
				Gr->GraspMotor[5].state = DisFinish;
				Gr->state[0] = 2;
			}
			break;
		}
		/*后翻*/
		case 2:
		{
			flip(&Gr->GraspMotor[4],&Gr->GraspMotor[5],-160.0f,20.0f,2);   // -170-->-160  变大
			if(Gr->GraspMotor[4].state == Finish && Gr->GraspMotor[5].state == Finish)
			{
				Gr->GraspMotor[3].ExpSpeed = 0;
				Gr->GraspMotor[3].state = DisFinish;					//夹子标志为未完成
				Gr->state[0] = 3;
			}
			break;
		}
		/*松夹*/
		case 3:
		{
			clip(&Gr->GraspMotor[3],Cilp_Speed,2);
			if(Gr->GraspMotor[3].state == Finish)
			{
				Gr->GraspMotor[3].ExpSpeed = 0;
				Gr->GraspMotor[3].state = DisFinish;				//夹子标记为未完成
				Gr->GraspMotor[0].state = DisFinish;				//抬升标记为未完成
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
		/*平移*/
		case 0:
		{
			rail(&Gr->GraspMotor[1],Gr->sensor,1);
			if(Gr->GraspMotor[1].state == Finish)
			{
				Gr->GraspMotor[4].state = DisFinish;					//翻转标记为未完成
				Gr->GraspMotor[5].state = DisFinish;
				Gr->state[0] = 0;															//夹取一箱为重新开始
				Gr->state[1] = 1;
			}
			break;
		}
		/*夹取一箱*/
		case 1:
		{
			Auto_One_Box(Gr);
			if(Gr->state[0] == 4)
			{
				Gr->GraspMotor[0].state = Finish;						//抬升标记为完成
				Gr->GraspMotor[1].state = DisFinish;				//平移标记为未完成
				Gr->state[1] = 2;
			}
			break;
		}
		/*平移*/
		case 2:
		{
			rail(&Gr->GraspMotor[1],Gr->sensor,1);
			if(Gr->GraspMotor[1].state == Finish)
			{
				Gr->GraspMotor[4].state = DisFinish;					//翻转标记为未完成
				Gr->GraspMotor[5].state = DisFinish;
				Gr->state[0] = 0;															//夹取一箱为重新开始
				Gr->state[1] = 3;
			}
			break;
		}
		/*夹取一箱*/
		case 3:
		{
			Auto_One_Box(Gr);
			if(Gr->state[0] == 4)
			{
				Gr->GraspMotor[0].state = DisFinish;						//抬升标记为完成
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
 * @description: 夹取失败复位:考虑到一般都是前翻时打到箱子-只执行后翻,并保持抬升,没有改变其他状态
 * @param {Gr_t} *Gr
 * @return {*}
 * @note 3/1 待测试
 */
void Reset(Gr_t *Gr)
{

	flip(&Gr->GraspMotor[4], &Gr->GraspMotor[5], -160.0f, 20.0f, 2); 

	if(Gr->GraspMotor[0].state==Finish) {
		PID_DEAL(&Gr->GraspMotor[0].PPID,Gr->GraspMotor[0].Encoder->Lock_Radian,Gr->GraspMotor[0].Encoder->Total_Radian);										//外环
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

		/*完成标志*/
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



