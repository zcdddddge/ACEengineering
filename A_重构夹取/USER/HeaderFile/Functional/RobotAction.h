#ifndef __ROBOTACTION_H_
#define __ROBOTACTION_H_

#include  "GraspMotor.h"
#include  "MotorAction.h" 



static void PID_CALA(Gr_t *Gr);
static void Auto_One_Box(Gr_t *Gr);


static void Auto_Two_Box(Gr_t *Gr);
static void RAIL(Motor_t *rall,Sensor_t*val,u8 dire);


/*×Ô¶¯¿ØÖÆ*/
void Auto_Ctrl(Gr_t *Gr,u8 box);
void ResetAction(Gr_t *Gr);
static void filpReset(Motor_t *  filp1 , Motor_t *filp2 ,float limit );

#endif
