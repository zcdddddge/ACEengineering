#include "Grasp_Fsm.h"   
#include "Remote_Task.h"
#include "Grasp_Task.h" 

extern Grasp_t Grasp;
static u8 lock=1 ;
FSM_t Grasp_Fsm ;
State_t AUTOGRASP;
State_t KEYBOARD;
State_t SUPPLY;
State_t OFFLINE;
State_t GRASPRESET;    /*夹取复位*/
State_t REMOTEG;  

State_t Grasp_State_Table[State_Line][State_Column];

/******************自动夹取**************************/
static void AutoGrasp_State(void) ;
static void AutoGrasp_bhv(void);
static void AtuoGrasp_Prepare(void) ;
/*****************键盘******************************/
static void KeyBoard_State(void) ;
static void KeyBoard_bhv(void);
static void KeyBoard_Prepare(void) ;
/*****************供弹*****************************/
static void Supply_State(void) ;
static void Supply_Open_bhv(void);
static void Supply_Close_bhv(void);
static void Supply_Prepare(void) ;
/*****************离线*****************************/
static void Offline_State(void) ;
static void Offline_bhv(void);
static void Offline_Prepare(void) ;
/*****************复位****************************/
static void Reset_State(void) ;
static void Reset_bhv(void);
static void Reset_Prepare(void) ;
/*****************遥控****************************/
static void Remote_State(void) ;
static void Remote_bhv(void);
static void Remote_Prepare(void) ;

void Chassis_FSM_Init(void) 
{
    Grasp_Fsm.State_Table    = Grasp_State_Table;
    Grasp_Fsm.Last_State     = NULL;
    Grasp_Fsm.Current_State  = NULL;
    Grasp_Fsm.State_Change   = StateChange; 

    AUTOGRASP.Behavior_Process = NULL ;
    AUTOGRASP.State_Prepare    = AutoGrasp_State ;
    AUTOGRASP.State_Process    = AtuoGrasp_Prepare;

    KEYBOARD.Behavior_Process = NULL ;
    KEYBOARD.State_Prepare    = KeyBoard_State ;
    KEYBOARD.State_Process    = KeyBoard_Prepare;

    SUPPLY.Behavior_Process = NULL ;
    SUPPLY.State_Prepare    = Supply_State ;
    SUPPLY.State_Process    = Supply_Prepare; 

    OFFLINE.Behavior_Process = NULL ;
    OFFLINE.State_Prepare    = Offline_State ;
    OFFLINE.State_Process    = Offline_Prepare;
 
    GRASPRESET.Behavior_Process = NULL ;
    GRASPRESET.State_Prepare    = Reset_State ;
    GRASPRESET.State_Process    = Reset_Prepare;
		
		REMOTEG.Behavior_Process = NULL ;
    REMOTEG.State_Prepare    = Remote_State ;
    REMOTEG.State_Process    = Remote_Prepare;

    /*底盘状态机初始化*/
    Grasp_State_Table[0][0] = OFFLINE;      //s1=1 ,s2=1 离线
    Grasp_State_Table[0][2] = OFFLINE;      //s1=1  s2=3 离线
    Grasp_State_Table[0][1] = OFFLINE;      //s1=1  s2=2 离线
    Grasp_State_Table[1][0] = OFFLINE;      //s1=2  s2=1 弹仓
    Grasp_State_Table[1][1] = OFFLINE;      //s1=2  s2=2 弹仓
    Grasp_State_Table[1][2] = REMOTEG;      //s1=2  s2=3 遥控夹取
    Grasp_State_Table[2][0] = KEYBOARD;     //s1=3 s2=1  键盘 
    Grasp_State_Table[2][1] = OFFLINE;      //s1=3 s2=2  离线
    Grasp_State_Table[2][2] = AUTOGRASP;    //s1=3 s2=3  自动夹取 
		
}

FSM_t *Return_Chassis_Fsm(void) 
{
    return &Grasp_Fsm;
}

/*****************************Auto_Grasp*****************************************/
static void AutoGrasp_State(void) 
{
    Grasp_Fsm.Current_State->Behavior_Process = AutoGrasp_bhv; 
}
static void AutoGrasp_bhv(void)
{
	#if 0 
		if(lock==1) {
			Grasp.Auto_Grasp(&Grasp.Gr,1) ;
			lock=2 ;  //上锁
		}else {
			Grasp.Auto_Grasp(&Grasp.Gr,0) ;
		}	
		
	#endif 
	
	
	Grasp.Send_Crtl(3000) ;
	
	
}
static void AtuoGrasp_Prepare(void) 
{}

/******************************Offline************************************************************/
static void Offline_State(void) 
{
    Grasp_Fsm.Current_State->Behavior_Process =  Offline_bhv;
}
static void Offline_bhv(void)
{
		lock=1 ; //解锁
    Grasp.Grasp_Poweroff(&Grasp.Gr); 
}
static void Offline_Prepare(void) {

}

/*******************************Supply***************************************************/
static void Supply_State(void) 
{
	if(Grasp.Can2_RC->Can_RC.s2 ==1) 
	{
		Grasp_Fsm.Current_State->Behavior_Process= Supply_Open_bhv; 
	}
	else if(Grasp.Can2_RC->Can_RC.s2==2) 
	{
		Grasp_Fsm.Current_State->Behavior_Process= Supply_Close_bhv; 
	}
	
}
static void Supply_Open_bhv(void)
{
	MAZAGINE_OPEN;
}

static void Supply_Close_bhv(void)
{
	MAZAGINE_CLOSE;
}
static void Supply_Prepare(void) 
{
	
}

/*******************************Remote******************************************************/
static void Remote_State(void) 
{
	Grasp_Fsm.Current_State->Behavior_Process =Remote_bhv; 
}
static void Remote_bhv(void) 
{
	Grasp.RC_Ctrl(&Grasp.Gr,&Grasp.Can2_RC->Can_RC); 
}
static void Remote_Prepare(void) {}


/**********************KeyBoard******************************************************/
static void KeyBoard_State(void) {
		Grasp_Fsm.Current_State->Behavior_Process=KeyBoard_bhv; 
}
static void KeyBoard_bhv(void){
		static uint8_t graspLock=1 ; 
		
		
		if(Grasp.Can2_RC->state.Magazine) 
		{
			MAZAGINE_OPEN;
		}
		else 
		{
			MAZAGINE_CLOSE ;
		}
		
	if(Grasp.Can2_RC->state.Auto_Clamp) 
	{
		if(graspLock==1){
			Grasp.Auto_Grasp(&Grasp.Gr,1) ;
			graspLock=2;
		}else {
			Grasp.Auto_Grasp(&Grasp.Gr,0) ;
		}
	}else {
			graspLock=1; //解锁
	}
		
} 
static void KeyBoard_Prepare(void) {}

/**********************Reset******************************************************/
static void Reset_State(void){}
static void Reset_bhv(void){}
static void Reset_Prepare(void) {}




