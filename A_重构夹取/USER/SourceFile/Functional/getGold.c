#include "getGold.h"


static u8 gold_lock=1 ;
void Auto_Gold(Gr_t *Gr) 
{
    
    if(gold_lock==1)
    {
        gold_lock = 2;
        Gr->GraspMotor[0].state= DisFinish ; 
    }
    else if (gold_lock == 2)
    {
        goldExchange(Gr) ;
        pid_Cala(Gr);
    }
		
    
}
 
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

    switch(Gr->state[2]) 
    {
        //抬升
        case  0 :  
        {
            uplift(&Gr->GraspMotor[0], Gr->vL53L0, 10.0f, 1);
            if (Gr->GraspMotor[0].state == Finish)
            {
                CLIP.state = DisFinish;  //夹子准备
                Gr->state[2] = 1;
            }
            break; 
        }
        //夹紧
        case 1: 
        {
            clip(&Gr->GraspMotor[3], Cilp_Speed, 1);
            if(Gr->GraspMotor[3].state == Finish)
            {
                Gr->GraspMotor[1].state = DisFinish;  
                Gr->state[2] = 2;
            }
            break;
        }
        //前伸
        case 2 :  
        {
             
            Telescoping(&Gr->GraspMotor[2],2) ; 
            if (TELESCOPING.state==Finish) {
                Gr->GraspMotor[4].state = DisFinish;    //翻转准备 
                Gr->state[2] = 3;
            }
        }
        //前翻 
        case 3: 
        {
            flip(&Gr->GraspMotor[4], &Gr->GraspMotor[5], 182.0f, 6.0f, 1); // 180-->182
            if (Gr->GraspMotor[4].state == Finish && Gr->GraspMotor[5].state == Finish)
            {
                Gr->GraspMotor[7].ExpSpeed = 0;
                Gr->GraspMotor[7].state = DisFinish;  //旋转准备 
                Gr->state[2] = 4;
            }
            break;
        }
        //旋转
        case 4: 
        {
            rotate(&Gr->GraspMotor[7]);
            if(Gr->GraspMotor[7].state == Finish) 
            {
								CLIP.ExpSpeed = 0; 
								CLIP.state    = DisFinish ; //夹子准备	
                Gr->state[2]  =  5; 
								
            }
        }
        //松夹 
        case 5: 
        {
            clip(&Gr->GraspMotor[3], Cilp_Speed, 2);
            if(CLIP.state == Finish) 
            {
                TELESCOPING.state=DisFinish;
								Gr->state[2]=6;
								Gr->state[3]=1;

            }
        }
        case 6: 
        {
            Telescoping(&Gr->GraspMotor[2], 1);
            if(TELESCOPING.state == Finish) 
            {
                FLIP_1.state = DisFinish;
                FLIP_2.state = DisFinish;
								Gr->state[2] =7 ;
								Gr->state[1] =1 ;
            }
        }
        case 7: 
        {
            flip(&Gr->GraspMotor[4], &Gr->GraspMotor[5], -160.0f, 20.0f, 2);
            if (FLIP_1.state == Finish && FLIP_2.state == Finish)
            {
								UPLIFT.state= DisFinish;
                Gr->state[2]=8 ;
            }
        }
        // 放下
        case 8 :
        {

            uplift(&Gr->GraspMotor[0], Gr->vL53L0, 1.0f, 2); 
            if(UPLIFT.state == Finish) 
            {
//                FLIP_1.state = DisFinish; 
//                FLIP_2.state = DisFinish;
//                CLIP.state         = DisFinish ;
//                TELESCOPING.state  = DisFinish ;
//                Gr->state[2] =  0 ;
//                gold_lock = 1; // 解锁
            }
        }
        
        default :break; 
        
        
    }

    

}