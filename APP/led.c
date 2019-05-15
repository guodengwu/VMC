//////////////////////////////////
//头文件
#include "includes.h"
#include "led.h"
#if 0
//////////////////////////////////
//堆栈
OS_STK  TASK_LED_STK[MaxStkSize+1]; //任务堆栈声明
u8 eeprom_wdata[10] = {1,2,3,4,5,6,7,8,9,10};
u8 eeprom_rdata[10] = {0};

///////////////////////////////////////////////
//LED任务
void TaskLed(void * ppdata)
{
	char i;
	ppdata = ppdata;
	
	OSTimeDlyHMSM(0,0,1,500);  
	EEPROM_SectorErase(0);
	EEPROM_write_n(0,eeprom_wdata,10);
	EEPROM_read_n(0,eeprom_rdata,10);
	BSP_PRINTF("eeprom rdata: ");
	for(i=0;i<10;i++)	{
		BSP_PRINTF("%x ",eeprom_rdata[i]);
	}
	BSP_PRINTF("\r\n");
	OSTaskSuspend(OS_PRIO_SELF);
	/*for(;;)
	{          
        //LED4=0;
        //OSTimeDlyHMSM(0,0,0,500); 
        //LED4=1;  
      OSTimeDlyHMSM(0,0,0,500);  
			EEPROM_write_n(0,eeprom_data,10);
			EEPROM_read_n();
    }*/
}
#endif

