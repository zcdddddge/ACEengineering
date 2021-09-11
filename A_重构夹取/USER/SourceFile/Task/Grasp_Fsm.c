#include "Remote_Task.h"
#include "Grasp_Task.h"

extern Grasp_t Grasp;


#define Lift_Up Grasp.Up_Init(&Grasp.Gr)
#define Lift_Down Grasp.Down_Init(&Grasp.Gr)//还没写这个函数记得补上



static u8 lock = 1 	;
//static uint8_t loop_lock = 1;
FSM_t Grasp_Fsm 	;
State_t AUTOGRASP	;
State_t KEYBOARD	;
State_t PICKGOLD	;
State_t OFFLINE		;
State_t GRASPRESET;    /*夹取复位*/
State_t REMOTEG		;
	
State_t GETGOLD		;

State_t Grasp_State_Table[State_Line][State_Column];


/******************自动夹取**************************/
static void AutoGrasp_State(void) ;
static void AutoGrasp_bhv(void);
static void AtuoGrasp_Prepare(void) ;
/*****************键盘******************************/
static void KeyBoard_State(void) ;
//static void KeyBoard_bhv(void);

static void Auto_One_Box_bhv(void) ;//夹取一箱
static void Auto_Two_Box_bhv(void) ;//夹取两箱
static void Auto_Three_Box_bhv(void) ;//夹取三箱
static void Flip_Reset_bhv(void) ;//翻转复位
static void All_Reset_bhv(void) ;//上装复位

static void KeyBoard_Prepare(void) ;
/***************捡*******************************/
static void Pick_State(void) ;
static void Pick_bhv(void);
static void Pick_Prepare(void) ;
/*****************离线*****************************/
static void Offline_State(void) ;
static void Offline_bhv(void);
static void Offline_Prepare(void) ;
/*****************遥控****************************/
static void Remote_State(void) ;
static void Remote_bhv(void);
static void Remote_Prepare(void) ;
/****************兑换金币*****************************/
static void Gold_State(void) ;
static void Gold_bhv(void);
static void Gold_Prepare(void) ;

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

    OFFLINE.Behavior_Process = NULL ;
    OFFLINE.State_Prepare    = Offline_State ;
    OFFLINE.State_Process    = Offline_Prepare;

    REMOTEG.Behavior_Process = NULL ;
    REMOTEG.State_Prepare    = Remote_State ;
    REMOTEG.State_Process    = Remote_Prepare;

    GETGOLD.Behavior_Process = NULL ;
    GETGOLD.State_Prepare    = Gold_State ;
    GETGOLD.State_Process    = Gold_Prepare;

    PICKGOLD.Behavior_Process = NULL ;
    PICKGOLD.State_Prepare    = Pick_State;
    PICKGOLD.State_Process    = Pick_Prepare ;

    /*底盘状态机初始化*/
    Grasp_State_Table[0][0] = OFFLINE;      //s1=1 ,s2=1 离线
    Grasp_State_Table[0][2] = OFFLINE;      //s1=1  s2=3 离线
    Grasp_State_Table[0][1] = OFFLINE;      //s1=1  s2=2 离线
    Grasp_State_Table[1][0] = OFFLINE;      //s1=2  s2=1 离线
    Grasp_State_Table[1][1] = PICKGOLD;     //s1=2  s2=2 地上捡
    Grasp_State_Table[1][2] = REMOTEG;      //s1=2  s2=3 遥控夹取
    Grasp_State_Table[2][0] = KEYBOARD;     //s1=3 s2=1  键盘
    Grasp_State_Table[2][1] = GETGOLD;      //s1=3 s2=2  兑换金币
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
    if(lock == 1)
    {
        Grasp.Auto_Grasp(&Grasp.Gr, 1) ;
        lock = 2 ; //上锁
    }
    else
    {
        Grasp.Auto_Grasp(&Grasp.Gr, 0) ;
    }
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
    lock = 1 ; //解锁
    Grasp.Grasp_Poweroff(&Grasp.Gr);
}
static void Offline_Prepare(void)
{

}

/*******************************Remote******************************************************/
static void Remote_State(void)
{
    Grasp_Fsm.Current_State->Behavior_Process = Remote_bhv;
}
static void Remote_bhv(void)
{
    Grasp.RC_Grasp(&Grasp.Gr, &Grasp.Can2_RC->Can_RC);
}
static void Remote_Prepare(void) {}


