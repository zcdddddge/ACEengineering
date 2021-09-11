#include "Data_Task.h"
#include "GraspMotor.h"
#include "BoardCommuni.h"
#include "Grasp_Task.h"
#include "FreeRTOS.h"
#include "task.h"

extern Grasp_t Grasp;

void Data_Task(void *pvParameters)
{
    vTaskDelay(500);
    Grasp.Can2_RC = Return_Board_Communi();

    while(1)
    {
        Update_Sensor_Val();
        Update_VL53L0_Val();
        Board_Communi_Updata();
        vTaskDelay(2);
    }
}
