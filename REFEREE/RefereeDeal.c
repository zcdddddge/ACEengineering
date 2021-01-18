#include "RefereeDeal.h"
#include "usart.h"
#include "CRC.h"


/**
  * @brief          ��������ʼ��
  * @param[in]      none
  * @retval         none
  * @attention  
  */
void Referee_System_Init(void)
{
	Usart6_Init();  //����ϵͳ
}

//static
REFEREE_t REFEREE;    //����ϵͳ���ݽṹ��
xFrameHeader_t FrameHeader;  //����֡ͷ��Ϣ


/**
  * @brief          ��ò���ϵͳ���ݿ�����
  * @param[in]      none
  * @retval         ����ϵͳ���ݿ���ָ�� &REFEREE
  * @attention  
  */
REFEREE_t *Return_Referee_Point(void)
{
    return &REFEREE;
}


/*���ݶγ���*/
static const u8 HeaderLen = 5;
static const u8 CmdIdLen = 2;
static const u8 CRC16Len = 2;

bool_t Judge_Data_TF = 0;//���������Ƿ����,������������

#if JUDGE_VERSION == JUDGE_SZ

#elif JUDGE_VERSION == JUDGE_ACE
/*����״̬*/
static void GAME_STATUS(REFEREE_t *referee, unsigned char k);
/*������״̬*/
static void ROBOT_STATUS(REFEREE_t *referee, unsigned char k);
/*��������*/
static void POWER_HEAT(REFEREE_t *referee, unsigned char k);
/*�˺�״̬*/
static void DAMAGE_STATUS(REFEREE_t *referee, unsigned char k);

/*����״̬���ݶγ���*/
static const u8 GameStatusLen = (3 + CmdIdLen + CRC16Len);
/*������״̬���ݶγ���*/
static const u8 RobotStatusLen = (18 + CmdIdLen + CRC16Len);
/*�����������ݶγ���*/
static const u8 PowerHeatLen = (16 + CmdIdLen + CRC16Len);

/*�������ݽ������ݴ���*/
void RefereeDataDeal(REFEREE_t *referee)
{
    u8 i, k;

    for (i = 0; i < referee->DataLen; i++)
    {
        if (referee->RefereeData[i] == 0xA5) //֡ͷ
        {
            if (Verify_CRC8_Check_Sum(referee->RefereeData, HeaderLen) == 1) //CRC8У��
            {
                referee->RealLen = ((referee->RefereeData[i + 1]) | (referee->RefereeData[i + 2] << 8)); //����֡�� data �ĳ���
                referee->Cmd_ID = ((referee->RefereeData[i + HeaderLen]) | (referee->RefereeData[i + HeaderLen + 1] << 8)); //������ID

                for (k = 0; k < 7; k++)
                    referee->RealData[k] = referee->RefereeData[k + i]; //��ȡ���� Ҳ���ǽ��գ�frame_header+cmd_id

				/* �ж�cmd_id�����ն�Ӧ��������Ϣ */
                if (referee->Cmd_ID == 0x0001)
                {
                    GAME_STATUS(referee, (i + HeaderLen + CmdIdLen)); //����״̬���ݴ���
                    i = i + HeaderLen + GameStatusLen - 1;
                }
				else if (referee->Cmd_ID == 0x0002) //�����������
				{
				}
                else if (referee->Cmd_ID == 0x0003) //������Ѫ��
                {
                }
                else if (referee->Cmd_ID == 0x0004) //���ڷ���״̬
                {
                }
                else if (referee->Cmd_ID == 0x0101) //�����¼�����
                {
                }
                else if (referee->Cmd_ID == 0x0102) //���ز���վ������ʶ���ݣ�������������
                {
                }
				else if (referee->Cmd_ID == 0x0104) //���о������ݣ����淢������
                {
                }
                else if (referee->Cmd_ID == 0x0105) //���ڷ���ڵ���ʱ��1Hz ���ڷ���
                {
                }
                else if (referee->Cmd_ID == 0x0201) //������״̬����
                {
                    ROBOT_STATUS(referee, (i + HeaderLen + CmdIdLen));
                    i = i + HeaderLen + RobotStatusLen - 1;
                }
                else if (referee->Cmd_ID == 0x0202) //������������
                {
                    POWER_HEAT(referee, (i + HeaderLen + CmdIdLen));
                    i = i + HeaderLen + PowerHeatLen - 1;
                }
                else if (referee->Cmd_ID == 0x0203) //������λ�����ݣ�10Hz ����
                {
                }
                else if (referee->Cmd_ID == 0x0204) //��������������
                {
                }
                else if (referee->Cmd_ID == 0x0205) //�ɻ�����
                {
                }
                else if (referee->Cmd_ID == 0x0206) //�˺�״̬
                {
					DAMAGE_STATUS(referee, (i + HeaderLen + CmdIdLen));
                }
                else if (referee->Cmd_ID == 0x0207) //ʵʱ�����Ϣ
                {
                }
                else if (referee->Cmd_ID == 0x0208) //�ӵ�ʣ�෢����
                {
                }
                else if (referee->Cmd_ID == 0x0209) //RFID״̬
                {
                }
            }
        }
    }

    REFEREE.RECEIVE_FLAG = 0; //������һ������
}

