#ifndef __RefereeDeal_H_
#define __RefereeDeal_H_
#include "main.h"


/***************������ID********************/

/*

	ID: 0x0001  Byte:  3    ����״̬����       			����Ƶ�� 1Hz
	ID: 0x0002  Byte:  1    �����������         		������������
	ID: 0x0003  Byte: 32    ���������˴������   		1Hz����
	ID: 0x0004  Byte:  3    ���ڷ���״̬                ���ڷ�����ͣ����ͷ�Χ�����л�����
	ID: 0x0101  Byte:  4    �����¼�����   				1Hz ���ڷ���
	ID: 0x0102  Byte:  4    ���ز���վ������ʶ����    	������������
	ID: 0x0104  Byte:  2    ���о�����Ϣ                ���淢������
	ID: 0X0105  Byte:  1    ���ڷ���ڵ���ʱ            1Hz ���ڷ���
	ID: 0X0201  Byte: 18    ������״̬����        		10Hz
	ID: 0X0202  Byte: 16    ʵʱ������������   			50Hz
	ID: 0x0203  Byte: 16    ������λ������           	10Hz
	ID: 0x0204  Byte:  1    ��������������           	1Hz ���ڷ���
	ID: 0x0205  Byte:  3    ���л���������״̬����      10Hz
	ID: 0x0206  Byte:  1    �˺�״̬����           		�˺���������
	ID: 0x0207  Byte:  6    ʵʱ�������           		�������
	ID: 0x0208  Byte:  2    �ӵ�ʣ�෢����              1Hz ���ڷ���
	ID: 0x0208  Byte:  4    ������ RFID ״̬            1Hz
	ID: 0x020A  Byte: 12    ���ڻ����˿ͻ���ָ������    10Hz
	ID: 0x0301  Byte:  n    �����˼佻������           	���ͷ���������,10Hz

*/
//������ID,�����жϽ��յ���ʲô����
typedef enum
{
    ID_game_state       			= 0x0001,//����״̬����
    ID_game_result 	   				= 0x0002,//�����������
    ID_game_robot_survivors       	= 0x0003,//���������˴������
    ID_event_data  					= 0x0101,//�����¼�����
    ID_supply_projectile_action   	= 0x0102,//���ز���վ������ʶ����
    ID_supply_projectile_booking 	= 0x0103,//���ز���վԤԼ�ӵ�����
    ID_game_robot_state    			= 0x0201,//������״̬����
    ID_power_heat_data    			= 0x0202,//ʵʱ������������
    ID_game_robot_pos        		= 0x0203,//������λ������
    ID_buff_musk					= 0x0204,//��������������
    ID_aerial_robot_energy			= 0x0205,//���л���������״̬����
    ID_robot_hurt					= 0x0206,//�˺�״̬����
    ID_shoot_data					= 0x0207,//ʵʱ�������

} CmdID;

//���������ݶγ�,���ݹٷ�Э�������峤��
typedef enum
{
    LEN_game_state       				=  3,	//0x0001
    LEN_game_result       				=  1,	//0x0002
    LEN_game_robot_survivors       		=  2,	//0x0003
    LEN_event_data  					=  4,	//0x0101
    LEN_supply_projectile_action        =  3,	//0x0102
    LEN_supply_projectile_booking		=  2,	//0x0103
    LEN_game_robot_state    			= 15,	//0x0201
    LEN_power_heat_data   				= 14,	//0x0202
    LEN_game_robot_pos        			= 16,	//0x0203
    LEN_buff_musk        				=  1,	//0x0204
    LEN_aerial_robot_energy        		=  3,	//0x0205
    LEN_robot_hurt        				=  1,	//0x0206
    LEN_shoot_data       				=  6,	//0x0207

} JudgeDataLength;

/* �Զ���֡ͷ */
typedef __packed struct
{
    uint8_t  SOF;
    uint16_t DataLength;
    uint8_t  Seq;
    uint8_t  CRC8;
} xFrameHeader_t;

//��ʼ�ֽ�,Э��̶�Ϊ0xA5
#define    JUDGE_FRAME_HEADER         (0xA5)
typedef enum
{
    FRAME_HEADER = 0,
    CMD_ID = 5,
    DATA = 7,

} JudgeFrameOffset;




/* ID: 0x0001  Byte:  3    ����״̬���� */
typedef __packed struct
{
    uint8_t game_type : 4;
    uint8_t game_progress : 4;
    uint16_t stage_remain_time;
    uint8_t error;
} ext_game_status_t;

/* ID: 0x0002  Byte:  1    ����������� */
typedef __packed struct
{
    uint8_t winner;
} ext_game_result_t;

