#include "app_ship.h"

OS_STK      AppShipStk       [APP_TASK_SHIP_STK_SIZE];           // Usart接收任务堆栈
static void AppShipTask(void *parg);
static u8 data_buf[100];
static  message_pkt_t    msg_pkt_ship;
appship_t appShip;

void ShipTaskInit (void)
{
//    INT8U  err;
	
    (void)OSTaskCreate(AppShipTask,
                       (void          * ) 0,
                       (OS_STK        * )&AppShipStk[0],                                 /* Set Top-Of-Stack     */
                       (INT8U           ) APP_SHIP_TASK_PRIO);
}

static void AppShipInit (void)
{
	//appShip.sem          = OSSemCreate(0);
	appShip.MBox         = OSMboxCreate((void *)0);
	appShip.state = SHIP_STATE_IDLE;
	appShip.pMotor = &motor;
}

static void AppShipTask(void *parg)
{
	u8 err,ret;
	message_pkt_t *msg;
	parg = parg;
			
	AppShipInit();
	
	while (DEF_True)
	{
		msg = (message_pkt_t *)OSMboxPend(appShip.MBox, 0, &err);
		if(err==OS_NO_ERR)    {
			if(msg->Src==MSG_START_SHIP)     {
					ret = start_motor(appShip.pMotor->row, appShip.pMotor->col);			
					if(ret==0)	{//出货失败--货道号错误
							msg_pkt_ship.Src = USART_MSG_RX_TASK;
							msg_pkt_ship.Cmd = CMD_ReportShipResult;
							data_buf[0] = (appShip.pMotor->row<<4)|appShip.pMotor->col;				
							data_buf[1] = SHIP_ERROR_INVALID_COL;
					}else	{
							appShip.state = SHIP_STATE_BUSY;//出货状态BUSY
							sys_status.IR_CheckFlag = DEF_False;
							Ext_Enable(EXT_INT0);//开启货物检测
							continue;
					}
			}else if(msg->Src==MSG_SHIP_MOTOR_NOMAL)	{//出货过程电机运转正常
					OSTimeDlyHMSM(0,0,3,0);//电机停止后3s货物检测超时 
					msg_pkt_ship.Src = USART_MSG_RX_TASK;
					msg_pkt_ship.Cmd = CMD_ReportShipResult;
					data_buf[0] = (appShip.pMotor->row<<4)|appShip.pMotor->col;			
					if(sys_status.IR_CheckFlag == DEF_True)	{//检测到货物掉下					
							data_buf[1] = SHIP_NOMAL;//出货正常					
					}else {//未检测到货物掉下
							data_buf[1] = MOTOR_NOMAL_NOSHIP;//电机正常但是无货物
							sys_status.pError->IR = 1;//红外传感器异常
					}
					Ext_Disable(EXT_INT0);//关闭货物检测
			}else if(msg->Src==MSG_SHIP_MOTOR_ABORT)	{//出货过程中电机异常停止
					Ext_Disable(EXT_INT0);//关闭货物检测
					msg_pkt_ship.Src = USART_MSG_RX_TASK;
					msg_pkt_ship.Cmd = CMD_ReportShipResult;
					data_buf[0] = (appShip.pMotor->row<<4)|appShip.pMotor->col;
					data_buf[1] = motor.status.abort_type;//停止原因
			}
			memcpy(data_buf+2, sys_status.pIMEI->dat, sys_status.pIMEI->len);//拷贝订单号
			msg_pkt_ship.Data = data_buf;
			msg_pkt_ship.dLen = 2 + sys_status.pIMEI->len;
			OSMboxPost(usart.mbox, &msg_pkt_ship);//反馈出货结果
			appShip.state = SHIP_STATE_IDLE;			//出货状态空闲
		}
	}
}