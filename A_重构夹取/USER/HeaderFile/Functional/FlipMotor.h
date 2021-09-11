#ifndef __FLIPMOTOR_H_
#define __FLIPMOTOR_H_


#include "GraspMotor.h"
#include "motor.h"




void Flip_Motor_Init(Gr_t *Gr);
void flip_motor(Motor_t *filp, float exp, float limit, u8 dire);










#endif



