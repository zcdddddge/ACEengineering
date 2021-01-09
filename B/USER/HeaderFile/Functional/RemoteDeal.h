#ifndef __REMOTEDEAL_H_
#define __REMOTEDEAL_H_
#include "REMOTE_ISR.h"
#include "FilterLib.h"



/* ----------------------- PC Key Definition-------------------------------- */
#define KEY_PRESSED_OFFSET_W ((uint16_t)0x01<<0) 														
#define KEY_PRESSED_OFFSET_S ((uint16_t)0x01<<1) 														
#define KEY_PRESSED_OFFSET_A ((uint16_t)0x01<<2)
#define KEY_PRESSED_OFFSET_D ((uint16_t)0x01<<3)
#define KEY_PRESSED_OFFSET_SHIFT ((uint16_t)0x01<<4)
#define KEY_PRESSED_OFFSET_CTRL ((uint16_t)0x01<<5)
#define KEY_PRESSED_OFFSET_Q ((uint16_t)0x01<<6)
#define KEY_PRESSED_OFFSET_E ((uint16_t)0x01<<7)
#define KEY_PRESSED_OFFSET_R ((uint16_t)0x01<<8)
#define KEY_PRESSED_OFFSET_F ((uint16_t)0x01<<9)
#define KEY_PRESSED_OFFSET_G ((uint16_t)0x01<<10)
#define KEY_PRESSED_OFFSET_Z ((uint16_t)0x01<<11)
#define KEY_PRESSED_OFFSET_X ((uint16_t)0x01<<12)
#define KEY_PRESSED_OFFSET_C ((uint16_t)0x01<<13)
#define KEY_PRESSED_OFFSET_V ((uint16_t)0x01<<14)
#define KEY_PRESSED_OFFSET_B ((uint16_t)0x01<<15)
/* ----------------------- PC Key Definition-------------------------------- */

/*���̿���״̬�ṹ��*/
typedef __packed struct
{
	unsigned char Electromagnet : 1;
	unsigned char RFID : 1;
	unsigned char Magazine : 1;
	unsigned char Auto_Clamp : 5;
}KeyBoard_State_t;

/*Remote�ṹ��*/
typedef __packed struct
{
	RC_ctrl_t *RC_ctrl;
	First_Order_t RC_X;
	First_Order_t RC_Y;
	First_Order_t RC_Z;
	First_Order_t KM_X;
	First_Order_t KM_Y;
	First_Order_t KM_Z;
	KeyBoard_State_t state;
}REMOTE_t;


/*Remote�ṹ���ʼ��*/
void Remote_Data_Init(void);


/*Remote���ݴ���*/
void Remote_Data_Deal(void);


/*����Remote����ָ��*/
REMOTE_t *Return_RemoteDeal_Point(void);


#endif
