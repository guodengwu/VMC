#include "app_ship.h"

OS_STK      AppShipStk       [APP_TASK_SHIP_STK_SIZE];           // Usart���������ջ
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
					if(sys_status.door_state == DOOR_OPEN)	{//�Ŵ�״̬ ���ܳ���
							OSTimeDlyHMSM(0,0,0,100); 
							msg_pkt_ship.Src = USART_MSG_RX_TASK;
							msg_pkt_ship.Cmd = CMD_ReportShipResult;
							data_buf[0] = (appShip.pMotor->row<<4)|appShip.pMotor->col;				
							data_buf[1] = SHIP_ERROR_DOOR_OPEN;
					}else	{
							ret = start_motor(appShip.pMotor->row, appShip.pMotor->col);			
							if(ret==0)	{//����ʧ��--�����Ŵ���
									msg_pkt_ship.Src = USART_MSG_RX_TASK;
									msg_pkt_ship.Cmd = CMD_ReportShipResult;
									data_buf[0] = (appShip.pMotor->row<<4)|appShip.pMotor->col;				
									data_buf[1] = SHIP_ERROR_INVALID_COL;
							}else	{
									appShip.state = SHIP_STATE_BUSY;//����״̬BUSY
									sys_status.IR_CheckFlag = DEF_False;
									OSSemSet(appShip.Sem, 0, &err);
									continue;
							}
					}
			}else if(msg->Src==MSG_SHIP_MOTOR_NOMAL)	{//�������̵����ת����
					u32 PluseTime;
					appShip.pMotor->plusecnt++;
					_nop_();
					if(appShip.pMotor->plusecnt==1)	{
							appShip.pMotor->PluseStartTime = OSTimeGet();
							OSTimeDlyHMSM(0,0,0,80); 
							Ext_Enable(EXT_INT1);
							continue;
					}else if(appShip.pMotor->plusecnt>=2)	{							
							appShip.pMotor->PluseEndTime = OSTimeGet();//one tick 10ms
							if(appShip.pMotor->PluseEndTime >= appShip.pMotor->PluseStartTime)
									PluseTime = appShip.pMotor->PluseEndTime - appShip.pMotor->PluseStartTime;
							else 
									PluseTime = (0xffffffff - appShip.pMotor->PluseStartTime) + appShip.pMotor->PluseEndTime;
							if(PluseTime>=1200)	{//1.2s
									appShip.pMotor->plusecnt = 0;
									stop_motor();
							}else	{
									appShip.pMotor->PluseStartTime = OSTimeGet();
									OSTimeDlyHMSM(0,0,0,80); 
									Ext_Enable(EXT_INT1);
									continue;
							}
					}					
					OSSemPend(appShip.Sem, 3000, &err);//���ֹͣ��3s�����ⳬʱ 
					msg_pkt_ship.Src = USART_MSG_RX_TASK;
					msg_pkt_ship.Cmd = CMD_ReportShipResult;
					data_buf[0] = (appShip.pMotor->row<<4)|appShip.pMotor->col;			
					if(sys_status.IR_CheckFlag == DEF_True)	{//��⵽�������					
							data_buf[1] = SHIP_NOMAL;//��������					
					}else {//δ��⵽�������
							data_buf[1] = MOTOR_NOMAL_NOSHIP;//������������޻���
							sys_status.pError->IR = 1;//���⴫�����쳣
					}
			}else if(msg->Src==MSG_SHIP_MOTOR_ABORT)	{//���������е���쳣ֹͣ
					stop_motor();
					msg_pkt_ship.Src = USART_MSG_RX_TASK;
					msg_pkt_ship.Cmd = CMD_ReportShipResult;
					data_buf[0] = (appShip.pMotor->row<<4)|appShip.pMotor->col;
					data_buf[1] = motor.status.abort_type;//ֹͣԭ��
			}
			memcpy(data_buf+2, sys_status.pIMEI->dat, sys_status.pIMEI->len);//����������
			msg_pkt_ship.Data = data_buf;
			msg_pkt_ship.dLen = 2 + sys_status.pIMEI->len;			
			appShip.state = SHIP_STATE_IDLE;			//����״̬����
			Ext_Disable(EXT_INT0);
			OSQPost(usart.Str_Q, &msg_pkt_ship);//�����������
		}
	}
}
