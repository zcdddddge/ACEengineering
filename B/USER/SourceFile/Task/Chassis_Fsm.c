#include "Chassis_Fsm.h"
#include "Chassis_Task.h"
#include "BoardCommuni.h"

extern Chassis_t Chassis;		/*底盘控制总结构体*/
#define Electromagnet_On					GPIO_SetBits(GPIOC,GPIO_Pin_3)
#define Electromagnet_Off					GPIO_ResetBits(GPIOC,GPIO_Pin_3)

FSM_t Chassis_FSM;			/*底盘状态机*/
State_t OFFLINE;				/*断电模式*/
State_t INDEPEN;				/*独立模式*/
State_t WIGGLE;					/*扭腰模式*/
State_t KEYBOARD;				/*键盘模式*/
State_t CTRL_GRASP;			/*控制夹取模式*/
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
/****************************************************************/

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
		Chassis_FSM.State_Change 		 = StateChange;	//状态机状态变更函数
	
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
	
		/*底盘状态机初始化*/
		Chassis_State_Table[0][0] = INDEPEN;
		Chassis_State_Table[0][2] = WIGGLE;
		Chassis_State_Table[0][1] = OFFLINE;
		Chassis_State_Table[1][0] = OFFLINE;
		Chassis_State_Table[1][1] = OFFLINE;
		Chassis_State_Table[1][2] = OFFLINE;
		Chassis_State_Table[2][0] = KEYBOARD;
		Chassis_State_Table[2][1] = CTRL_GRASP;   //s1 =3 ;  s2=2  
		Chassis_State_Table[2][2] = CTRL_GRASP;   //3      3  
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
	if(Chassis.RC->state.Auto_Clamp == 4)
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
	if(Chassis.RC->state.Electromagnet)
	{
		Electromagnet_On;
	}
	else
	{
		Electromagnet_Off;
	}
	if(Chassis.RC->state.RFID)
	{
		
	}
}

/*键盘控制夹取行为函数*/
static void KeyBoard_Grasp_bhv(void)	 
{
	Send_Ctrl_To_Board(Chassis.RC->state.Auto_Clamp,Chassis.RC->state.Magazine);
	Chassis.Indepen(&Chassis.C,0,0,0,0);
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
	
	Send_RC_To_Board();												//发送遥控数据
	Chassis.Indepen(&Chassis.C,0,0,0,0);			//底盘速度为0
}

/*CTRL_GRASP状态准备函数*/
static void CTRL_GRASP_Prepare(void)
{
	Chassis.Indepen(&Chassis.C,0,0,0,0);			//底盘速度为0
}
