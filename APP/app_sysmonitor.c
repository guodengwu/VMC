#include "app_sysmonitor.h"

OS_STK      AppSysMonitorStk       [APP_TASK_SYS_MONITOR_STK_SIZE];           // Usart接收任务堆栈
static void SysMonitorTask();
_sysMonitor_t sysMonitor;

void SysMonitorTaskInit (void)
{
    //INT8U  err;

    (void)OSTaskCreate(SysMonitorTask,
                       (void          * ) 0,
                       (OS_STK        * )&AppSysMonitorStk[0],                                 /* Set Top-Of-Stack     */
                       (INT8U           ) APP_SYSMONITOR_TASK_PRIO);
}

static void SysMonitorInit (void)
{
    //sysMonitor.lock         = OSSemCreate(1);
    sysMonitor.sem          = OSSemCreate(0);
		sysMonitor.mbox         = OSMboxCreate((void *)0);
}

static void SysMonitorTask(void *parg)
{
	INT8U err;
	message_pkt_t *msg;
	parg = parg;
	
	while (DEF_True)
	{
		msg = (message_pkt_t *)OSMboxPend(sysMonitor.mbox, 10, &err);//100ms执行
		if(err==OS_NO_ERR)    {
			
		}
	}
}