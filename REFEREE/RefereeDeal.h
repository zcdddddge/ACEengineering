#ifndef __RefereeDeal_H_
#define __RefereeDeal_H_
#include "main.h"


/***************命令码ID********************/

/*

	ID: 0x0001  Byte:  3    比赛状态数据       			发送频率 1Hz
	ID: 0x0002  Byte:  1    比赛结果数据         		比赛结束后发送
	ID: 0x0003  Byte: 32    比赛机器人存活数据   		1Hz发送
	ID: 0x0004  Byte:  3    飞镖发射状态                飞镖发射后发送，发送范围：所有机器人
	ID: 0x0101  Byte:  4    场地事件数据   				1Hz 周期发送
	ID: 0x0102  Byte:  4    场地补给站动作标识数据    	动作触发后发送
	ID: 0x0104  Byte:  2    裁判警告信息                警告发生后发送
	ID: 0X0105  Byte:  1    飞镖发射口倒计时            1Hz 周期发送
	ID: 0X0201  Byte: 18    机器人状态数据        		10Hz
	ID: 0X0202  Byte: 16    实时功率热量数据   			50Hz
	ID: 0x0203  Byte: 16    机器人位置数据           	10Hz
	ID: 0x0204  Byte:  1    机器人增益数据           	1Hz 周期发送
	ID: 0x0205  Byte:  3    空中机器人能量状态数据      10Hz
	ID: 0x0206  Byte:  1    伤害状态数据           		伤害发生后发送
	ID: 0x0207  Byte:  6    实时射击数据           		射击后发送
	ID: 0x0208  Byte:  2    子弹剩余发射数              1Hz 周期发送
	ID: 0x0208  Byte:  4    机器人 RFID 状态            1Hz
	ID: 0x020A  Byte: 12    飞镖机器人客户端指令数据    10Hz
	ID: 0x0301  Byte:  n    机器人间交互数据           	发送方触发发送,10Hz

*/
//命令码ID,用来判断接收的是什么数据
typedef enum
{
    ID_game_state       			= 0x0001,//比赛状态数据
    ID_game_result 	   				= 0x0002,//比赛结果数据
    ID_game_robot_survivors       	= 0x0003,//比赛机器人存活数据
    ID_event_data  					= 0x0101,//场地事件数据
    ID_supply_projectile_action   	= 0x0102,//场地补给站动作标识数据
    ID_supply_projectile_booking 	= 0x0103,//场地补给站预约子弹数据
    ID_game_robot_state    			= 0x0201,//机器人状态数据
    ID_power_heat_data    			= 0x0202,//实时功率热量数据
    ID_game_robot_pos        		= 0x0203,//机器人位置数据
    ID_buff_musk					= 0x0204,//机器人增益数据
    ID_aerial_robot_energy			= 0x0205,//空中机器人能量状态数据
    ID_robot_hurt					= 0x0206,//伤害状态数据
    ID_shoot_data					= 0x0207,//实时射击数据

} CmdID;

//命令码数据段长,根据官方协议来定义长度
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

/* 自定义帧头 */
typedef __packed struct
{
    uint8_t  SOF;
    uint16_t DataLength;
    uint8_t  Seq;
    uint8_t  CRC8;
} xFrameHeader_t;

//起始字节,协议固定为0xA5
#define    JUDGE_FRAME_HEADER         (0xA5)
typedef enum
{
    FRAME_HEADER = 0,
    CMD_ID = 5,
    DATA = 7,

} JudgeFrameOffset;




/* ID: 0x0001  Byte:  3    比赛状态数据 */
typedef __packed struct
{
    uint8_t game_type : 4;
    uint8_t game_progress : 4;
    uint16_t stage_remain_time;
    uint8_t error;
} ext_game_status_t;

/* ID: 0x0002  Byte:  1    比赛结果数据 */
typedef __packed struct
{
    uint8_t winner;
} ext_game_result_t;

/* ID: 0x0003  Byte:  32    机器人血量数据 */
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

/* ID: 0x0004  Byte:  3    飞镖发射状态 */
typedef __packed struct
{
    uint8_t dart_belong;
    uint16_t stage_remaining_time;
    uint8_t error;
} ext_dart_status_t;


/* ID: 0x0101  Byte:  4    场地事件数据 */
typedef __packed struct
{
    uint32_t Parking : 2;
    uint32_t EnergyAgency : 2;
    uint32_t Shield : 1;
    uint32_t other : 27;
    uint8_t error;
} ext_event_data_t;

/* ID: 0x0102  Byte:  4    补给站动作标识 */
typedef __packed struct
{
    uint8_t supply_projectile_id;
    uint8_t supply_robot_id;
    uint8_t supply_projectile_step;
    uint8_t supply_projectile_num;
    uint8_t error;
} ext_supply_projectile_action_t;

