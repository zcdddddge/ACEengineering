#ifndef __BOARDCOMMUNI_H_
#define __BOARDCOMMUNI_H_
#include "stm32f4xx.h"
#include "REMOTE_ISR.h"
#include "GPIO_DEAL.h"

#define AUTO_GRASP_S1     3
#define AUTO_GRASP_S2     3 


typedef __packed struct 
{
	unsigned char Magazine 			: 2;
	unsigned char Auto_Clamp 		: 1;
	unsigned char Up_motor			: 1;				//抬升电机
	unsigned char Flex_motor		: 1;				//伸缩电机
	unsigned char Flip_motor		: 1;				//翻转电机
	unsigned char Clamp_motor		: 1;				//夹子电机

	/* -----------------------------以下为新结构体------------------------ */
	unsigned char Clap_Init : 1;	  //夹取抬升初始化
	unsigned char Auto_One_Box : 2;	  //夹取一箱
	unsigned char Auto_Two_Box : 2;	  //夹取两箱
	unsigned char Auto_Three_Box : 2; //夹取三箱
	unsigned char Flip_Reset : 2;	  //翻转电机复位
	unsigned char All_Reset : 2;	  //上装复位
}KeyBoard_State_t;

typedef __packed struct
{
	RC_ctrl_t Can_RC;
	u8 *Rc;
	KeyBoard_State_t state;
}Board_Communi_t;
	
void Board_Communi_Init(void);
void Board_Communi_Updata(void);
Board_Communi_t* Return_Board_Communi(void);
void Send_Crtl_To_Board(int16_t ch0 , u8 Smooth_L , u8 Smooth_R);

#endif
