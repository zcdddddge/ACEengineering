#include "BoardCommuni.h"
#include "CAN2_ISR.h"

Board_Communi_t Board_Communi;

void Board_Communi_Init(void)
{
	Board_Communi.Rc = Return_CAN2_Board_Data();
}

void Board_Communi_Updata(void)
{
	static int16_t clock[2] = {0,0};
	
	if(Board_Communi.Rc[16] == 1)
	{
		Board_Communi.Can_RC.ch0 = (int16_t)(Board_Communi.Rc[1]<<8)|(Board_Communi.Rc[0]);
		Board_Communi.Can_RC.ch1 = (int16_t)(Board_Communi.Rc[3]<<8)|(Board_Communi.Rc[2]);
		Board_Communi.Can_RC.ch2 = (int16_t)(Board_Communi.Rc[5]<<8)|(Board_Communi.Rc[4]);
		Board_Communi.Can_RC.ch3 = (int16_t)(Board_Communi.Rc[7]<<8)|(Board_Communi.Rc[6]);
		Board_Communi.Rc[16] = 0;
		clock[0] = 0;
	}
	else
	{
		clock[0] ++;
		if(clock[0] >= 500)
		{
			Board_Communi.Can_RC.ch0 = Board_Communi.Can_RC.ch1 = Board_Communi.Can_RC.ch2 = Board_Communi.Can_RC.ch3 = 0;
		}
	}
	
	if(Board_Communi.Rc[17] == 1)
	{
		Board_Communi.Can_RC.sw = -((int16_t)(Board_Communi.Rc[9]<<8)|(Board_Communi.Rc[8]));
		Board_Communi.Can_RC.s1 = (int8_t)Board_Communi.Rc[10];
		Board_Communi.Can_RC.s2 = (int8_t)Board_Communi.Rc[11];
		Board_Communi.Can_RC.KV.key = (int16_t)(Board_Communi.Rc[13]<<8)|(Board_Communi.Rc[12]);
		Board_Communi.Rc[17] = 0;
		clock[1] = 0;
	}
	else
	{
		clock[1] ++;
		if(clock[1] >= 500)
		{
			Board_Communi.Can_RC.sw = 0;
			Board_Communi.Can_RC.KV.key = 0;
			Board_Communi.Can_RC.s1 = Board_Communi.Can_RC.s2 = 0;
		}
	}
}


Board_Communi_t* Return_Board_Communi(void)
{
	return &Board_Communi;
}