#include "CAN2_ISR.h"

/*反馈数据区*/
u8 can2_board_buffer_first[18];
int16_t can2_201_208_buffer[16];

/*************************************************************************************************
*名称:	CAN2_RX0_IRQHandler
*功能:	can2 接收函数
*形参: 	无
*返回:	无
*说明:	无
*************************************************************************************************/
void CAN2_RX0_IRQHandler(void)
{
    CanRxMsg RxMessage;

    if (CAN_GetITStatus(CAN2, CAN_IT_FMP0) != RESET)
    {
        CAN_ClearITPendingBit(CAN2, CAN_IT_FMP0);
        CAN_Receive(CAN2, CAN_FIFO0, &RxMessage);

        switch (RxMessage.StdId)
        {
        case 0x401:
        {
            can2_board_buffer_first[0] = RxMessage.Data[0];
            can2_board_buffer_first[1] = RxMessage.Data[1];
            can2_board_buffer_first[2] = RxMessage.Data[2];
            can2_board_buffer_first[3] = RxMessage.Data[3];
            can2_board_buffer_first[4] = RxMessage.Data[4];
            can2_board_buffer_first[5] = RxMessage.Data[5];
            can2_board_buffer_first[6] = RxMessage.Data[6];
            can2_board_buffer_first[7] = RxMessage.Data[7];
            can2_board_buffer_first[16] = 1;
            break;
        }

        case 0x402:
        {
            can2_board_buffer_first[8] = RxMessage.Data[0];
            can2_board_buffer_first[9] = RxMessage.Data[1];
            can2_board_buffer_first[10] = RxMessage.Data[2];
            can2_board_buffer_first[11] = RxMessage.Data[3];
            can2_board_buffer_first[12] = RxMessage.Data[4];
            can2_board_buffer_first[13] = RxMessage.Data[5];
            can2_board_buffer_first[17] = 1;
            break;
        }

        /****1/13 增加夹取 **************/
        case 0x403:
        {
            if (RxMessage.Data[0] == 0x0A && RxMessage.Data[7] == 0xCE)
            {
                can2_board_buffer_first[0] = RxMessage.Data[1]; //夹取抬升初始化 Clap_Init
                can2_board_buffer_first[1] = RxMessage.Data[2]; //夹取一箱   Auto_One_Box
                can2_board_buffer_first[2] = RxMessage.Data[3]; //夹取两箱   Auto_Two_Box
                can2_board_buffer_first[3] = RxMessage.Data[4]; //夹取三箱   Auto_Three_Box
                can2_board_buffer_first[4] = RxMessage.Data[5]; //翻转复位   Flip_Reset
                can2_board_buffer_first[5] = RxMessage.Data[6]; //上装复位  All_Reset
                can2_board_buffer_first[17] = 2;
            }
        }

        /****************1-17增加键盘控制夹取***/
        case 0x404:
        {
            if (RxMessage.Data[7] == 0x77)
            {
                can2_board_buffer_first[0] = RxMessage.Data[0];
                can2_board_buffer_first[1] = RxMessage.Data[1];
                can2_board_buffer_first[2] = RxMessage.Data[2];
                can2_board_buffer_first[3] = RxMessage.Data[3];
                can2_board_buffer_first[4] = RxMessage.Data[4];
                can2_board_buffer_first[5] = RxMessage.Data[5];
                can2_board_buffer_first[6] = RxMessage.Data[6];
                can2_board_buffer_first[7] = RxMessage.Data[7];
            }
        }

        case 0x405:
        {
            if (RxMessage.Data[6] == 0xAC && RxMessage.Data[7] == 0xCE)
            {
                can2_board_buffer_first[0] = RxMessage.Data[0];
                can2_board_buffer_first[1] = RxMessage.Data[1];
            }
        }

        default:
            break;
        }
    }
}

/*************************************************************************************************
*名称:	CAN2_To_Board
*功能:	发送用户发送自定义数据
*形参: 	unsigned char*CAN_DATA
*返回:	无
*说明:	自己定义标识符
*************************************************************************************************/
void CAN2_To_Board(u8 *CAN_DATA, int16_t stdid)
{
    u8 mbox;
    u16 i = 0;
    CanTxMsg TxMessage; //定义一个发送信息的结构体

    TxMessage.StdId = stdid;      //根据820r设置标识符
    TxMessage.IDE = CAN_ID_STD;   //指定将要传输的消息的标识符的类型
    TxMessage.RTR = CAN_RTR_DATA; //指定的帧将被传输的消息的类型   数据帧或远程帧
    TxMessage.DLC = 8;            //指定数据的长度
    TxMessage.Data[0] = (unsigned char)(CAN_DATA[0]);
    TxMessage.Data[1] = (unsigned char)(CAN_DATA[1]);
    TxMessage.Data[2] = (unsigned char)(CAN_DATA[2]);
    TxMessage.Data[3] = (unsigned char)(CAN_DATA[3]);
    TxMessage.Data[4] = (unsigned char)(CAN_DATA[4]);
    TxMessage.Data[5] = (unsigned char)(CAN_DATA[5]);
    TxMessage.Data[6] = (unsigned char)(CAN_DATA[6]);
    TxMessage.Data[7] = (unsigned char)(CAN_DATA[7]);
    mbox = CAN_Transmit(CAN2, &TxMessage); //发送信息
    i = 0;

    while ((CAN_TransmitStatus(CAN2, mbox) == CAN_TxStatus_Failed) && (i < 0XFFF))
        i++;
}

/*************************************************************************************************
*名称:	Return_CAN2_RC_DATA
*功能:	返回CAN2接收数据指针
*形参: 	无
*返回:	无
*说明:	返回夹取板子的状态值
*************************************************************************************************/
u8 *Return_CAN2_Board_Data(void)
{
    return can2_board_buffer_first;
}
