#include "BoardCommuni.h"
#include "CAN2_ISR.h"

RC_ctrl_t *rc;
/*���ͨ�Ż�����--B�嵽A��*/
u8 temp[8]; //uint8_t

/*���ͨ�Ż�����A�嵽B��*/
uint8_t *Can2_temp;

u8 Smooth_L;
u8 Smooth_R;

/*���ͨ�ų�ʼ��*/
void BoardCommuni_Init(void)
{
    rc = Return_Remote_Point();
    Can2_temp = Return_CAN2_Board_Data();
}

void BoardCommuni_DataUpdate(int16_t *speed)
{

    if (Can2_temp[0] == 0xAC && Can2_temp[7] == 0xCE)
    {
        *speed = (int16_t)(Can2_temp[1] | (Can2_temp[2] << 8));
        Smooth_L = Can2_temp[3];
        Smooth_R = Can2_temp[4];
    }
    else
    {
        *speed = 0;
    }
}

/*����--δ����*/
void Send_KeyBoard_TO_Board(void)
{
    temp[0] = rc->KV.x;
    temp[1] = (rc->KV.x >> 8);
    temp[2] = rc->KV.y;
    temp[3] = (rc->KV.y >> 8);
    temp[4] = rc->KV.press_l;
    temp[5] = rc->KV.kv0;
    temp[6] = (rc->KV.kv0 >> 8);
    temp[7] = 0x77;
    CAN2_To_Board(temp, 0x0404);

    temp[0] = rc->KV.kv1;
    temp[1] = (rc->KV.kv1 >> 8);
    temp[2] = 0;
    temp[3] = 0;
    temp[4] = 0;
    temp[5] = 0;
    temp[6] = 0xAC;
    temp[7] = 0xCE;
    CAN2_To_Board(temp, 0x0405);
}

/*����ң������*/
void Send_RC_To_Board(void)
{
    temp[0] = rc->ch0;        // ch0 ��8λ
    temp[1] = (rc->ch0 >> 8); //ch0 ��8λ
    temp[2] = rc->ch1;
    temp[3] = (rc->ch1 >> 8);
    temp[4] = rc->ch2;
    temp[5] = (rc->ch2 >> 8);
    temp[6] = rc->ch3;
    temp[7] = (rc->ch3 >> 8);
    CAN2_To_Board(temp, 0x0401);

    temp[0] = rc->sw;
    temp[1] = (rc->sw >> 8);
    temp[2] = rc->s1;
    temp[3] = rc->s2;
    temp[4] = rc->KV.key;
    temp[5] = (rc->KV.key >> 8);
    temp[6] = 0;
    temp[7] = 0;
    CAN2_To_Board(temp, 0x0402);
}

/*���Ϳ���ָ��������*/
void Send_Ctrl_To_Board(unsigned char Clap_Init, unsigned char Auto_One_Box, unsigned char Auto_Two_Box, unsigned char Auto_Three_Box, unsigned char Flip_Reset, unsigned char All_Reset)
{
    temp[0] = 0x0A;
    temp[1] = Clap_Init;
    temp[2] = Auto_One_Box;
    temp[3] = Auto_Two_Box;
    temp[4] = Auto_Three_Box;
    temp[5] = Flip_Reset;
    temp[6] = All_Reset;
    temp[7] = 0xCE;
    CAN2_To_Board(temp, 0x0403);
}
