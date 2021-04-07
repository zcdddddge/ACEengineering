#include "Chassis_Fsm.h"
#include "Chassis_Task.h"
#include "BoardCommuni.h"


/*底盘控制总结构体*/
extern Chassis_t Chassis;		
/*电机动作宏定义*/
#define Electromagnet_On					GPIO_SetBits(GPIOC,GPIO_Pin_3)
#define Electromagnet_Off					GPIO_ResetBits(GPIOC,GPIO_Pin_3)
#define Rifd_ForWard              Chassis.Rescue(&Chassis.C, -1);   
#define Rifd_BackWard             Chassis.Rescue(&Chassis.C, 1);   

#define Barrier_Up  							2
#define Barrier_Down    					3

FSM_t Chassis_FSM;			/*底盘状态机*/
State_t OFFLINE;				/*断电模式*/
State_t INDEPEN;				/*独立模式*/
State_t WIGGLE;					/*扭腰模式*/
State_t KEYBOARD;				/*键盘模式*/
State_t CTRL_GRASP;			/*控制夹取模式*/
State_t RESCUE;          /*救援模式*/
State_t Chassis_State_Table[State_Line][State_Column];			/*状态参数表*/

/***************************OFFLINE******************************/
static void Offline_State(void);				/*离线状态处理*/
static void PowerOff_bhv(void);					/*断电行为函数*/
static void Offline_Prepare(void);			/*OFFLINE状态准备函数*/
/***************************INDEPEN******************************/
static void Indepen_State(void);				/*独立状态处理*/
static void Chassis_Normal_bhv(void);		/*底盘正常运动行为函数*/
static void Indepen_Prepare(void);			/*INDEPEN状态准备函数*/
/***************************WIGGLE******************************/
static void Wiggle_State(void);					/*扭腰状态处理*/
static void Wiggle_Normal_bhv(void);		/*正常扭腰运动行为函数*/
static void Wiggle_Prepare(void);				/*Wiggle状态准备函数*/
/***************************KEYBOARD*****************************/
static void KeyBoard_State(void);				/*键盘状态处理*/
static void KeyBoard_Chassis_bhv(void);	/*键盘控制底盘行为函数*/
static void KeyBoard_Grasp_bhv(void);	  /*键盘控制夹取行为函数*/
static void KeyBoard_Prepare(void);			/*KEYBOARD状态准备函数*/
/***************************CTRL_GRASP*****************************/
static void CTRL_GRASP_State(void);				/*控制夹取状态处理*/
static void GRASP_RC_bhv(void);						/*遥控控制夹取行为函数*/
static void CTRL_GRASP_Prepare(void);			/*CTRL_GRASP状态准备函数*/
/****************************RESCUE-2020-11-18-author:wingchi-leung*********************/ 
static void Rescue_State(void);            /*救援状态处理*/
static void Rescue_Prepare(void); 				/*救援状态准备*/
static void Rescue_bhv(void) ;  					/*救援行为函数*/



/*返回底盘状态机控制指针*/
FSM_t *Return_Chassis_FSM(void)
{
    return &Chassis_FSM;
}


