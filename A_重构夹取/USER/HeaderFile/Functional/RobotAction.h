#ifndef __ROBOTACTION_H_
#define __ROBOTACTION_H_

#include  "GraspMotor.h"
#include  "MotorAction.h" 
#include  "GPIO_DEAL.h"


void Auto_Ctrl(Gr_t *Gr,u8 box);		/*×Ô¶¯¿ØÖÆ*/
void Grasp_init(Gr_t *Gr);
void Grasp_reset(Gr_t *Gr);
void Grasp_First(Gr_t *Gr);
void Grasp_Second(Gr_t *Gr);
void Grasp_Third(Gr_t *Gr);
void Reset(Gr_t *Gr);

#endif
