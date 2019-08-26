#include "app_display.h"
#include "tm1640.h"

OS_STK      AppDisplayStk       [APP_TASK_DISPLAY_STK_SIZE];
_display_t display_t;
static void AppDisplayTask(void *parg);
	
void DisplayTaskInit (void)
{
//    INT8U  err;
	
    (void)OSTaskCreate(AppDisplayTask,
                       (void          * ) 0,
                       (OS_STK        * )&AppDisplayStk[0],                                 /* Set Top-Of-Stack     */
                       (INT8U           ) APP_DISPLAY_TASK_PRIO);
}

static void DisDataInit(void)
{
	display_t.mbox = OSMboxCreate((void *)0);
	display_t.ui_flag = UI_OUTSIDETEMP;
}

static void WatchDogEnable(void)
{
//  WDT_CONTR = 0x23;                           //使能看门狗,溢出时间约为0.5s
    WDT_CONTR = 0x25;                           //使能看门狗,溢出时间约为1s
//  WDT_CONTR = 0x27;                           //使能看门狗,溢出时间约为8s
}

static void WatchDogFeed(void)
{
	 WDT_CONTR |= 0x10; //清看门狗,否则系统复位
}

void	DisplayUI(void)
{
	static u8 count=0;
	
	count++;
	if(count>=100)	{
		count = 0;
		 /*if(display_t.ui_flag==UI_MOTORNUM)	{
			display_t.ui_flag=UI_INSIDETEMP;
			Update_DisInt(sys_status.pTempCtrl->inside_temp);
		}else */if(display_t.ui_flag==UI_INSIDETEMP)	{//显示室内温度	
			display_t.ui_flag=UI_OUTSIDETEMP;
			Update_DisInt(sys_status.pTempCtrl->outside_temp);
		}else if(display_t.ui_flag==UI_OUTSIDETEMP)	{//显示室外温度
			/*display_t.ui_flag=UI_MOTORNUM;
			Update_DisInt(15);*/
			display_t.ui_flag=UI_INSIDETEMP;
			Update_DisInt(sys_status.pTempCtrl->inside_temp);
		}
	}
}

static void AppDisplayTask(void *parg)
{
	u8 err;
	message_pkt_t *msg;
	parg = parg;
		
	DisDataInit();
	Update_DisString("----");
	BEEP=1;
	OSTimeDlyHMSM(0,0,1,0); 
	//Update_DisInt(15);
	BEEP=0;
	//Ext_Enable(EXT_INT0);
	BSP_PRINTF("SYS Startup...\r\n");
	
	while (DEF_True)
	{
		msg = (message_pkt_t *)OSMboxPend(display_t.mbox, 30, &err);//系統tick 10ms
		if(err==OS_NO_ERR)	{
			
		}else if(err==OS_TIMEOUT)	{//3s 切换数码管显示内容
			DisplayUI();
			WatchDogFeed();
		}
	}
}