/* ID: 0x0104  Byte:  2    裁判警告信息 */
typedef __packed struct
{
    uint8_t level;
    uint8_t foul_robot_id;
} ext_referee_warning_t;

/* ID: 0x0105  Byte:  1    飞镖发射口倒计时 */
typedef __packed struct
{
    uint8_t dart_remaining_time;
    uint8_t error;
} ext_dart_remaining_time_t;

/* ID: 0X0201  Byte: 15    比赛机器人状态 */
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

/* ID: 0X0202  Byte: 14    实时功率热量数据 */
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

/* ID: 0x0203  Byte: 16    机器人位置数据 */
typedef __packed struct
{
    float x;
    float y;
    float z;
    float yaw;
} ext_game_robot_pos_t;

/* ID: 0x0204  Byte:  1    机器人增益数据 */
typedef __packed struct
{
    uint8_t power_rune_buff;
} ext_buff_musk_t;

/* ID: 0x0205  Byte:  3    空中机器人能量状态数据 */
typedef __packed struct
{
    uint16_t energy_point;
    uint8_t attack_time;
    uint8_t error;
} ext_aerial_robot_energy_t;

/* ID: 0x0206  Byte:  1    伤害状态数据 */
typedef __packed struct
{
    uint8_t armor_id : 4;
    uint8_t hurt_type : 4;
    uint8_t error;
} ext_robot_hurt_t;

/* ID: 0x0207  Byte:  6    实时射击数据 */
typedef __packed struct
{
    uint8_t bullet_type;
    uint8_t bullet_freq;
    float bullet_speed;
    uint8_t error;
} ext_shoot_data_t;

/* ID: 0x0208  Byte:  2    实时射击数据 */
typedef __packed struct
{
    uint16_t bullet_remaining_num;
    uint8_t error;
} ext_bullet_remaining_t;

/* ID: 0x0209  Byte:  4    机器人 RFID 状态 */
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

/* 裁判系统数据，总结构体 */
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


/*float数组重组*/
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

extern void Referee_System_Init(void); //串口六初始化
REFEREE_t *Return_Referee_Point(void); //返回函数指针

extern u8 Referee_Read_Data(REFEREE_t *referee, uint8_t *usart6_receive);

//返回裁判系统数据
/**
  * @brief          数据是否可用
  * @param[in]      void
  * @retval         TRUE可用   FALSE不可用
  * @attention      在裁判读取函数中实时改变返回值
  */
bool_t JUDGE_sGetDataState(void);
/**
  * @brief          读取瞬时功率
  * @param[in]      void
  * @retval         实时功率值
  * @attention      
  */
u32 Referee_Chassis_Power(void);
/**
  * @brief          读取剩余焦耳能量
  * @param[in]      void
  * @retval         剩余缓冲焦耳能量(最大60)
  * @attention      
  */
uint16_t Referee_Chassis_Power_Buffer(void);
/**
  * @brief          读取当前等级
  * @param[in]      void
  * @retval         当前等级
  * @attention      
  */
uint8_t Referee_Robot_Level(void);
/**
  * @brief          读取枪口热量
  * @param[in]      void
  * @retval         17mm
  * @attention      实时热量
  */
uint16_t Referee_Shooter_Heat0(void);
/**
  * @brief          统计发弹量
  * @param[in]      void
  * @retval         void
  * @attention      
  */
void JUDGE_ShootNumCount(void);
/**
  * @brief          读取发弹量
  * @param[in]      void
  * @retval         发弹量
  * @attention      不适用于双枪管
  */
uint16_t JUDGE_usGetShootNum(void);
/**
  * @brief          发弹量清零
  * @param[in]      void
  * @retval         void
  * @attention      
  */
void JUDGE_ShootNum_Clear(void);
/**
  * @brief          读取枪口热量
  * @param[in]      void
  * @retval         当前等级17mm热量上限
  * @attention      
  */
uint16_t Referee_Shooter_Heat0_Cooling_Limit(void);
/**
  * @brief          当前等级对应的枪口每秒冷却值
  * @param[in]      void
  * @retval         当前等级17mm冷却速度
  * @attention      
  */
uint16_t Referee_Shooter_Heat0_Cooling_Rate(void);
/**
  * @brief          读取子弹射频 单位 Hz
  * @param[in]      void
  * @retval         子弹射频 单位 Hz
  * @attention      
  */
uint8_t Referee_Bullet_Freq(void);
/**
  * @brief          读取子弹射速 单位 m/s
  * @param[in]      void
  * @retval         子弹射速 单位 m/s
  * @attention      
  */
float Referee_Bullet_Speed(void);

#endif
