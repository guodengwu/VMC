/* Includes -----------------------------------------------------------------*/
#include "includes.h"
#include "app_usart.h"
#include "app_display.h"
#include "app_sysmonitor.h"
#include "app_ship.h"

//////////////////////////////////////////////////////////
OS_STK  TASK_START_STK[MaxStkSize+1]; //任务堆栈声明

//////////////////////////////////////////////////////////
void TaskCreateOther(void) reentrant;             //用这个函数来创建其他任务

//////////////////////////////////////////////////////////
//起始任务 又这个任务再创建其他任务
void TaskStart(void * ppdata) reentrant
{
	ppdata = ppdata;
	
    ET0=1;                              //使能时钟节拍中断-------根据杨屹文章更改
    
    //使能ucos 的统计任务
    #if (OS_TASK_STAT_EN > 0)
    OSStatInit();                       //统计任务初始化函数   
    #endif  
    
    bsp();                              //板载硬件初始化
    globalvariable();                   //用户应用程序全局变量初始化
    TaskCreateOther();			        //建立其他的任务
    OSTaskSuspend(OS_PRIO_SELF);	    //suspend but not delete 挂起任务	
}

/////////////////////////////////////////////////////////////////////////////
//创建其他所有任务 在这里我建立了个LED闪烁的任务当例子
void TaskCreateOther(void) reentrant
{
    //OSTaskCreate(TaskTest, (void * )0, (OS_STK *)&TASK_TEST_STK[0], TEST_TASK_Prio);                  //TEST任务�
    //OSTaskCreate(TaskLed,  (void * )0, (OS_STK *)&TASK_LED_STK[0], 8);                     //LED显示任务�		
		UsartTxTaskInit();
		UsartRxTaskInit();
		DisplayTaskInit();
		SysMonitorTaskInit();
		ShipTaskInit();
}


      