/*����״̬����*/
static void GAME_STATUS(REFEREE_t *referee, unsigned char k) //0x0001
{
    u8 j;

    for (j = 0; j < GameStatusLen; j++) //��������
        referee->RealData[HeaderLen + CmdIdLen + j] = referee->RefereeData[k + j];  //���� data λ��

    if (Verify_CRC16_Check_Sum(referee->RealData, (HeaderLen + GameStatusLen)) == 1) //CRC16У��
    {
        referee->GameStatus.game_type = referee->RealData[7];
        referee->GameStatus.game_progress = (referee->RealData[7] >> 4);
        referee->GameStatus.stage_remain_time = ((referee->RealData[8]) | (referee->RealData[9] << 8));
        referee->GameStatus.error = 0;
    }
    else
    {
        referee->GameStatus.error = 1;
    }
}

/*������״̬����*/
static void ROBOT_STATUS(REFEREE_t *referee, unsigned char k) //0x0201
{
    u8 j;

    for (j = 0; j < RobotStatusLen; j++) //��������
        referee->RealData[HeaderLen + CmdIdLen + j] = referee->RefereeData[k + j];

    if (Verify_CRC16_Check_Sum(referee->RealData, (HeaderLen + RobotStatusLen)) == 1) //CRC16У��
    {
        referee->RobotStatus.robot_id = referee->RealData[7];
        referee->RobotStatus.robot_level = referee->RealData[8];
        referee->RobotStatus.remain_HP = ((referee->RealData[9]) | (referee->RealData[10] << 8));
        referee->RobotStatus.max_HP = ((referee->RealData[11]) | (referee->RealData[12] << 8));
        referee->RobotStatus.shooter_heat0_cooling_rate = ((referee->RealData[13]) | (referee->RealData[14] << 8));
        referee->RobotStatus.shooter_heat0_cooling_limit = ((referee->RealData[15]) | (referee->RealData[16] << 8));
        referee->RobotStatus.shooter_heat1_cooling_rate = ((referee->RealData[17]) | (referee->RealData[18] << 8));
        referee->RobotStatus.shooter_heat1_cooling_limit = ((referee->RealData[19]) | (referee->RealData[20] << 8));
        referee->RobotStatus.shooter_heat0_speed_limit = (referee->RealData[21]);
        referee->RobotStatus.shooter_heat1_speed_limit = (referee->RealData[22]);
        referee->RobotStatus.max_chassis_power = (referee->RealData[23]);
        referee->RobotStatus.mains_power_gimbal_output = referee->RealData[24];
        referee->RobotStatus.mains_power_chassis_output = (referee->RealData[24] >> 1);
        referee->RobotStatus.mains_power_shooter_output = (referee->RealData[24] >> 2);

        referee->RobotStatus.error = 0;
    }
    else
    {
        referee->RobotStatus.error = 1;
    }
}

