/*
 * @Author: your name
 * @Date: 2021-01-09 15:32:34
 * @LastEditTime: 2021-01-12 11:19:43
 * @LastEditors: Please set LastEditors
 * @Description: ������ң����ֵ��ʼ��,����,���̴���ң�ش���,ģʽ�л�����ĺ���
 */
#include "RemoteDeal.h"
#include "MathLib.h"

/*ң�ؽṹ��*/
REMOTE_t REMOTE;
/*ң������*/
static const u8 DeadZone = 10;




/*************************************************************************************************
*����:	Get_RemoteDeal_Point
*����:	���ش�����ң����ֵ���Ʊ�����ͨ��ָ�봫�ݷ�ʽ������Ϣ
*�β�: 	��
*����:	��
*˵��:	��
*************************************************************************************************/
REMOTE_t *Return_RemoteDeal_Point(void)
{		
    return &REMOTE;
}



/*************************************************************************************************
*����:	Remote_Data_Zero
*����:	ң����ֵ����
*�β�: 	��
*����:	��
*˵��:	��
*************************************************************************************************/
static void Remote_Data_Zero(void)
{
		/*ң����ֵ��ʼ��*/
		REMOTE.RC_ctrl->ch0 = REMOTE.RC_ctrl->ch1 = REMOTE.RC_ctrl->ch2 = REMOTE.RC_ctrl->ch3 = 0;
		REMOTE.RC_ctrl->sw = REMOTE.RC_ctrl->Flag = 0;
	  REMOTE.RC_ctrl->KV.x = REMOTE.RC_ctrl->KV.y = REMOTE.RC_ctrl->KV.z = 0;
		REMOTE.RC_ctrl->KV.key = REMOTE.RC_ctrl->KV.press_l = REMOTE.RC_ctrl->KV.press_r = 0;
	
		/*״ֵ̬��ʼ��*/
		REMOTE.state.Electromagnet = REMOTE.state.Magazine = REMOTE.state.RFID = 0;
		REMOTE.state.Auto_Clamp = 0;
}



/*************************************************************************************************
*����:	Remote_Data_Init
*����:	ң����ֵ�����ʼ��
*�β�: 	��
*����:	��
*˵��:	��
*************************************************************************************************/
void Remote_Data_Init(void)
{
		/*ң����ֵ����*/
		Remote_Data_Zero();
		REMOTE.RC_ctrl->s1 = REMOTE.RC_ctrl->s2 = 2;			//�˼�λΪ�ϵ��λ
	
		/*��ͨ�˲���ʼ��*/
		First_Order_Init(&REMOTE.RC_X,0.08);
		First_Order_Init(&REMOTE.RC_Y,0.08);
		First_Order_Init(&REMOTE.RC_Z,0.08);

		First_Order_Init(&REMOTE.KM_X,0.08);
		First_Order_Init(&REMOTE.KM_Y,0.08);
		First_Order_Init(&REMOTE.KM_Z,0.08);
	
		/*��ȡң��ָ��*/
		REMOTE.RC_ctrl = Return_Remote_Point();
}



/*************************************************************************************************
*����:	Rc_Deal
*����:	����ң�˲��˵�ֵ
*�β�: 	��
*����:	��
*˵��:	��
*************************************************************************************************/
static void Rc_Deal(void)
{
		/*��������*/
		REMOTE.RC_ctrl->ch0 = Dead_Zone(REMOTE.RC_ctrl->ch0,DeadZone);
		REMOTE.RC_ctrl->ch1 = Dead_Zone(REMOTE.RC_ctrl->ch1,DeadZone);
		REMOTE.RC_ctrl->ch2 = Dead_Zone(REMOTE.RC_ctrl->ch2,DeadZone);
	
		/*�˲�����*/
		First_Order(&REMOTE.RC_X,REMOTE.RC_ctrl->ch1);
		First_Order(&REMOTE.RC_Y,REMOTE.RC_ctrl->ch0);
		First_Order(&REMOTE.RC_Z,REMOTE.RC_ctrl->ch2);	
}



