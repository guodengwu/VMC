#include "app_sysmonitor.h"
#include "motor.h"
#include "app_ship.h"

OS_STK   xdata   AppSysMonitorStk       [APP_TASK_SYS_MONITOR_STK_SIZE+1];           // Usart���������ջ
static void SysMonitorTask();
_sysMonitor_t sysMonitor;
static s8 data_buf[15];
static s8 data_buf_2[15];//���Ͼ���
static  message_pkt_t    msg_pkt_sysmonitor[3];

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
    //sysMonitor.sem          = OSSemCreate(0);
		sysMonitor.Mbox         = OSMboxCreate((void *)0);
}
//�������״̬
static void SysCheckOnlineState(void)
{
	static u16 check_cnt,check_timeout=200;
	
	check_cnt++;
	if(sys_status.online_state == DEF_False)	{//
		check_timeout=200;//10s�ϱ�
	}else if(sys_status.online_state == DEF_True)	{
		check_timeout=36000;//30min�ϱ�
	}
	if((check_cnt>=check_timeout)&&(appShip.state == SHIP_STATE_IDLE))	{
		check_cnt = 0;
		msg_pkt_sysmonitor[0].Src = USART_MSG_RX_TASK;
		msg_pkt_sysmonitor[0].Cmd = CMD_CheckOnlineStatus;
		msg_pkt_sysmonitor[0].dLen = 0;
		OSQPost(usart.Str_Q, &msg_pkt_sysmonitor[0]);//����������
	}
}
//�ϴ�ϵͳ����
static u8 UploadSysParam(void)
{
	u8 len=0;
	if(sys_status.online_state == DEF_True)	{
		msg_pkt_sysmonitor[0].Src = USART_MSG_RX_TASK;
		msg_pkt_sysmonitor[0].Cmd = CMD_ReportParam;
		data_buf[len++] = IO_IR_CTRL;
		data_buf[len++] = !sys_status.pTempCtrl->flag;//�¶ȿ��Ʊ�־
		data_buf[len++] = sys_status.pTempCtrl->inside_temp;//
		data_buf[len++] = sys_status.pTempCtrl->outside_temp;//;
		data_buf[len++] = !IO_LIGHT_CTRL;
		data_buf[len++] = !IO_FOG_CTRL;
		data_buf[len++] = IO_DOOR_STATE;
		data_buf[len++] = !HuaShuangCtrl.enable;//��˪���ÿ���
		msg_pkt_sysmonitor[0].Data = (u8 *)data_buf;
		msg_pkt_sysmonitor[0].dLen = len;
		OSQPost(usart.Str_Q, &msg_pkt_sysmonitor[0]);//����������
		return 1;
	}
	return 0;
}
//��⿪����
static void CheckDoorState(void)
{
	static u8 door_state_bk=DOOR_OPEN;
	u8 state;
	
	state = IO_DOOR_STATE;//�Ŵ�/���� �͵�ƽ
	if(state == DEF_True)	{
		sys_status.door_state = DOOR_CLOSE;
	}else	{
		sys_status.door_state = DOOR_OPEN;
		IO_RELAY = 0;
	}
	if((door_state_bk == DOOR_OPEN) && (sys_status.door_state == DOOR_CLOSE))	{//����ʱ�ϱ�������Ϣ
		UploadSysParam();
	}
	door_state_bk = sys_status.door_state;
}
//////////////////ȥ�����ϼ����ϱ�/////////////////////////////////////////////
#if 0 
//�ϱ�ϵͳ����
static u8 ReportSysError(void)
{
	u8 len=0;
	if(sys_status.online_state == DEF_True && appShip.state == SHIP_STATE_IDLE)	{
		msg_pkt_sysmonitor[2].Src = USART_MSG_RX_TASK;
		msg_pkt_sysmonitor[2].Cmd = CMD_ReportError;
		data_buf_2[len++] = sys_status.pError->IR;
		data_buf_2[len++] = sys_status.pError->Compressor;
		data_buf_2[len++] = sys_status.pError->TempSensor;//
		data_buf_2[len++] = 0;//
		msg_pkt_sysmonitor[2].dLen = len;
		msg_pkt_sysmonitor[2].Data = (u8 *)data_buf_2;
		OSQPost(usart.Str_Q, &msg_pkt_sysmonitor[2]);//����������
		return 1;
	}else	{
		return 0;
	}
}
//���ϵͳ����
static void CheckSysError(void)
{
	static u8 IR_bk,Compressor_bk,TempSensor_bk;
	//���ϱ�־�����仯�����ϴ�
	if(IR_bk!=sys_status.pError->IR || Compressor_bk!=sys_status.pError->Compressor || TempSensor_bk!= sys_status.pError->TempSensor)	{
			if(ReportSysError())	{//�ϴ�ok��
				IR_bk = sys_status.pError->IR;
				Compressor_bk = sys_status.pError->Compressor;
				TempSensor_bk = sys_status.pError->TempSensor;
			}	
	}
	if(1==sys_status.pError->IR || 1==sys_status.pError->Compressor || 1==sys_status.pError->TempSensor)	{
			sys_status.pError->flag = DEF_True;//ϵͳ�й���
	}else	{
			sys_status.pError->flag = DEF_False;//ϵͳ�޹���
	}
}
//�й�������£�30min�ϱ�һ��
static void ReportSysError30min(void)
{
	static u16 count;
	if(sys_status.pError->flag)	{
			count++;
			if(count>=36000)	{
				count = 0;
				//ReportSysError();	
			}
	}else	{
		count = 0;
	}
}
#endif
u8 temp_over_flag=0;
/////////////////////////////////////////////////////////////////////////
//��˪����
void SysHuaShuangIOCtrl(u8 onoff)
{
	if(HuaShuangCtrl.enable == DEF_False)	{//δ������˪����
		//IO_HUASHUANG_CTRL = 0;
		HuaShuangCtrl.runflag = DEF_False;
		return;
	}
	if(onoff)	{//�򿪻�˪ ���ر�ѹ����
		IO_HUASHUANG_CTRL = 1;
		HuaShuangCtrl.runflag = DEF_Enable;	
		IO_RELAY = 0;
//		printf("start huashuang.\r\n");
	}else	{//�رջ�˪ ������ѹ����		
		HuaShuangCtrl.runflag = DEF_False;
//		printf("stop huashuang.");
		if(temp_over_flag)	{
			IO_RELAY = 1;
			IO_HUASHUANG_CTRL = 1;
//			printf("open relay.\r\n");
		}
	}
}

