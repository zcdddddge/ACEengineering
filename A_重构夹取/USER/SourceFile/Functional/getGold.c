#include "getGold.h"


static u8 gold_lock=1 ;
void Change_Gold(Gr_t *Gr) 
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
                CLIP.state= DisFinish ;
                CLIP.ExpSpeed =0 ;
                Gr->GraspMotor[7].ExpSpeed = 0;
                Gr->GraspMotor[7].state = DisFinish;  //旋转准备 
                Gr->state[2] = 5;
            }
            break;
        }

        //松夹 
        case 5: 
        {
            clip(&Gr->GraspMotor[3], Cilp_Speed, 2);
            if(CLIP.state == Finish) 
            {
								//Gr->GraspMotor[4].state=DisFinish ;
								//Gr->GraspMotor[5].state=DisFinish ;
								Gr->state[2]++;
								Gr->state[3]=1;

            }
        }
		#if  0
        case 6: 
        {
						flip(&Gr->GraspMotor[4], &Gr->GraspMotor[5], -160.0f, 20.0f, 2);
            if (FLIP_1.state == Finish && FLIP_2.state == Finish)
            {
								//UPLIFT.state= DisFinish;
                Gr->state[2]++ ;
            }
            
        }
			
        case				7: 
        {
						Telescoping(&Gr->GraspMotor[2], 1);
            if(TELESCOPING.state == Finish) 
            {
                FLIP_1.state = DisFinish;
                FLIP_2.state = DisFinish;
								Gr->state[2] ++ ;
            }
        }
        // 放下
        case 8 :
        {

            //uplift(&Gr->GraspMotor[0], Gr->vL53L0, 1.0f, 2); 
            //if(UPLIFT.state == Finish) 
            //{
//                FLIP_1.state = DisFinish; 
//                FLIP_2.state = DisFinish;
//                CLIP.state         = DisFinish ;
//                TELESCOPING.state  = DisFinish ;
//                Gr->state[2] =  0 ;
//                gold_lock = 1; // 解锁 
            //}
        }
        #endif 
        default :break; 
        
        
    }
}









/**
 * @description: 夹取地上的矿石,逻辑上使用了两把锁 
 * @param {Gr_t} *Gr
 * @return {*}
 */
void Pick_Gold(Gr_t *Gr)
{
		switch( Gr->state[3] ){
				// 翻转
				case 0 : {
						// 前翻
						flip2(&FLIP_1,237.0, 6,1) ; 
						if(FLIP_1.state==Finish){
								CLIP.state=DisFinish;
								Gr->state[3] =1 ;
						}
						break; 
				}
				case 1 :
				{
						clip(&Gr->GraspMotor[3], Cilp_Speed, 1);
						if (Gr->GraspMotor[3].state == Finish)
						{
								Gr->GraspMotor[4].state = DisFinish;
								Gr->state[2] = 2;
						}
						break;
				}
				case 2:
				{
						flip2(&Gr->GraspMotor[4], -160.0f, 20.0f, 2);
						if (FLIP_1.state == Finish)
						{
								CLIP.state= DisFinish;
								Gr->state[3]++;
						}
						break ; 
				}
				case 3 :{
						clip(&Gr->GraspMotor[3], Cilp_Speed, 2);
						if (Gr->GraspMotor[3].state == Finish)
						{
								Gr->GraspMotor[4].state = DisFinish;
								Gr->state[3] ++ ;
						}
						break;
				}
				case 4:{
						// 解锁 
						CLIP.state=DisFinish;
						FLIP_1.state=DisFinish;
					 
						Gr->state[3]++;  // 5标记已完成 
				}
				default :
				{
						break;
				}
		}
    
}

#if 0 
/**
 * @description: 3.12存档: 准备删掉一个一个动作调
 * @param {Gr_t} *Gr
 * @return {*}
 */
static void goldExchange(Gr_t *Gr)
{

    switch (Gr->state[2])
    {
    //抬升
    case 0:
    {
        uplift(&Gr->GraspMotor[0], Gr->vL53L0, 10.0f, 1);
        if (Gr->GraspMotor[0].state == Finish)
        {
            CLIP.state = DisFinish; //夹子准备
            Gr->state[2] = 1;
        }
        break;
    }
    //夹紧
    case 1:
    {
        clip(&Gr->GraspMotor[3], Cilp_Speed, 1);
        if (Gr->GraspMotor[3].state == Finish)
        {
            Gr->GraspMotor[1].state = DisFinish;
            Gr->state[2] = 2;
        }
        break;
    }
    //前伸
    case 2:
    {

        Telescoping(&Gr->GraspMotor[2], 2);
        if (TELESCOPING.state == Finish)
        {
            Gr->GraspMotor[4].state = DisFinish; //翻转准备
            Gr->state[2] = 3;
        }
    }
    //前翻
    case 3:
    {
        flip(&Gr->GraspMotor[4], &Gr->GraspMotor[5], 182.0f, 6.0f, 1); // 180-->182
        if (Gr->GraspMotor[4].state == Finish && Gr->GraspMotor[5].state == Finish)
        {
            CLIP.state = DisFinish;
            CLIP.ExpSpeed = 0;
            Gr->GraspMotor[7].ExpSpeed = 0;
            Gr-  >GraspMotor[7].state = DisFinish; //旋转准备
            Gr->state[2] = 5;
        }
        break;
    }
#if 0 
        //旋转
        case 4: 
        {
            rotate(&Gr->GraspMotor[7],182.0f,6.0f);
            if(Gr->GraspMotor[7].state == Finish) 
            {
                CLIP.ExpSpeed = 0; 
                CLIP.state    = DisFinish ; //夹子准备	
                Gr->state[2]  ++; 
                Gr->state[1]  = 1 ;
								
            }
        }
#endif
    //松夹
    case 5:
    {
        clip(&Gr->GraspMotor[3], Cilp_Speed, 2);
        if (CLIP.state == Finish)
        {
            TELESCOPING.state = DisFinish;
            Gr->state[2]++;
            Gr->state[3] = 1;
        }
    }
    case 6:
    {
        Telescoping(&Gr->GraspMotor[2], 1);
        if (TELESCOPING.state == Finish)
        {
            FLIP_1.state = DisFinish;
            FLIP_2.state = DisFinish;
            Gr->state[2]++;
        }
    }
    case 7:
    {
        flip(&Gr->GraspMotor[4], &Gr->GraspMotor[5], -160.0f, 20.0f, 2);
        if (FLIP_1.state == Finish && FLIP_2.state == Finish)
        {
            //UPLIFT.state= DisFinish;
            Gr->state[2]++;
        }
    }
    // 放下
    case 8:
    {

        //uplift(&Gr->GraspMotor[0], Gr->vL53L0, 1.0f, 2);
        //if(UPLIFT.state == Finish)
        //{
        //                FLIP_1.state = DisFinish;
        //                FLIP_2.state = DisFinish;
        //                CLIP.state         = DisFinish ;
        //                TELESCOPING.state  = DisFinish ;
        //                Gr->state[2] =  0 ;
        //                gold_lock = 1; // 解锁
        //}
    }

    default:
        break;
    }
}

#endif 