/*************************************************************************************************
*����:	KEY_MOUSE
*����:	�����������ֵ
*�β�: 	��
*����:	��
*˵��:	��֧�ṹ�����Ż�
*************************************************************************************************/
static void Key_Mouse_Deal(void)
{
		static unsigned char lock = 1;
		uint16_t key = REMOTE.RC_ctrl->KV.key;
	
		/*************************************����ǰ��*********************************/
		if(key & KEY_PRESSED_OFFSET_W)
		{
			REMOTE.RC_ctrl->KV.kv0 += 10;
		}
		else if(key & KEY_PRESSED_OFFSET_S)
		{
			REMOTE.RC_ctrl->KV.kv0 -= 10;
		}
		else
		{
			REMOTE.RC_ctrl->KV.kv0 = 0;
		}
		/**************************************��������*********************************/
		if(key & KEY_PRESSED_OFFSET_A)
		{
			REMOTE.RC_ctrl->KV.kv1 -= 10;
		}
		else if(key & KEY_PRESSED_OFFSET_D)
		{
			REMOTE.RC_ctrl->KV.kv1 += 10;
		}
		else
		{
			REMOTE.RC_ctrl->KV.kv1 = 0;
		}
		
		/*************************************���X��************************************/
		REMOTE.RC_ctrl->KV.kv3 = (float)REMOTE.RC_ctrl->KV.x * 5.0f;
  		/**************************************���Y��***********************************/
		REMOTE.RC_ctrl->KV.kv2 = (float)REMOTE.RC_ctrl->KV.y * 5.0f;
		/****************************************************************************/
		switch  (key)
		{
		/*************************************�����**********************************/
			case KEY_PRESSED_OFFSET_Z:
			{
				if(lock)
				{
					REMOTE.state.Electromagnet = 1 - REMOTE.state.Electromagnet;
					lock = 0;
				}
				break;
			}
		/*************************************��Ԯ��**********************************/
			case KEY_PRESSED_OFFSET_X:
			{
				if(lock)
				{
					REMOTE.state.RFID = 1 - REMOTE.state.RFID;
					lock = 0;
				}
				break;
			}
		/*************************************�Զ���ȡ********************************/
			case KEY_PRESSED_OFFSET_C:
			{
				if(lock)
				{
					REMOTE.state.Auto_Clamp = 1-REMOTE.state.Auto_Clamp ;
					lock = 0;
				}
				break;
			}
			/*************************************����********************************/
			case KEY_PRESSED_OFFSET_V:
			{
				if(lock)
				{
					REMOTE.state.Magazine = 1 - REMOTE.state.Magazine;
					lock = 0;
				}
				break;
			}
			/********************************A��********************************/
			case KEY_PRESSED_OFFSET_B :
			{
				if(lock) {
					REMOTE.state.Clip    =1-REMOTE.state.Clip ;
					lock=0 ;
				}
				break; 
			}
			default:
			{
//				REMOTE.state.Auto_Clamp = 0;
				lock = 1;
				break;
			}
		}
		/*���Ʒ��ȴ���*/
		REMOTE.RC_ctrl->KV.kv0 = limit(REMOTE.RC_ctrl->KV.kv0,660,-660);
		REMOTE.RC_ctrl->KV.kv1 = limit(REMOTE.RC_ctrl->KV.kv1,660,-660);
		REMOTE.RC_ctrl->KV.kv2 = limit(REMOTE.RC_ctrl->KV.kv2,660,-660);
		REMOTE.RC_ctrl->KV.kv3 = limit(REMOTE.RC_ctrl->KV.kv3,660,-660);
		/*�˲�����*/
		First_Order(&REMOTE.KM_X,REMOTE.RC_ctrl->KV.kv0);     //�������� 
		First_Order(&REMOTE.KM_Y,REMOTE.RC_ctrl->KV.kv1);     //����ǰ��
		First_Order(&REMOTE.KM_Z,REMOTE.RC_ctrl->KV.kv3);     // ���x�� 
		
}



/*************************************************************************************************
*����:	RC_DATA_DEAL
*����:	ң����ֵ����
*�β�: 	��
*����:	��
*˵��:	��
*************************************************************************************************/
void Remote_Data_Deal(void)
{
	static unsigned char _s1 = 0;
	if(_s1 != REMOTE.RC_ctrl->s1)
			Remote_Data_Zero();
	
	if(REMOTE.RC_ctrl->Flag)
	{
		if(REMOTE.RC_ctrl->s1 == 3)
		{
			Key_Mouse_Deal();
		}
		else
		{
			Rc_Deal();
		}
		REMOTE.RC_ctrl->Flag = 0;
	}
	
	_s1 = REMOTE.RC_ctrl->s1;
}