static void SysHuaShuangTimeCtrl(void)
{
	if(HuaShuangCtrl.enable == DEF_False)	{//δ������˪����
		IO_HUASHUANG_CTRL = 0;
		return;
	}
	
	if(HuaShuangCtrl.runflag == DEF_Enable)	{//������˪ʱ�����
		HuaShuangCtrl.run_timecnt ++;
		SysHuaShuangIOCtrl(DEF_ON);
		if(HuaShuangCtrl.run_timecnt>=HuaShuangCtrl.run_time)	{
			SysHuaShuangIOCtrl(DEF_OFF);
			HuaShuangCtrl.run_timecnt = 0;
		}
	}else if(HuaShuangCtrl.runflag == DEF_False)	{//ֹͣ��˪ʱ�����
		HuaShuangCtrl.stop_timecnt++;
		SysHuaShuangIOCtrl(DEF_OFF);
		if(HuaShuangCtrl.stop_timecnt>=HuaShuangCtrl.run_interval)	{
			SysHuaShuangIOCtrl(DEF_ON);
			HuaShuangCtrl.stop_timecnt = 0;
		}
	}
	/*HuaShuangCtrl.run_timecnt ++;
	HuaShuangCtrl.run_intercnt ++;
	if(HuaShuangCtrl.run_timecnt>=HuaShuangCtrl.run_time)	{
		SysHuaShuangIOCtrl(DEF_OFF);
		HuaShuangCtrl.run_timecnt = 0;
		HuaShuangCtrl.run_intercnt = 0;
	}else {
		if(HuaShuangCtrl.run_intercnt>=HuaShuangCtrl.run_interval)	{//����ʱ�������� ͣrun_interval ����run_interval
			if(HuaShuangCtrl.runflag==DEF_Enable)	{
				SysHuaShuangIOCtrl(DEF_OFF);
			}else if(HuaShuangCtrl.runflag==DEF_False)	{
				SysHuaShuangIOCtrl(DEF_ON);
			}
			HuaShuangCtrl.run_intercnt = 0;
		}
	}*/
}
//1min�¼�
static void SysMinEvent(void)
{
	static u16 count;
	static u16 _1min_cnt;
	
	count++;
	if(count>=1200)	{//1min
		count = 0;
		_1min_cnt ++;
		SysHuaShuangTimeCtrl();//��˪����ʱ�����

		if((_1min_cnt%30) == 0)	{//30min
			UploadSysParam();//�ϴ�ϵͳ����
		}
	}
}
u8 close_relay_cnt=0;
static void CalcInsideTemp(void)
{
	static u16 count;
	u16 Vad;
	float temp,Rx;
	count++;
	if(count>=100)	{//5s����һ���¶�
		count=0;
		temp = Cal_Vol(ADC_CH7,1);
		Vad = (u16)(temp*1000);
		temp = (float)Vad/(5000-Vad);
		Rx = temp*51000;
		temp = CalculateTemperature(Rx,10000,3950);
		sys_status.pTempCtrl->inside_temp = (s8)temp;
		//printf("Vad:%d, T:%bd",Vad,sys_status.inside_temp);
		if((sys_status.pTempCtrl->flag == DEF_True)&&(sys_status.door_state == DOOR_CLOSE))	{//�¶ȿ���ʹ�ܣ�VMC�Զ������¶�
			if(sys_status.pTempCtrl->inside_temp > sys_status.pTempCtrl->tInsideTempH)	{//�����¶ȴ���Ŀ���¶����ֵ����ѹ����	
				if(HuaShuangCtrl.runflag == DEF_False)		{//��˪����ʱ ѹ�������ܿ���			
					IO_RELAY = 1;
					//printf("open relay.\r\n");
				}
				//SysHuaShuangIOCtrl(DEF_ON);
				IO_HUASHUANG_CTRL = 1;
				temp_over_flag = 1;
				close_relay_cnt = 0;
			}else if(sys_status.pTempCtrl->inside_temp < sys_status.pTempCtrl->tInsideTempL)	{//С��Ŀ���¶���Сֵ �ر�ѹ����
				close_relay_cnt ++;
				if(close_relay_cnt>6)	{//����30s�� ��⵽����Ŀ���¶� �ر�ѹ����
					close_relay_cnt = 0;
					IO_RELAY = 0;
					//IO_HUASHUANG_CTRL = 0;
					temp_over_flag = 0;
				}
			}
		}
		else /*if(sys_status.door_state == DOOR_OPEN)*/	{
			IO_RELAY = 0;
			IO_HUASHUANG_CTRL = 0;
			temp_over_flag = 0;
		}
	}
}

