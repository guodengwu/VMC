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
	appShip.Sem          = OSSemCreate(0);
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
					if(sys_status.door_state == DOOR_OPEN)	{//门打开状态 不能出货
							OSTimeDlyHMSM(0,0,0,100); 
							msg_pkt_ship.Src = USART_MSG_RX_TASK;
							msg_pkt_ship.Cmd = CMD_ReportShipResult;
							data_buf[0] = (appShip.pMotor->row<<4)|appShip.pMotor->col;				
							data_buf[1] = SHIP_ERROR_DOOR_OPEN;
					}else	{
							ret = start_motor(appShip.pMotor->row, appShip.pMotor->col);			
							if(ret==0)	{//出货失败--货道号错误
									msg_pkt_ship.Src = USART_MSG_RX_TASK;
									msg_pkt_ship.Cmd = CMD_ReportShipResult;
									data_buf[0] = (appShip.pMotor->row<<4)|appShip.pMotor->col;				
									data_buf[1] = SHIP_ERROR_INVALID_COL;
							}else	{//开启出货流程
									appShip.state = SHIP_STATE_BUSY;//出货状态BUSY
									sys_status.IR_CheckFlag = DEF_False;
									sys_status.ShipStuckFlag = 0;
									OSSemSet(appShip.Sem, 0, &err);
									OSTimeDlyHMSM(0,0,0,200); 
									if(motor.CtrlType == MOTOR_CTRL_TYPE_HOLE)	{
										Ext_Enable(EXT_INT1);//开启霍尔检测
									}
									appShip.pMotor->PluseStartTime = OSTimeGet();
									continue;
							}
					}
			}else if(msg->Src==MSG_SHIP_MOTOR_NOMAL)	{//出货过程电机运转正常
					if(motor.CtrlType == MOTOR_CTRL_TYPE_HOLE)	{
							u32 PluseTime;
							appShip.pMotor->PluseEndTime = OSTimeGet();//one tick 1ms
							if(appShip.pMotor->PluseEndTime >= appShip.pMotor->PluseStartTime)
									PluseTime = appShip.pMotor->PluseEndTime - appShip.pMotor->PluseStartTime;
							else
									PluseTime = (0xffffffff - appShip.pMotor->PluseStartTime) + appShip.pMotor->PluseEndTime;
							appShip.pMotor->PluseStartTime = OSTimeGet();
							if(PluseTime<1300)	{// 1/4圈
								if(sys_status.ShipStuckFlag==1)	{//多走1/4圈 防卡货
										//appShip.pMotor->plusecnt = 0;
										stop_motor();
								}else	{// 1/4圈 继续走
									OSTimeDlyHMSM(0,0,0,200); 
									Ext_Enable(EXT_INT1);
									continue;
								}
							}
							else	if(PluseTime<4000)// 3/4圈 停止
							{
									//appShip.pMotor->plusecnt = 0;
									stop_motor();
							}
							OSSemPend(appShip.Sem, 3000, &err);//电机停止后3s货物检测超时 
					}
					msg_pkt_ship.Src = USART_MSG_RX_TASK;
					msg_pkt_ship.Cmd = CMD_ReportShipResult;
					data_buf[0] = (appShip.pMotor->row<<4)|appShip.pMotor->col;			
					if(sys_status.IR_CheckFlag == DEF_True)	{//检测到货物掉下					
							data_buf[1] = SHIP_NOMAL;//出货正常					
					}else {//未检测到货物掉下
							sys_status.ShipStuckFlag++;
							if(sys_status.ShipStuckFlag>=2)	{
								data_buf[1] = MOTOR_NOMAL_NOSHIP;//电机正常但是无货物
								sys_status.pError->IR = 1;//红外传感器异常
							}else if(sys_status.ShipStuckFlag==1)	{
								start_motor(appShip.pMotor->row, appShip.pMotor->col);
								OSTimeDlyHMSM(0,0,0,200); 
								Ext_Enable(EXT_INT1);
								appShip.pMotor->PluseStartTime = OSTimeGet();
								continue;
							}
					}
			}else if(msg->Src==MSG_SHIP_MOTOR_ABORT)	{//出货过程中电机异常停止
					stop_motor();
					msg_pkt_ship.Src = USART_MSG_RX_TASK;
					msg_pkt_ship.Cmd = CMD_ReportShipResult;
					data_buf[0] = (appShip.pMotor->row<<4)|appShip.pMotor->col;
					data_buf[1] = motor.status.abort_type;//停止原因
			}
			memcpy(data_buf+2, sys_status.pIMEI->dat, sys_status.pIMEI->len);//拷贝订单号
			msg_pkt_ship.Data = data_buf;
			msg_pkt_ship.dLen = 2 + sys_status.pIMEI->len;			
			appShip.state = SHIP_STATE_IDLE;			//出货状态空闲
			//Ext_Disable(EXT_INT0);
			Ext0_Disable();
			stop_motor();
			OSMboxPost(usart.MBox, &msg_pkt_ship);//反馈出货结果
		}
	}
}