/*������������*/
static void POWER_HEAT(REFEREE_t *referee, unsigned char k) //0x0202
{
    u8 j;
    Float_t F;

    for (j = 0; j < PowerHeatLen; j++) //��������
        referee->RealData[HeaderLen + CmdIdLen + j] = referee->RefereeData[k + j];

    if (Verify_CRC16_Check_Sum(referee->RealData, (HeaderLen + PowerHeatLen)) == 1) //CRC16У��
    {
        referee->PowerHeat.chassis_volt = ((referee->RealData[7]) | (referee->RealData[8] << 8));
        referee->PowerHeat.chassis_current = ((referee->RealData[9]) | (referee->RealData[10] << 8));
        F.float_byte.LB = (referee->RealData[11]);
        F.float_byte.MLB = (referee->RealData[12]);
        F.float_byte.MHB = (referee->RealData[13]);
        F.float_byte.HB = (referee->RealData[14]);
        referee->PowerHeat.chassis_power = F.value;
        referee->PowerHeat.chassis_power_buffer = ((referee->RealData[15]) | (referee->RealData[16] << 8));
        referee->PowerHeat.shooter_heat0 = ((referee->RealData[17]) | (referee->RealData[18] << 8));
        referee->PowerHeat.shooter_heat1 = ((referee->RealData[19]) | (referee->RealData[20] << 8));
        referee->PowerHeat.mobile_shooter_heat2 = ((referee->RealData[21]) | (referee->RealData[22] << 8));
		
		referee->PowerHeat.error = 0;
    }
    else
    {
        referee->PowerHeat.error = 1;
    }
}

/*�˺�״̬*/
static void DAMAGE_STATUS(REFEREE_t *referee, unsigned char k)
{
    u8 j;

    for (j = 0; j < PowerHeatLen; j++) //��������
        referee->RealData[HeaderLen + CmdIdLen + j] = referee->RefereeData[k + j];

    if (Verify_CRC16_Check_Sum(referee->RealData, (HeaderLen + PowerHeatLen)) == 1) //CRC16У��
    {
        referee->RobotHurt.armor_id = referee->RealData[7];
        referee->RobotHurt.hurt_type = (referee->RealData[7] >> 4);
		
		referee->RobotHurt.error = 0;
    }
    else
    {
        referee->RobotHurt.error = 1;
    }
}

#endif

/*�ж�*/
void USART6_IRQHandler(void)
{
    unsigned short i = 0;
    unsigned short Len = 0;

    if (USART_GetITStatus(USART6, USART_IT_IDLE) != RESET) //�����жϱ�־λ
    {
        DMA_Cmd(DMA2_Stream1, DISABLE); //�ر�DMA,��ֹ�����ڼ�������

        while (DMA_GetCmdStatus(DMA2_Stream1) != DISABLE);

        Len = USART6->SR;
        Len = USART6->DR;
        Len = USART6_RX_LEN - DMA_GetCurrDataCounter(DMA2_Stream1); //DMA_GetCurrDataCounter���ڻ�ô�����ʣ��ĵ�Ԫ��
        REFEREE.DataLen = Len;

        for (i = 0; i < Len; i++)
        {
            REFEREE.RefereeData[i] = Usart6_Rx[i];
        }
		
		Referee_Read_Data(&REFEREE, Usart6_Rx);
		memset(Usart6_Rx, 0, USART6_RX_LEN);

//		RefereeDataDeal(&REFEREE);
        DMA_SetCurrDataCounter(DMA2_Stream1, USART6_RX_LEN); //����Ҫ��������ݵ�Ԫ��
        DMA_Cmd(DMA2_Stream1, ENABLE);                       //����DMA
        REFEREE.RECEIVE_FLAG = 1;
    }
}




/**
  * @brief          ��ȡ��������,�ж��ж�ȡ��֤�ٶ�
  * @param[in]      ���ղ���ϵͳ���ݽṹ��,��������
  * @retval         �Ƿ�������ж�������
  * @attention      �ڴ��ж�֡ͷ��CRCУ��,������д�����ݣ����ظ��ж�֡ͷ
  */
