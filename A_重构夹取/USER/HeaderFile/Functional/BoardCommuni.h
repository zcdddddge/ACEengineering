#ifndef __BOARDCOMMUNI_H_
#define __BOARDCOMMUNI_H_
#include "stm32f4xx.h"
#include "REMOTE_ISR.h"

#define AUTO_GRASP_S1     3
#define AUTO_GRASP_S2     3 





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
void Send_Crtl_To_Board(int16_t ch0);

#endif
