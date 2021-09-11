#include "Remote_Task.h"
#include "FreeRTOS.h"
#include "task.h"




void Remote_Task(void *pvParameters)
{
    vTaskDelay(500);

    while(1)
    {
			
        vTaskDelay(2);
    }
}