u8 Referee_Read_Data(REFEREE_t *referee, uint8_t *usart6_receive)
{
    bool_t retval_tf = 0;//������ȷ����־,ÿ�ε��ö�ȡ����ϵͳ���ݺ�������Ĭ��Ϊ����

    uint16_t judge_length;//ͳ��һ֡���ݳ���

    int CmdID = 0;//�������������

    //�����ݰ��������κδ���
    if (usart6_receive == NULL)
    {
        return 0;
    }

    //д��֡ͷ����,�����ж��Ƿ�ʼ�洢��������
    memcpy(&FrameHeader, referee->RefereeData, HeaderLen);

    //�ж�֡ͷ�����Ƿ�Ϊ0xA5
    if(usart6_receive[0] == JUDGE_FRAME_HEADER)
    {
        //֡ͷCRC8У��
        if (Verify_CRC8_Check_Sum( usart6_receive, HeaderLen ) == 1)
        {
            //ͳ��һ֡���ݳ���,����CR16У��
            judge_length = usart6_receive[1] + HeaderLen + CmdIdLen + CRC16Len;;

            //֡βCRC16У��
            if(Verify_CRC16_Check_Sum(usart6_receive, judge_length) == 1)
            {
                retval_tf = 1;//��У�������˵�����ݿ���

                CmdID = (usart6_receive[6] << 8 | usart6_receive[5]);

                //��������������,�����ݿ�������Ӧ�ṹ����(ע�⿽�����ݵĳ���)
                switch(CmdID)
                {
                    case 0x0001: //����״̬����
                        memcpy(&referee->GameStatus, (usart6_receive + DATA), LEN_game_state);
                        break;

                    case 0x0002: //�����������
                        memcpy(&referee->GmaeResult, (usart6_receive + DATA), LEN_game_result);
                        break;

                    case 0x0003:  //������Ѫ������    
                        memcpy(&referee->RobotHP, (usart6_receive + DATA), LEN_game_robot_survivors);
                        break;
					
					case 0x0004:  //���ڷ���״̬    
                        memcpy(&referee->DartStatus, (usart6_receive + DATA), LEN_game_robot_survivors);
                        break;

                    case 0x0101:  //�����¼�����
                        memcpy(&referee->EventData, (usart6_receive + DATA), LEN_event_data);
                        break;

                    case 0x0102:  //����վ������ʶ
                        memcpy(&referee->SupplyAction, (usart6_receive + DATA), LEN_supply_projectile_action);
                        break;

                    case 0x0104:  //���о�����Ϣ
                        memcpy(&referee->RefereeWarning, (usart6_receive + DATA), LEN_supply_projectile_booking);
                        break;
					
                    case 0x0105:  //���ڷ���ڵ���ʱ
                        memcpy(&referee->RemainingTime, (usart6_receive + DATA), LEN_supply_projectile_booking);
                        break;

                    case 0x0201:  //����������״̬
                        memcpy(&referee->RobotStatus, (usart6_receive + DATA), LEN_game_robot_state);
                        break;

                    case 0x0202: //ʵʱ������������
                        memcpy(&referee->PowerHeat, (usart6_receive + DATA), LEN_power_heat_data);
                        break;

                    case 0x0203:  //������λ��
                        memcpy(&referee->RobotPos, (usart6_receive + DATA), LEN_game_robot_pos);
                        break;

                    case 0x0204:  //����������
                        memcpy(&referee->Buff, (usart6_receive + DATA), LEN_buff_musk);
                        break;

                    case 0x0205:  //���л���������״̬
                        memcpy(&referee->AerialEnergy, (usart6_receive + DATA), LEN_aerial_robot_energy);
                        break;

                    case 0x0206:  //�˺�״̬
                        memcpy(&referee->RobotHurt, (usart6_receive + DATA), LEN_robot_hurt);

//                        if(RobotHurt.hurt_type == 0)//��װ�װ���������˺�
//                        {
//                            Hurt_Data_Update = 1;	   //װ������ÿ����һ�����ж�Ϊ�ܵ�һ���˺�
//                        }

                        break;

                    case 0x0207:  //ʵʱ�����Ϣ
                        memcpy(&referee->ShootData, (usart6_receive + DATA), LEN_shoot_data);
                        break;
					
					case 0x0208:  //�ӵ�ʣ�෢����
                        memcpy(&referee->BulletNum, (usart6_receive + DATA), LEN_aerial_robot_energy);
                        break;
					
					case 0x0209:  //������ RFID ״̬
                        memcpy(&referee->RFIDStatus, (usart6_receive + DATA), LEN_aerial_robot_energy);
                        break;
                }
            }
        }

        //�׵�ַ��֡����,ָ��CRC16��һ�ֽ�,�����ж��Ƿ�Ϊ0xA5,�����ж�һ�����ݰ��Ƿ��ж�֡����
        if(*(usart6_receive + sizeof(FrameHeader) + CmdIdLen + FrameHeader.DataLength + CRC16Len) == 0xA5)
        {
            //���һ�����ݰ������˶�֡����,���ٴζ�ȡ
            Referee_Read_Data(referee, (usart6_receive + sizeof(FrameHeader) + CmdIdLen + FrameHeader.DataLength + CRC16Len));
        }
    }


    return retval_tf;//����������������
}







