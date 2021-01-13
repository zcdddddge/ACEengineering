/*
 * @Author: your name
 * @Date: 2021-01-09 15:32:31
 * @LastEditTime: 2021-01-10 10:43:42
 * @LastEditors: your name
 * @Description: In User Settings Edit
 * @FilePath: \PROJECTd:\RMware\A\USER\HeaderFile\Functional\RobotAction.h
 */
#ifndef __ROBOTACTION_H_
#define __ROBOTACTION_H_

#include "GraspMotor.h"

/*静态函数*/
static void UPLIFT(Motor_t *uplift,VL53L0_t *vl53l0,float dis,u8 dire);
static void RAIL(Motor_t *rall,Sensor_t*val,u8 dire);
static void Telescoping(Motor_t *telescoping,u8 dire);
static void FLIP(Motor_t *filp1,Motor_t *filp2,float exp,float limit,u8 dire);
static void CLIP(Motor_t *clip,int16_t exp,u8 dire);
static void Auto_One_Box(Gr_t *Gr);
static void Auto_Two_Box(Gr_t *Gr);

/*自动控制*/
void Auto_Ctrl(Gr_t *Gr,u8 box);
void ResetAction(Gr_t *Gr);
static void filpReset(Motor_t *  filp1 , Motor_t *filp2 ,float limit );

#endif
