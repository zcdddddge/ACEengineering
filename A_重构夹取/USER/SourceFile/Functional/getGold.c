#include "getGold.h"


/**
 * @description: 矿石自动换取金币 
 * @param {*}
 * @return {*}
 * @note : 0抬升--1夹紧--前伸-前翻-旋转 
 *         2.松夹-后伸-后翻-放下 
 *         
 */
static void goldExchange(Gr_t *Gr) 
{
    static u8 lock=1; 

    switch(Gr->state[2]) 
    {
        //抬升
        case  0 :  
        {
            UPLIFT(&Gr->GraspMotor[0], Gr->vL53L0, 10.0f, 1);
            if (Gr->GraspMotor[0].state == Finish)
            {
                Gr->GraspMotor[3].state = DisFinish;  //夹子准备
                Gr->state[2] = 1;
            }
            break; 
        }
        //夹紧
        case 1: 
        {
            CLIP(&Gr->GraspMotor[3], Cilp_Speed, 1);
            if(Gr->GraspMotor[3].state == Finish)
            {
                Gr->GraspMotor[4].state = DisFinish; //翻转标记为未完成
                Gr->state[2] = 2;
            }
            break;
        }
        //前伸
        case 2 :  
        {
            
        }
        //前翻 
        case 3: 
        {

        }
        // 旋转
        case 4: 
        {

        }
    }
    

}