

//////////////////////////////////
//ͷ�ļ�
#include "includes.h"

//////////////////////////////////
//��ջ
OS_STK  TASK_LED_STK[MaxStkSize+1]; //�����ջ����


///////////////////////////////////////////////
//LED����
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