/* ID: 0x0003  Byte:  32    ������Ѫ������ */
typedef __packed struct
{
    uint16_t red_1_robot_HP;
    uint16_t red_2_robot_HP;
    uint16_t red_3_robot_HP;
    uint16_t red_4_robot_HP;
    uint16_t red_5_robot_HP;
    uint16_t red_7_robot_HP;
    uint16_t red_outpost_HP;
    uint16_t red_base_HP;
    uint16_t blue_1_robot_HP;
    uint16_t blue_2_robot_HP;
    uint16_t blue_3_robot_HP;
    uint16_t blue_4_robot_HP;
    uint16_t blue_5_robot_HP;
    uint16_t blue_7_robot_HP;
    uint16_t blue_outpost_HP;
    uint16_t blue_base_HP;
    uint8_t error;
} ext_game_robot_HP_t;

/* ID: 0x0004  Byte:  3    ���ڷ���״̬ */
typedef __packed struct
{
    uint8_t dart_belong;
    uint16_t stage_remaining_time;
    uint8_t error;
} ext_dart_status_t;


/* ID: 0x0101  Byte:  4    �����¼����� */
typedef __packed struct
{
    uint32_t Parking : 2;
    uint32_t EnergyAgency : 2;
    uint32_t Shield : 1;
    uint32_t other : 27;
    uint8_t error;
} ext_event_data_t;

/* ID: 0x0102  Byte:  4    ����վ������ʶ */
typedef __packed struct
{
    uint8_t supply_projectile_id;
    uint8_t supply_robot_id;
    uint8_t supply_projectile_step;
    uint8_t supply_projectile_num;
    uint8_t error;
} ext_supply_projectile_action_t;

/* ID: 0x0104  Byte:  2    ���о�����Ϣ */
typedef __packed struct
{
    uint8_t level;
    uint8_t foul_robot_id;
} ext_referee_warning_t;

/* ID: 0x0105  Byte:  1    ���ڷ���ڵ���ʱ */
typedef __packed struct
{
    uint8_t dart_remaining_time;
    uint8_t error;
} ext_dart_remaining_time_t;

/* ID: 0X0201  Byte: 15    ����������״̬ */
typedef __packed struct
{
    uint8_t robot_id;
    uint8_t robot_level;
    uint16_t remain_HP;
    uint16_t max_HP;
    uint16_t shooter_heat0_cooling_rate;
    uint16_t shooter_heat0_cooling_limit;
    uint16_t shooter_heat1_cooling_rate;
    uint16_t shooter_heat1_cooling_limit;
    uint8_t shooter_heat0_speed_limit;
    uint8_t shooter_heat1_speed_limit;
    uint8_t max_chassis_power;
    uint8_t mains_power_gimbal_output : 1;
    uint8_t mains_power_chassis_output : 1;
    uint8_t mains_power_shooter_output : 1;
    uint8_t error;
} ext_game_robot_status_t;

/* ID: 0X0202  Byte: 14    ʵʱ������������ */
typedef __packed struct
{
    uint16_t chassis_volt;
    uint16_t chassis_current;
    float chassis_power;
    uint16_t chassis_power_buffer;
    uint16_t shooter_heat0;
    uint16_t shooter_heat1;
    uint16_t mobile_shooter_heat2;
    uint8_t error;
} ext_power_heat_data_t;

/* ID: 0x0203  Byte: 16    ������λ������ */
typedef __packed struct
{
    float x;
    float y;
    float z;
    float yaw;
} ext_game_robot_pos_t;

/* ID: 0x0204  Byte:  1    �������������� */
typedef __packed struct
{
    uint8_t power_rune_buff;
} ext_buff_musk_t;

/* ID: 0x0205  Byte:  3    ���л���������״̬���� */
typedef __packed struct
{
    uint16_t energy_point;
    uint8_t attack_time;
    uint8_t error;
} ext_aerial_robot_energy_t;

/* ID: 0x0206  Byte:  1    �˺�״̬���� */
typedef __packed struct
{
    uint8_t armor_id : 4;
    uint8_t hurt_type : 4;
    uint8_t error;
} ext_robot_hurt_t;

/* ID: 0x0207  Byte:  6    ʵʱ������� */
typedef __packed struct
{
    uint8_t bullet_type;
    uint8_t bullet_freq;
    float bullet_speed;
    uint8_t error;
} ext_shoot_data_t;

/* ID: 0x0208  Byte:  2    ʵʱ������� */
typedef __packed struct
{
    uint16_t bullet_remaining_num;
    uint8_t error;
} ext_bullet_remaining_t;

/* ID: 0x0209  Byte:  4    ������ RFID ״̬ */
typedef __packed struct
{
    uint32_t BaseLand : 1;
    uint32_t HighLand : 1;
    uint32_t Energy : 1;
    uint32_t OverSlope : 1;
    uint32_t Sentry : 1;
    uint32_t Resources : 1;
    uint32_t BloodPoint : 1;
    uint32_t BloodCard : 1;
    uint32_t other : 24;
    uint8_t error;
} ext_rfid_status_t;

