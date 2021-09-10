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

    /*״ֵ̬��ʼ�� ȫ����ͣ*/
    REMOTE.state.Independent = 0; //��������
    REMOTE.state.Auto_Run = 0;    //�Զ���
    REMOTE.state.Rotation = 0;    //С����ģʽ
    REMOTE.state.Wiggle = 0;      //Ť��ģʽ

    REMOTE.state.Clap_Init = 0;      //��ȡ̧����ʼ��
    REMOTE.state.Auto_One_Box = 0;   //��ȡһ��
    REMOTE.state.Auto_Two_Box = 0;   //��ȡ����
    REMOTE.state.Auto_Three_Box = 0; //��ȡ����
    REMOTE.state.Flip_Reset = 0;     //��ת��λ
    REMOTE.state.All_Reset = 0;      //��װ��λ
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
    REMOTE.RC_ctrl->s1 = REMOTE.RC_ctrl->s2 = 2; //�˼�λΪ�ϵ��λ

    /*��ͨ�˲���ʼ��*/
    First_Order_Init(&REMOTE.RC_X, 0.08);
    First_Order_Init(&REMOTE.RC_Y, 0.08);
    First_Order_Init(&REMOTE.RC_Z, 0.08);

    First_Order_Init(&REMOTE.KM_X, 0.08);
    First_Order_Init(&REMOTE.KM_Y, 0.08);
    First_Order_Init(&REMOTE.KM_Z, 0.08);

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
    REMOTE.RC_ctrl->ch0 = Dead_Zone(REMOTE.RC_ctrl->ch0, DeadZone);
    REMOTE.RC_ctrl->ch1 = Dead_Zone(REMOTE.RC_ctrl->ch1, DeadZone);
    REMOTE.RC_ctrl->ch2 = Dead_Zone(REMOTE.RC_ctrl->ch2, DeadZone);

    /*�˲�����*/
    First_Order(&REMOTE.RC_X, REMOTE.RC_ctrl->ch1);
    First_Order(&REMOTE.RC_Y, REMOTE.RC_ctrl->ch0);
    First_Order(&REMOTE.RC_Z, REMOTE.RC_ctrl->ch2);
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
    uint16_t key = REMOTE.RC_ctrl->KV.key;

    /*************************************����ǰ��*********************************/
    if (key & KEY_PRESSED_OFFSET_W)
    {
        if (key & KEY_PRESSED_OFFSET_SHIFT && key & KEY_PRESSED_OFFSET_W)
        {
            REMOTE.RC_ctrl->KV.kv0 = 6000;
        }
        else
        {
            REMOTE.RC_ctrl->KV.kv0 = 3000;
        }
    }
    else if (key & KEY_PRESSED_OFFSET_S)
    {
        if (key & KEY_PRESSED_OFFSET_SHIFT && key & KEY_PRESSED_OFFSET_S)
        {
            REMOTE.RC_ctrl->KV.kv0 = -6000;
        }
        else
        {
            REMOTE.RC_ctrl->KV.kv0 = -3000;
        }
    }
    else
    {
        REMOTE.RC_ctrl->KV.kv0 = 0;
    }

    /**************************************��������*********************************/
    if (key & KEY_PRESSED_OFFSET_A)
    {
        if (key & KEY_PRESSED_OFFSET_SHIFT && key & KEY_PRESSED_OFFSET_A)
        {
            REMOTE.RC_ctrl->KV.kv1 = -6000;
        }
        else
        {
            REMOTE.RC_ctrl->KV.kv1 = -3000;
        }
    }
    else if (key & KEY_PRESSED_OFFSET_D)
    {
        if (key & KEY_PRESSED_OFFSET_SHIFT && key & KEY_PRESSED_OFFSET_D)
        {
            REMOTE.RC_ctrl->KV.kv1 = 6000;
        }
        else
        {
            REMOTE.RC_ctrl->KV.kv1 = 3000;
        }
    }
    else
    {
        REMOTE.RC_ctrl->KV.kv1 = 0;
    }

    /*************************************���X��************************************/
    REMOTE.RC_ctrl->KV.kv3 = (float)REMOTE.RC_ctrl->KV.x * 5.0f;
    /**************************************���Y��***********************************/
    REMOTE.RC_ctrl->KV.kv2 = (float)REMOTE.RC_ctrl->KV.y * 5.0f;

    /*******************************��ȡ��ʼ��********************************/
    if (key & KEY_PRESSED_OFFSET_Q)
    {
        if (key & KEY_PRESSED_OFFSET_Q && key & KEY_PRESSED_OFFSET_CTRL)
        {
            REMOTE.state.Clap_Init = 0; // ̧����λ
        }
        else
        {
            REMOTE.state.Clap_Init = 1; // ̧������
        }
    }

    /********************************��ȡ��һ��*******************************/
    if (key & KEY_PRESSED_OFFSET_E)
    {

        REMOTE.state.Auto_One_Box = 1; //��ȡһ������
        REMOTE.state.Auto_Two_Box = 2;
        REMOTE.state.Auto_Three_Box = 2;

        REMOTE.state.Flip_Reset = 2; //��ת��λ�ر�
        REMOTE.state.All_Reset = 2;  //��װ��λ�ر�
    }

    /********************************��ת��λ*******************************/
    if (key & KEY_PRESSED_OFFSET_R)
    {

        if (key & KEY_PRESSED_OFFSET_R && key & KEY_PRESSED_OFFSET_CTRL)
        {
            /*******************************��װ��λ*******************************/
            REMOTE.state.Auto_One_Box = 2; //��ȡһ��ر�
            REMOTE.state.Auto_Two_Box = 2;
            REMOTE.state.Auto_Three_Box = 2;

            REMOTE.state.Flip_Reset = 2; //��ת��λ�ر�
            REMOTE.state.All_Reset = 1;  //��װ��λ
        }
        else
        {
            REMOTE.state.Auto_One_Box = 2; //��ȡһ��ر�
            REMOTE.state.Auto_Two_Box = 2;
            REMOTE.state.Auto_Three_Box = 2;

            REMOTE.state.Flip_Reset = 1; //��ת��λ
            REMOTE.state.All_Reset = 2;  //��װ��λ�ر�
        }
    }

    /*********************************RFID********************************/
    if (key & KEY_PRESSED_OFFSET_F)
    {

        if (key & KEY_PRESSED_OFFSET_F && key & KEY_PRESSED_OFFSET_CTRL)
        {
            REMOTE.state.RFID = 0; //RFID��
        }
        else
        {
            REMOTE.state.RFID = 1; //RFID��
        }
    }

    /*********************************��Ԯצ********************************/
    if (key & KEY_PRESSED_OFFSET_G)
    {

        if (key & KEY_PRESSED_OFFSET_G && key & KEY_PRESSED_OFFSET_CTRL)
        {
            REMOTE.state.Barrier = 0; //��Ԯצ��
        }
        else
        {
            REMOTE.state.Barrier = 1; //��Ԯצ��
        }
    }

    /* ========================================����Ϊ����====================================== */

    /*********************************����**********************************/
    if (key & KEY_PRESSED_OFFSET_SHIFT)
    {

        REMOTE.state.Independent = 1; //���������˶�
        REMOTE.state.Auto_Run = 2;    //�Զ�����ͣ
        REMOTE.state.Rotation = 2;    //С����ģʽ��ͣ
        REMOTE.state.Wiggle = 2;      //Ť��ģʽ��ͣ
    }

    /*********************************������**********************************/
    if (key & KEY_PRESSED_OFFSET_Z)
    {

        REMOTE.state.Independent = 2; //����������ͣ
        REMOTE.state.Auto_Run = 2;    //�Զ�����ͣ
        REMOTE.state.Rotation = 1;    //������ģʽ
        REMOTE.state.Wiggle = 2;      //Ť��ģʽ��ͣ
    }

    /**********************************Ť��**********************************/
    if (key & KEY_PRESSED_OFFSET_X)
    {

        REMOTE.state.Independent = 2; //����������ͣ
        REMOTE.state.Auto_Run = 2;    //�Զ�����ͣ
        REMOTE.state.Rotation = 2;    //С����ģʽ��ͣ
        REMOTE.state.Wiggle = 1;      //Ť��ģʽ
    }

    /*******************************�����Զ���ȡ********************************/
    if (key & KEY_PRESSED_OFFSET_C)
    {

        REMOTE.state.Independent = 2; //����������ͣ
        REMOTE.state.Auto_Run = 1;    //�Զ���
        REMOTE.state.Rotation = 2;    //С����ģʽ��ͣ
        REMOTE.state.Wiggle = 2;      //Ť��ģʽ��ͣ
    }

    /* ======================================����Ϊ����===================================== */

    /********************************��ȡ�ڶ���********************************/
    if (key & KEY_PRESSED_OFFSET_V)
    {

        REMOTE.state.Auto_One_Box = 2;   //��ȡһ��ر�
        REMOTE.state.Auto_Two_Box = 1;   //��ȡ���俪��
        REMOTE.state.Auto_Three_Box = 2; //��ȡ����ر�

        REMOTE.state.Flip_Reset = 2; //��ת��λ�ر�
        REMOTE.state.All_Reset = 2;  //��װ��λ�ر�
    }

    /********************************��ȡ������********************************/
    if (key & KEY_PRESSED_OFFSET_B)
    {
        REMOTE.state.Auto_One_Box = 2;   //��ȡһ��ر�
        REMOTE.state.Auto_Two_Box = 2;   //��ȡ���俪��
        REMOTE.state.Auto_Three_Box = 1; //��ȡ���俪��

        REMOTE.state.Flip_Reset = 2; //��ת��λ�ر�
        REMOTE.state.All_Reset = 2;  //��װ��λ�ر�
    }

    /*���Ʒ��ȴ���*/
    REMOTE.RC_ctrl->KV.kv0 = limit(REMOTE.RC_ctrl->KV.kv0, 660, -660);
    REMOTE.RC_ctrl->KV.kv1 = limit(REMOTE.RC_ctrl->KV.kv1, 660, -660);
    REMOTE.RC_ctrl->KV.kv2 = limit(REMOTE.RC_ctrl->KV.kv2, 660, -660);
    REMOTE.RC_ctrl->KV.kv3 = limit(REMOTE.RC_ctrl->KV.kv3, 660, -660);
    /*�˲�����*/
    First_Order(&REMOTE.KM_X, REMOTE.RC_ctrl->KV.kv0); //��������
    First_Order(&REMOTE.KM_Y, REMOTE.RC_ctrl->KV.kv1); //����ǰ��
    First_Order(&REMOTE.KM_Z, REMOTE.RC_ctrl->KV.kv3); // ���x��
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

    if (_s1 != REMOTE.RC_ctrl->s1)
        Remote_Data_Zero();

    if (REMOTE.RC_ctrl->Flag)
    {
        if (REMOTE.RC_ctrl->s1 == 3)
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