///********************�������ݸ����жϺ���***************************/

/**
  * @brief          �����Ƿ����
  * @param[in]      void
  * @retval         TRUE����   FALSE������
  * @attention      �ڲ��ж�ȡ������ʵʱ�ı䷵��ֵ
  */
bool_t JUDGE_sGetDataState(void)
{
    return Judge_Data_TF;
}

/**
  * @brief          ��ȡ˲ʱ����
  * @param[in]      void
  * @retval         ʵʱ����ֵ
  * @attention      
  */
u32 Referee_Chassis_Power(void)
{
    return (REFEREE.PowerHeat.chassis_power);
}

/**
  * @brief          ��ȡʣ�ཹ������
  * @param[in]      void
  * @retval         ʣ�໺�役������(���60)
  * @attention      
  */
uint16_t Referee_Chassis_Power_Buffer(void)
{
    return (REFEREE.PowerHeat.chassis_power_buffer);
}


/**
  * @brief          ��ȡ��ǰ�ȼ�
  * @param[in]      void
  * @retval         ��ǰ�ȼ�
  * @attention      
  */
uint8_t Referee_Robot_Level(void)
{
    return	(REFEREE.RobotStatus.robot_level);
}


/**
  * @brief          ��ȡǹ������
  * @param[in]      void
  * @retval         17mm
  * @attention      ʵʱ����
  */
uint16_t Referee_Shooter_Heat0(void)
{
    return (REFEREE.PowerHeat.shooter_heat0);
}


/**
  * @brief          ͳ�Ʒ�����
  * @param[in]      void
  * @retval         void
  * @attention      
  */
portTickType shoot_time;//������ʱ����
portTickType shoot_ping;//����������շ����ӳ�
float Shoot_Speed_Now = 0;
float Shoot_Speed_Last = 0;
uint16_t ShootNum;//ͳ�Ʒ�����,0x0003����һ������Ϊ������һ��
void JUDGE_ShootNumCount(void)
{
    Shoot_Speed_Now = REFEREE.ShootData.bullet_speed;

    if(Shoot_Speed_Last != Shoot_Speed_Now)//��Ϊ��float�ͣ�������������ȫ���,�����ٶȲ���ʱ˵��������һ�ŵ�
    {
        ShootNum++;
        Shoot_Speed_Last = Shoot_Speed_Now;
    }

//    shoot_time = xTaskGetTickCount();//��ȡ���跢��ʱ��ϵͳʱ��
//    shoot_ping = shoot_time - REVOL_uiGetRevolTime();//�����ӳ�
}


/**
  * @brief          ��ȡ������
  * @param[in]      void
  * @retval         ������
  * @attention      ��������˫ǹ��
  */
uint16_t JUDGE_usGetShootNum(void)
{
    return ShootNum;
}


/**
  * @brief          ����������
  * @param[in]      void
  * @retval         void
  * @attention      
  */
void JUDGE_ShootNum_Clear(void)
{
    ShootNum = 0;
}


/**
  * @brief          ��ȡǹ������
  * @param[in]      void
  * @retval         ��ǰ�ȼ�17mm��������
  * @attention      
  */
uint16_t Referee_Shooter_Heat0_Cooling_Limit(void)
{
    return (REFEREE.RobotStatus.shooter_heat0_cooling_limit);
}


/**
  * @brief          ��ǰ�ȼ���Ӧ��ǹ��ÿ����ȴֵ
  * @param[in]      void
  * @retval         ��ǰ�ȼ�17mm��ȴ�ٶ�
  * @attention      
  */
uint16_t Referee_Shooter_Heat0_Cooling_Rate(void)
{
    return (REFEREE.RobotStatus.shooter_heat0_cooling_rate);
}


/**
  * @brief          ��ȡ�ӵ���Ƶ ��λ Hz
  * @param[in]      void
  * @retval         �ӵ���Ƶ ��λ Hz
  * @attention      
  */
uint8_t Referee_Bullet_Freq(void)
{
    return (REFEREE.ShootData.bullet_freq);
}


/**
  * @brief          ��ȡ�ӵ����� ��λ m/s
  * @param[in]      void
  * @retval         �ӵ����� ��λ m/s
  * @attention      
  */
float Referee_Bullet_Speed(void)
{
    return (REFEREE.ShootData.bullet_speed);
}