typedef __packed struct
{
	uint8_t dart_launch_opening_status;
	uint8_t dart_attack_target;
	uint16_t target_change_time;
	uint8_t first_dart_speed;
	uint8_t second_dart_speed;
	uint8_t third_dart_speed;
	uint8_t fourth_dart_speed;
	uint16_t last_dart_launch_time;
	uint16_t operate_launch_cmd_time;
} ext_dart_client_cmd_t;

/* ����ϵͳ���ݣ��ܽṹ�� */
typedef __packed struct
{
    u8 RefereeData[256];
    u8 RealData[45];
	
    int16_t DataLen;
    int16_t RealLen;
    int16_t Cmd_ID;
    u8 RECEIVE_FLAG;
	
    ext_game_status_t GameStatus; //0x0001
	ext_game_result_t GmaeResult; //0x0002
    ext_game_robot_HP_t RobotHP;  //0x0003
    ext_dart_status_t DartStatus; //0x0004
    ext_event_data_t EventData;   //0x0101
    ext_supply_projectile_action_t SupplyAction;  //0x0102
	ext_referee_warning_t RefereeWarning;  //0x0104
    ext_dart_remaining_time_t RemainingTime;  //0x0105
    ext_game_robot_status_t RobotStatus;  //0x0201
    ext_power_heat_data_t PowerHeat;  //0x0202
	ext_game_robot_pos_t RobotPos;  //0x0203
    ext_buff_musk_t Buff;  //0x0204
    ext_aerial_robot_energy_t AerialEnergy;  //0x0205
    ext_robot_hurt_t RobotHurt;  //0x0206
    ext_shoot_data_t ShootData;  //0x0207
    ext_bullet_remaining_t BulletNum;   //0x0208
    ext_rfid_status_t RFIDStatus;  //0x0209
} REFEREE_t;


/*float��������*/
typedef union
{
    struct
    {
        u8 LB;
        u8 MLB;
        u8 MHB;
        u8 HB;
    } float_byte;
    float value;
} Float_t;


#if JUDGE_VERSION == JUDGE_ACE
extern void RefereeDataDeal(REFEREE_t*referee);
#endif

extern void Referee_System_Init(void); //��������ʼ��
REFEREE_t *Return_Referee_Point(void); //���غ���ָ��

extern u8 Referee_Read_Data(REFEREE_t *referee, uint8_t *usart6_receive);

//���ز���ϵͳ����
/**
  * @brief          �����Ƿ����
  * @param[in]      void
  * @retval         TRUE����   FALSE������
  * @attention      �ڲ��ж�ȡ������ʵʱ�ı䷵��ֵ
  */
bool_t JUDGE_sGetDataState(void);
/**
  * @brief          ��ȡ˲ʱ����
  * @param[in]      void
  * @retval         ʵʱ����ֵ
  * @attention      
  */
u32 Referee_Chassis_Power(void);
/**
  * @brief          ��ȡʣ�ཹ������
  * @param[in]      void
  * @retval         ʣ�໺�役������(���60)
  * @attention      
  */
uint16_t Referee_Chassis_Power_Buffer(void);
/**
  * @brief          ��ȡ��ǰ�ȼ�
  * @param[in]      void
  * @retval         ��ǰ�ȼ�
  * @attention      
  */
uint8_t Referee_Robot_Level(void);
/**
  * @brief          ��ȡǹ������
  * @param[in]      void
  * @retval         17mm
  * @attention      ʵʱ����
  */
uint16_t Referee_Shooter_Heat0(void);
/**
  * @brief          ͳ�Ʒ�����
  * @param[in]      void
  * @retval         void
  * @attention      
  */
void JUDGE_ShootNumCount(void);
/**
  * @brief          ��ȡ������
  * @param[in]      void
  * @retval         ������
  * @attention      ��������˫ǹ��
  */
uint16_t JUDGE_usGetShootNum(void);
/**
  * @brief          ����������
  * @param[in]      void
  * @retval         void
  * @attention      
  */
void JUDGE_ShootNum_Clear(void);
/**
  * @brief          ��ȡǹ������
  * @param[in]      void
  * @retval         ��ǰ�ȼ�17mm��������
  * @attention      
  */
uint16_t Referee_Shooter_Heat0_Cooling_Limit(void);
/**
  * @brief          ��ǰ�ȼ���Ӧ��ǹ��ÿ����ȴֵ
  * @param[in]      void
  * @retval         ��ǰ�ȼ�17mm��ȴ�ٶ�
  * @attention      
  */
uint16_t Referee_Shooter_Heat0_Cooling_Rate(void);
/**
  * @brief          ��ȡ�ӵ���Ƶ ��λ Hz
  * @param[in]      void
  * @retval         �ӵ���Ƶ ��λ Hz
  * @attention      
  */
uint8_t Referee_Bullet_Freq(void);
/**
  * @brief          ��ȡ�ӵ����� ��λ m/s
  * @param[in]      void
  * @retval         �ӵ����� ��λ m/s
  * @attention      
  */
float Referee_Bullet_Speed(void);

#endif