static void CalcOutsideTemp(void)
{
	static u16 count;
	u16 Vad;
	float temp,Rx;
	count++;
	if(count>=100)	{//5s����һ���¶�
		count=0;
		temp = Cal_Vol(ADC_CH4,1);
		Vad = (u16)(temp*1000);
		temp = (float)Vad/(5000-Vad);
		Rx = temp*51000;
		temp = CalculateTemperature(Rx,10000,3950);
		sys_status.pTempCtrl->outside_temp = (s8)temp;
	}
}
//�������������ֵ���жϰ���/ҹ�����Ƶ�on/off
#define LIGTH_SENSOR_Rmin			5//��λk
#define LIGTH_SENSOR_Rmax			10
static void CalLightSensor(void)
{
	static u16 count,testcnt;
	u16 Vad,Rx;
	float temp;
	
	if(sys_status.light_ctrl_enable)	{//�ƿ���ʹ��
			count++;
			if(count>=20)	{//1s����һ�ι�������
				count=0;
				temp = Cal_Vol(ADC_CH3,1);
				Vad = (u16)(temp*1000);
				temp = (float)(5000-Vad)/Vad;
				Rx = (u16)(temp*10);//��λk
				if(Rx>LIGTH_SENSOR_Rmin && Rx<LIGTH_SENSOR_Rmax)	{//������
						testcnt++;
						if(testcnt>=3)	{//����3�ζ��������� �����
								IO_LIGHT_CTRL = 0;
						}
				}else {
						IO_LIGHT_CTRL = 1;
						testcnt = 0;
				}
			}
	}else 	{
			IO_LIGHT_CTRL = 0;
			count=0;
			testcnt = 0;
	}
}