/**********************KeyBoard******************************************************/
static void KeyBoard_State(void)
{
//    Grasp_Fsm.Current_State->Behavior_Process = KeyBoard_bhv;



    if(Grasp.Can2_RC->state.Clap_Init)
    {
        Lift_Up;
    }
    else
    {
        Lift_Down;
    }

    if(Grasp.Can2_RC->state.Auto_One_Box == 1)
    {
        Grasp_Fsm.Current_State->Behavior_Process = Auto_One_Box_bhv;
    }

    if(Grasp.Can2_RC->state.Auto_Two_Box ==1)
    {
        Grasp_Fsm.Current_State->Behavior_Process = Auto_Two_Box_bhv;
    }

    if(Grasp.Can2_RC->state.Auto_Three_Box == 1)
    {
        Grasp_Fsm.Current_State->Behavior_Process = Auto_Three_Box_bhv;
    }

    if(Grasp.Can2_RC->state.Flip_Reset == 1)
    {
        Grasp_Fsm.Current_State->Behavior_Process = Flip_Reset_bhv;
    }

//    if(Grasp.Can2_RC->state.All_Reset == 1)
//    {
//        Grasp_Fsm.Current_State->Behavior_Process = All_Reset_bhv;
//    }
}



static void Auto_One_Box_bhv(void)
{
    Grasp.Grasp_The_First(&Grasp.Gr);
}
static void Auto_Two_Box_bhv(void)
{
    Grasp.Grasp_The_Second(&Grasp.Gr);
}
static void Auto_Three_Box_bhv(void)
{
    Grasp.Grasp_The_Third(&Grasp.Gr);
}
static void Flip_Reset_bhv(void)
{
    Grasp.Flipmotor_Reset(&Grasp.Gr);
}
/* static void All_Reset_bhv(void)
{
等函数补齐再写这部分
}
 */











//static void KeyBoard_bhv(void)
//{
//    static uint8_t graspLock = 1 ;

//    /*弹仓*/
////    if(Grasp.Can2_RC->state.Magazine)
////    {
////        MAZAGINE_OPEN;
////    }
////    else
////    {
////        MAZAGINE_CLOSE ;
////    }
//		
//    /*抬升*///等测距装上换函数再测
////    if(Grasp.Can2_RC->state.Up_motor)
////    {
////        DOWN;
////    }
////    else
////    {
////        LIFT;
////    }
//		
//    /*平移*/
//    if(Grasp.Can2_RC->state.Flex_motor)
//    {
////				SLIDE_BACK;
////        RALL_BACK;
//    }
//    else
//    {
////				SLIDE_FORWARD;
////        RALL_FORWARD;
//    }
//		
//    /*翻转*/
//    if(Grasp.Can2_RC->state.Flip_motor)
//    {
//        TIP_BACK;
//    }
//    else
//    {
//        TURN_OVER;
//    }
//		pid_Cala(&Grasp.Gr);
//    /*夹子*/
////    if(Grasp.Can2_RC->state.Clamp_motor)
////    {
////        PINE_CLIP;
////    }
////    else
////    {
////        CLAMPING ;
////    }
//		
//    /**夹取一箱**/
//    #if 0

//    if(Grasp.Can2_RC->state.Auto_Clamp)
//    {
//        if(graspLock == 1)
//        {
//            Grasp.Auto_Grasp(&Grasp.Gr, 1) ;
//            graspLock = 2;
//        }
//        else
//        {
//            Grasp.Auto_Grasp(&Grasp.Gr, 0) ;
//        }
//    }
//    else
//    {
//        graspLock = 1; //解锁
//    }

//    #endif
//}
static void KeyBoard_Prepare(void) {}



/*****************Gold*******************************/
static void Gold_State(void)
{
    Grasp_Fsm.Current_State->Behavior_Process = Gold_bhv;
}
static void Gold_bhv(void)
{
    Grasp.Change_Gold(&Grasp.Gr);
}
static void Gold_Prepare(void) {}



static void Pick_bhv(void)
{
    //锁检测
    if(Grasp.Gr.state[3] != 5)
    {
        Grasp.Pick_Gold(&Grasp.Gr);
    }

}
static void Pick_State(void)
{
    Grasp_Fsm.Current_State->Behavior_Process = Pick_bhv;
}

static void Pick_Prepare(void)
{
    Grasp.Gr.state[3] = 0 ; //重新赋值状态、开锁
}
