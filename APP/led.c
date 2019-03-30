

//////////////////////////////////
//头文件
#include "includes.h"

//////////////////////////////////
//堆栈
OS_STK  TASK_LED_STK[MaxStkSize+1]; //任务堆栈声明


///////////////////////////////////////////////
//LED任务
void TaskLed(void * ppdata) reentrant
{
	ppdata = ppdata;
	for(;;)
	{          
        LED4=0;
        OSTimeDlyHMSM(0,0,0,500); 
        LED4=1;  
        OSTimeDlyHMSM(0,0,0,500);  
    }
}