static void UploadShipResult(void)
{
	if(SaveShipDat.flag == DEF_True)	{
			msg_pkt_sysmonitor[1].Src = USART_MSG_RX_TASK;
			msg_pkt_sysmonitor[1].Cmd = CMD_ReportShipResult;
			msg_pkt_sysmonitor[1].Data = SaveShipDat.buf;
			msg_pkt_sysmonitor[1].dLen = SaveShipDat.len;
			OSQPost(usart.Str_Q, &msg_pkt_sysmonitor[1]);//�����������
	}
}
static u8 CopyShipResultFromEEPROM(void)	
{
	u8 tempbuf[60];
	u16 calcCRC=0;
	_SaveShipData_t  *pSaveShipDat;
	
	pSaveShipDat = (_SaveShipData_t *)tempbuf;
	//����Ƿ���δ�ϴ��ĳ������
	EEPROM_read_n(ShipRes_EEPROMAddr,tempbuf,sizeof(SaveShipDat));
	if((pSaveShipDat->flag)&&pSaveShipDat->len<50)	{
		calcCRC = pSaveShipDat->flag + pSaveShipDat->len + crc16(pSaveShipDat->buf,pSaveShipDat->len);
		if(calcCRC == pSaveShipDat->crc16)	{//У������
				memcpy((u8 *)&SaveShipDat,(u8 *)pSaveShipDat,sizeof(SaveShipDat));
				BSP_PRINTF("copy data ok\r\n");
				return 1;
		}
	}
	//��鱸�����Ƿ�������
	EEPROM_read_n(ShipResBK_EEPROMAddr,tempbuf,sizeof(SaveShipDat));
	if((pSaveShipDat->flag)&&pSaveShipDat->len<50)	{
		calcCRC = pSaveShipDat->flag + pSaveShipDat->len + crc16(pSaveShipDat->buf,pSaveShipDat->len);
		if(calcCRC == pSaveShipDat->crc16)	{
				memcpy((u8 *)&SaveShipDat,(u8 *)pSaveShipDat,sizeof(SaveShipDat));
			BSP_PRINTF("copy backup data ok\r\n");
				return 1;
		}
	}
	return 0;
}
//static u8 startup_flag;
static void SysMonitorTask(void *parg)
{
	INT8U err;
	message_pkt_t *msg;
	parg = parg;
	
	SysMonitorInit();
	CopyShipResultFromEEPROM();
//	printf("sys startup.\r\n");
	while (DEF_True)
	{
		msg = (message_pkt_t *)OSMboxPend(sysMonitor.Mbox, 50, &err);//50msִ��
		if(err==OS_NO_ERR)    {
				if(msg->Src==MSG_SYSTEM_RESTART)     {
//					BSP_PRINTF("cmd reset\r\n");
					stop_motor();
					OSTimeDlyHMSM(0,0,0,120);
					soft_reset();
				}else if(msg->Src==MSG_SYS_ONLINE)	{
						UploadSysParam();//���B��server �ψ�ϵ�y����
						//ReportSysError();//�ψ����
						UploadShipResult();//�z���Ƿ���δ�ς��ɹ��ĳ�؛�Y��
				}
		}else if(err==OS_TIMEOUT)	{
				SysCheckOnlineState();//δ��������£�5s���һ�Σ���������£�30min���һ��
				CheckDoorState();//�����Ŷ���
//				CheckSysError();//��⵽���ϱ仯�����ϱ�
				SysMinEvent();//������Ҫ30min�ϱ����¼�
				CalcInsideTemp();
				CalcOutsideTemp();
				CalLightSensor();
				//CheckMotorMoveState();//�����ת״̬���			
				save_data();
		}
	}
}