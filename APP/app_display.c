#include "app_display.h"
#include "tm1640.h"

OS_STK      AppDisplayStk       [APP_TASK_DISPLAY_STK_SIZE];
_display_t display_t;
static void AppDisplayTask(void *parg);
	
void DisplayTaskInit (void)
{
    INT8U  err;
	
    (void)OSTaskCreate(AppDisplayTask,
                       (void          * ) 0,
                       (OS_STK        * )&AppDisplayStk[0],                                 /* Set Top-Of-Stack     */
                       (INT8U           ) APP_DISPLAY_TASK_PRIO);
}

static void DisDataInit(void)
{
	display_t.mbox = OSMboxCreate((void *)0);
	display_t.ui_flag = UI_MOTORNUM;
}

static void AppDisplayTask(void *parg)
{
	u8 err;
	message_pkt_t *msg;
	parg = parg;
		
	DisDataInit();
	Update_DisString("----");
	BEEP=1;
	OSTimeDlyHMSM(0,0,1,500); 
	Update_DisInt(15);
	BEEP=0;
	
	while (DEF_True)
	{
		msg = (message_pkt_t *)OSMboxPend(display_t.mbox, 500, &err);//Ïµ½ytick 10ms
		if(err==OS_NO_ERR)	{
			
		}else if(err==OS_TIMEOUT)	{
			if(display_t.ui_flag==UI_TEMP)	{				
				display_t.ui_flag=UI_MOTORNUM;
				Update_DisInt(15);
			}else if(display_t.ui_flag==UI_MOTORNUM)	{
				display_t.ui_flag=UI_TEMP;
				Update_DisInt(-50);
			}
		}
	}
}