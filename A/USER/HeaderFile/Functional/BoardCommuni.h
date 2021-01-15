#ifndef __BOARDCOMMUNI_H_
#define __BOARDCOMMUNI_H_
#include "stm32f4xx.h"
#include "REMOTE_ISR.h"

typedef __packed struct 
{
	unsigned char Magazine : 2;
	unsigned char Auto_Clamp : 1;
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
#endif