/*底盘状态机初始化*/
void Chassis_FSM_Init(void)
{
    Chassis_FSM.State_Table      = Chassis_State_Table;
    Chassis_FSM.Last_State       = NULL;
    Chassis_FSM.Current_State    = NULL;
	Chassis_FSM.State_Change 	 = StateChange;	//状态机状态变更函数
	
		/*OFFLINE状态初始化*/
		OFFLINE.Behavior_Process 	= NULL;
		OFFLINE.State_Process			= Offline_State;
		OFFLINE.State_Prepare			= Offline_Prepare;
		/*INDEPEN状态初始化*/
		INDEPEN.Behavior_Process 	= NULL;
		INDEPEN.State_Process			= Indepen_State;
		INDEPEN.State_Prepare			= Indepen_Prepare;
		/*WIGGLE状态初始化*/
		WIGGLE.Behavior_Process 	= NULL;
		WIGGLE.State_Process			= Wiggle_State;
		WIGGLE.State_Prepare			= Wiggle_Prepare;
		/*KEYBOARD状态初始化*/
		KEYBOARD.Behavior_Process 	= NULL;
		KEYBOARD.State_Process			= KeyBoard_State;
		KEYBOARD.State_Prepare			= KeyBoard_Prepare;
		/*CTRL_GRASP状态初始化*/
		CTRL_GRASP.Behavior_Process 	= NULL;
		CTRL_GRASP.State_Process			= CTRL_GRASP_State;
		CTRL_GRASP.State_Prepare			= CTRL_GRASP_Prepare;
		/*RESCUE状态初始化*/ 
		RESCUE.Behavior_Process   =	NULL ;
		RESCUE.State_Process      = Rescue_State;
		RESCUE.State_Prepare			= Rescue_Prepare; 
		
	
		/*底盘状态机初始化*/
		Chassis_State_Table[0][0] = INDEPEN;     //s1=1 ,s2=1 底盘独立INDEPEN
		Chassis_State_Table[0][2] = WIGGLE;      //s1=1  s2=3 扭腰WIGGLE
		Chassis_State_Table[0][1] = OFFLINE;     //s1=1  s2=2 离线
		Chassis_State_Table[1][0] = RESCUE;	//s1=2  s2=1 RESCUE暂时--障碍块测试
		Chassis_State_Table[1][1] = CTRL_GRASP;  //s1=2  s2=2 RESCUE 救援卡后缩
		Chassis_State_Table[1][2] = CTRL_GRASP;  //s1=2  s2=3 夹取
		Chassis_State_Table[2][0] = KEYBOARD;    //s1=3 s2=1  键盘
		Chassis_State_Table[2][1] = CTRL_GRASP;  //s1=3 s2=2  夹取
		Chassis_State_Table[2][2] = CTRL_GRASP;  //s1=3 s2=3  夹取
}


/********************************RESCUE**********************************************/
static void Rescue_State(void)
{
	Chassis_FSM.Current_State->Behavior_Process = Rescue_bhv;
}

static void Rescue_Prepare(void) 
{
	
}
static void Rescue_bhv(void) 
{
		float  s = ((float)(Chassis.RC->RC_ctrl->ch0) * 10.0f) ;  
//	if(Chassis.RC->RC_ctrl->s2 ==0 ) 
		Chassis.Barrier(&Chassis.C, s);
//	else if  (Chassis.RC->RC_ctrl->s2 == 1)
//		Chassis.Barrier(&Chassis.C, -1 );
//	Chassis.Indepen(&Chassis.C,0,0,0,0);


	
}

/***************************************OFFLINE**************************************/
/*OFFLINE状态执行函数*/
static void Offline_State(void)
{
	Chassis_FSM.Current_State->Behavior_Process = PowerOff_bhv;
}


/*OFFLINE状态准备函数*/
static void Offline_Prepare(void)
{
    Chassis.C.WheelMotor[0].SPID.Out = Chassis.C.WheelMotor[1].SPID.Out \
			= Chassis.C.WheelMotor[2].SPID.Out = Chassis.C.WheelMotor[3].SPID.Out = 0;
}	

/*断电行为函数*/
static void PowerOff_bhv(void)
{
		Chassis.Poweroff(&Chassis.C);
}


/***************************************INDEPEN**************************************/
/*独立状态处理*/
static void Indepen_State(void)				
{

	Chassis_FSM.Current_State->Behavior_Process = Chassis_Normal_bhv;
}

/*底盘正常运动行为函数*/
static void Chassis_Normal_bhv(void)
{
	Electromagnet_On;
	Chassis.Indepen(&Chassis.C,Chassis.RC->RC_X.Output,Chassis.RC->RC_Y.Output,Chassis.RC->RC_Z.Output,0);
	
}

