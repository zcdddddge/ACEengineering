/*
 * 
 * 　　┏┓　　　┏┓+ +
 * 　┏┛┻━━━┛┻┓ + +
 * 　┃　　　　　　　┃ 　
 * 　┃　　　━　　　┃ ++ + + +
 *  ████━████ ┃+
 * 　┃　　　　　　　┃ +
 * 　┃　　　┻　　　┃
 * 　┃　　　　　　　┃ + +
 * 　┗━┓　　　┏━┛
 * 　　　┃　　　┃　　　　　　　　　　　
 * 　　　┃　　　┃ + + + +
 * 　　　┃　　　┃
 * 　　　┃　　　┃ +  神兽保佑
 * 　　　┃　　　┃    代码无bug　　
 * 　　　┃　　　┃　　+　　　　　　　　　
 * 　　　┃　 　　┗━━━┓ + +
 * 　　　┃ 　　　　　　　┣┓
 * 　　　┃ 　　　　　　　┏┛
 * 　　　┗┓┓┏━┳┓┏┛ + + + +
 * 　　　　┃┫┫　┃┫┫
 * 　　　　┗┻┛　┗┻┛+ + + +
 * 
 * 
 * @Author: your name
 * @Date: 2020-12-01 09:06:59
 * @LastEditTime: 2021-01-06 11:50:05
 * @LastEditors: Please set LastEditors
 * @Description: None 
 */


#include "motor.h"
/**
 * @description: 电机结构体清零
 * @param {Motor_t} *motor
 * @return {*}
 */
void MotorValZero(Motor_t *motor)
{
		motor->ID = motor->Pos_Lock = motor->Radio = 0;
		motor->ExpSpeed 	= 0;
		motor->ExpRadian 	= 0;
		motor->state			= 0;
		PID_INIT(&motor->SPID,0,0,0,0,0);
		PID_INIT(&motor->PPID,0,0,0,0,0);
		EncoderValZero(motor->Encoder);
		motor->MotorType = CURRENCY_M;
		motor->ResetFlag =0;
}
