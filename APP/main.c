//头文件
#include "includes.h"

////////////////////////////////////////////////////////////////////////
/*功能说明
实时监测接收串口1的数据
串口1发的数据段之间最少间隔时间为20ms
接收到数据段 “lamp light 1” LED1就亮
接收到数据段 “lamp light 2” LED2就亮
接收到数据段 “lamp light 3” LED3就亮
接收到数据段 “lamp light 0” LED123全灭

这里LED4留着当闪烁灯用 亮1s灭1s 交替
有3个LED实验足够了
*/
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////
//uCOSII 系统入口
void main(void)
{
	OSInit();               //OS系统初始化
	SystickInitOfTmer0();   //系统滴答计数器初始化 采用TMER0 
   
	OSTaskCreate(TaskStart,
                (void*)0,
                &TASK_START_STK[0],
                APP_SYSTEM_START_TASK_PRIO
                );          //起始任务 又这个任务再创建其他任务
	OSStart();              //启动OS系统 并且打开总中断开关  即EA=1
}