/*INDEPEN状态准备函数*/
static void Indepen_Prepare(void)
{
	Chassis.C.gyro->Yaw_Lock = Chassis.C.gyro->Yaw;
}


/***************************************WIGGLE**************************************/
/*扭腰状态处理*/
static void Wiggle_State(void)					
{
	Chassis_FSM.Current_State->Behavior_Process = Wiggle_Normal_bhv;
}

/*正常扭腰运动行为函数*/
static void Wiggle_Normal_bhv(void)
{
	Chassis.Wiggle(&Chassis.C,Chassis.RC->RC_X.Output,Chassis.RC->RC_Y.Output,0.0f);
}	

/*Wiggle状态准备函数*/
static void Wiggle_Prepare(void)
{
	Chassis.C.gyro->Yaw_Lock = Chassis.C.gyro->Yaw;
}	


/***************************************KeyBoard**************************************/
/*键盘状态处理*/
static void KeyBoard_State(void)
{
  /************键盘B键-->模式选择*****************************************/
	if(Chassis.RC->state.Clip) 
	{
		Chassis_FSM.Current_State->Behavior_Process = KeyBoard_Grasp_bhv;
	}
	else
	{
		Chassis_FSM.Current_State->Behavior_Process = KeyBoard_Chassis_bhv;
	}
}

/*键盘控制底盘行为函数*/
static void KeyBoard_Chassis_bhv(void)
{
	
	Chassis.Indepen(&Chassis.C,Chassis.RC->KM_X.Output,Chassis.RC->KM_Y.Output,Chassis.RC->KM_Z.Output,0);
	//电磁铁 
	if(Chassis.RC->state.Electromagnet){ 
		Electromagnet_On;
	}
	else{
		Electromagnet_Off;
	}
	//救援卡
	if(Chassis.RC->state.RFID){
		Rifd_ForWard; 
	}else {
	   Rifd_BackWard;
	}
	//障碍块
	if(!Chassis.RC->state.Barrier)
	{
		
	}
	
}

/*键盘控制夹取行为函数*/
static void KeyBoard_Grasp_bhv(void)	 
{
	/*发送两个标志位给夹取*/
	Send_Ctrl_To_Board(Chassis.RC->state.Auto_Clamp,Chassis.RC->state.Magazine);
	/*键盘控制底盘*/
	Chassis.Indepen(&Chassis.C,Chassis.RC->KM_X.Output,Chassis.RC->KM_Y.Output,Chassis.RC->KM_Z.Output,0);

}

/*KEYBOARD状态准备函数*/
static void KeyBoard_Prepare(void)
{
	Chassis.C.gyro->Yaw_Lock = Chassis.C.gyro->Yaw;
}


/***************************************CTRL_GRASP**************************************/
/*控制夹取状态处理*/
static void CTRL_GRASP_State(void)				
{
	Chassis_FSM.Current_State->Behavior_Process = GRASP_RC_bhv;
	
}

/*遥控控制夹取行为函数*/
static void GRASP_RC_bhv(void)
{
	Send_RC_To_Board();		//发送遥控数据
	Chassis.Indepen(&Chassis.C,0,0,0,0);
}

/*CTRL_GRASP状态准备函数*/
static void CTRL_GRASP_Prepare(void)
{
	Chassis.Indepen(&Chassis.C,0,0,0,0);			//底盘速度为0
}


#if 0 
static void Gimbal_State(void)
{
	Chassis_FSM.Current_State->Behavior_Process = Gimbal_bhv;
}

static void Gimbal_Prepare(void){
}
static void Gimbal_bhv(void) {
		PY_Encoder_DRIVE(&Chassis.G, Chassis.RC->RC_X.Output,Chassis.RC->RC_Y.Output,Chassis.G.YawMotor.Encoder->Speed[1]);
}
#endif 
