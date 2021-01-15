#include "BoardCommuni.h"
#include "CAN2_ISR.h"

RC_ctrl_t *rc;
u8 temp[8];

/*板间通信初始化*/
void BoardCommuni_Init(void)
{
	rc = Return_Remote_Point();
}

/*发送遥控数据*/
void Send_RC_To_Board(void)
{
	temp[0] = rc->ch0;
	temp[1] = (rc->ch0>>8);
	temp[2] = rc->ch1;
	temp[3] = (rc->ch1>>8);
	temp[4] = rc->ch2;
	temp[5] = (rc->ch2>>8);
	temp[6] = rc->ch3;
	temp[7] = (rc->ch3>>8);
	CAN2_To_Board(temp,0x0401);
	
	temp[0] = rc->sw;
	temp[1] = (rc->sw>>8);
	temp[2] = rc->s1;
	temp[3] = rc->s2;
	temp[4] = rc->KV.key;
	temp[5] = (rc->KV.key>>8);;
	temp[6] = 0;
	temp[7] = 0;
	CAN2_To_Board(temp,0x0402);
}

/*发送控制指令至板子*/
void Send_Ctrl_To_Board(unsigned char boxs,unsigned char Magazine)
{
	temp[0] = 0x0A;
	temp[1] = boxs;
	temp[2] = Magazine;
	temp[3] = 0;
	temp[4] = 0;
	temp[5] = 0;
	temp[6] = 0;
	temp[7] = 0xCE;
	//CAN2_To_Board(temp,0x0401); // 修改 
	CAN2_To_Board(temp,0x0403);
}



