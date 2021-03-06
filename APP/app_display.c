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

void	DisplayUI(void)
{
	static u16 count=0;
	
	count++;
	if(count>=30)	{
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
	//WatchDogEnable();
	
	while (DEF_True)
	{
		msg = (message_pkt_t *)OSMboxPend(display_t.mbox, 100, &err);//系統tick 1ms
		if(err==OS_NO_ERR)	{
			
		}else if(err==OS_TIMEOUT)	{//3s 切换数码管显示内容
			DisplayUI();
			save_data();
		}
	}
}
