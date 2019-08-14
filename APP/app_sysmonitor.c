#include "app_sysmonitor.h"
#include "motor.h"
#include "app_ship.h"

OS_STK   xdata   AppSysMonitorStk       [APP_TASK_SYS_MONITOR_STK_SIZE+1];           // Usart接收任务堆栈
static void SysMonitorTask();
_sysMonitor_t sysMonitor;
static s8 data_buf[15];
static s8 data_buf_2[15];//故障緩存
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
//检测联网状态
static void SysCheckOnlineState(void)
{
	static u16 check_cnt,check_timeout=200;
	
	check_cnt++;
	if(sys_status.online_state == DEF_False)	{//
		check_timeout=200;//10s上报
	}else if(sys_status.online_state == DEF_True)	{
		check_timeout=36000;//30min上报
	}
	if((check_cnt>=check_timeout)&&(appShip.state == SHIP_STATE_IDLE))	{
		check_cnt = 0;
		msg_pkt_sysmonitor[0].Src = USART_MSG_RX_TASK;
		msg_pkt_sysmonitor[0].Cmd = CMD_CheckOnlineStatus;
		msg_pkt_sysmonitor[0].dLen = 0;
		OSQPost(usart.Str_Q, &msg_pkt_sysmonitor[0]);//请求网络检查
	}
}
//上传系统参数
static u8 UploadSysParam(void)
{
	u8 len=0;
	if(sys_status.online_state == DEF_True)	{
		msg_pkt_sysmonitor[0].Src = USART_MSG_RX_TASK;
		msg_pkt_sysmonitor[0].Cmd = CMD_ReportParam;
		data_buf[len++] = IO_IR_CTRL;
		data_buf[len++] = !sys_status.pTempCtrl->flag;//温度控制标志
		data_buf[len++] = sys_status.pTempCtrl->inside_temp;//
		data_buf[len++] = sys_status.pTempCtrl->outside_temp;//;
		data_buf[len++] = !IO_LIGHT_CTRL;
		data_buf[len++] = !IO_FOG_CTRL;
		data_buf[len++] = IO_DOOR_STATE;
		data_buf[len++] = !HuaShuangCtrl.enable;//化霜设置开关
		msg_pkt_sysmonitor[0].Data = (u8 *)data_buf;
		msg_pkt_sysmonitor[0].dLen = len;
		OSQPost(usart.Str_Q, &msg_pkt_sysmonitor[0]);//请求网络检查
		return 1;
	}
	return 0;
}
//检测开关门
static void CheckDoorState(void)
{
	static u8 door_state_bk=DOOR_OPEN;
	u8 state;
	
	state = IO_DOOR_STATE;//门打开/常开 低电平
	if(state == DEF_True)	{
		sys_status.door_state = DOOR_CLOSE;
	}else	{
		sys_status.door_state = DOOR_OPEN;
		IO_RELAY = 0;
	}
	if((door_state_bk == DOOR_OPEN) && (sys_status.door_state == DOOR_CLOSE))	{//关门时上报参数信息
		UploadSysParam();
	}
	door_state_bk = sys_status.door_state;
}
//////////////////去掉故障检测和上报/////////////////////////////////////////////
#if 0 
//上报系统故障
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
		OSQPost(usart.Str_Q, &msg_pkt_sysmonitor[2]);//请求网络检查
		return 1;
	}else	{
		return 0;
	}
}
//检测系统故障
static void CheckSysError(void)
{
	static u8 IR_bk,Compressor_bk,TempSensor_bk;
	//故障标志发生变化立刻上传
	if(IR_bk!=sys_status.pError->IR || Compressor_bk!=sys_status.pError->Compressor || TempSensor_bk!= sys_status.pError->TempSensor)	{
			if(ReportSysError())	{//上传ok，
				IR_bk = sys_status.pError->IR;
				Compressor_bk = sys_status.pError->Compressor;
				TempSensor_bk = sys_status.pError->TempSensor;
			}	
	}
	if(1==sys_status.pError->IR || 1==sys_status.pError->Compressor || 1==sys_status.pError->TempSensor)	{
			sys_status.pError->flag = DEF_True;//系统有故障
	}else	{
			sys_status.pError->flag = DEF_False;//系统无故障
	}
}
//有故障情况下，30min上报一次
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
//化霜控制
void SysHuaShuangIOCtrl(u8 onoff)
{
	if(HuaShuangCtrl.enable == DEF_False)	{//未开启化霜控制
		//IO_HUASHUANG_CTRL = 0;
		HuaShuangCtrl.runflag = DEF_False;
		return;
	}
	if(onoff)	{//打开化霜 即关闭压缩机
		IO_HUASHUANG_CTRL = 1;
		HuaShuangCtrl.runflag = DEF_Enable;	
		IO_RELAY = 0;
//		printf("start huashuang.\r\n");
	}else	{//关闭化霜 即启动压缩机		
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
	if(HuaShuangCtrl.enable == DEF_False)	{//未开启化霜控制
		IO_HUASHUANG_CTRL = 0;
		return;
	}
	
	if(HuaShuangCtrl.runflag == DEF_Enable)	{//开启化霜时间控制
		HuaShuangCtrl.run_timecnt ++;
		SysHuaShuangIOCtrl(DEF_ON);
		if(HuaShuangCtrl.run_timecnt>=HuaShuangCtrl.run_time)	{
			SysHuaShuangIOCtrl(DEF_OFF);
			HuaShuangCtrl.run_timecnt = 0;
		}
	}else if(HuaShuangCtrl.runflag == DEF_False)	{//停止化霜时间控制
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
		if(HuaShuangCtrl.run_intercnt>=HuaShuangCtrl.run_interval)	{//运行时间间隔控制 停run_interval 运行run_interval
			if(HuaShuangCtrl.runflag==DEF_Enable)	{
				SysHuaShuangIOCtrl(DEF_OFF);
			}else if(HuaShuangCtrl.runflag==DEF_False)	{
				SysHuaShuangIOCtrl(DEF_ON);
			}
			HuaShuangCtrl.run_intercnt = 0;
		}
	}*/
}
//1min事件
static void SysMinEvent(void)
{
	static u16 count;
	static u16 _1min_cnt;
	
	count++;
	if(count>=1200)	{//1min
		count = 0;
		_1min_cnt ++;
		SysHuaShuangTimeCtrl();//化霜运行时间控制

		if((_1min_cnt%30) == 0)	{//30min
			UploadSysParam();//上传系统参数
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
	if(count>=100)	{//5s计算一次温度
		count=0;
		temp = Cal_Vol(ADC_CH7,1);
		Vad = (u16)(temp*1000);
		temp = (float)Vad/(5000-Vad);
		Rx = temp*51000;
		temp = CalculateTemperature(Rx,10000,3950);
		sys_status.pTempCtrl->inside_temp = (s8)temp;
		//printf("Vad:%d, T:%bd",Vad,sys_status.inside_temp);
		if((sys_status.pTempCtrl->flag == DEF_True)&&(sys_status.door_state == DOOR_CLOSE))	{//温度控制使能，VMC自动调节温度
			if(sys_status.pTempCtrl->inside_temp > sys_status.pTempCtrl->tInsideTempH)	{//室内温度大于目标温度最大值，打开压缩机	
				if(HuaShuangCtrl.runflag == DEF_False)		{//化霜开启时 压缩机不能开启			
					IO_RELAY = 1;
					//printf("open relay.\r\n");
				}
				//SysHuaShuangIOCtrl(DEF_ON);
				IO_HUASHUANG_CTRL = 1;
				temp_over_flag = 1;
				close_relay_cnt = 0;
			}else if(sys_status.pTempCtrl->inside_temp < sys_status.pTempCtrl->tInsideTempL)	{//小于目标温度最小值 关闭压缩机
				close_relay_cnt ++;
				if(close_relay_cnt>6)	{//连续30s内 检测到低于目标温度 关闭压缩机
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
	if(count>=100)	{//5s计算一次温度
		count=0;
		temp = Cal_Vol(ADC_CH4,1);
		Vad = (u16)(temp*1000);
		temp = (float)Vad/(5000-Vad);
		Rx = temp*51000;
		temp = CalculateTemperature(Rx,10000,3950);
		sys_status.pTempCtrl->outside_temp = (s8)temp;
	}
}
//计算光敏电阻阻值，判断白天/夜晚，控制灯on/off
#define LIGTH_SENSOR_Rmin			5//单位k
#define LIGTH_SENSOR_Rmax			10
static void CalLightSensor(void)
{
	static u16 count,testcnt;
	u16 Vad,Rx;
	float temp;
	
	if(sys_status.light_ctrl_enable)	{//灯控制使能
			count++;
			if(count>=20)	{//1s计算一次光敏电阻
				count=0;
				temp = Cal_Vol(ADC_CH3,1);
				Vad = (u16)(temp*1000);
				temp = (float)(5000-Vad)/Vad;
				Rx = (u16)(temp*10);//单位k
				if(Rx>LIGTH_SENSOR_Rmin && Rx<LIGTH_SENSOR_Rmax)	{//亮电阻
						testcnt++;
						if(testcnt>=3)	{//连续3次都是亮电阻 灯灭掉
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
			OSQPost(usart.Str_Q, &msg_pkt_sysmonitor[1]);//反馈出货结果
	}
}
static u8 CopyShipResultFromEEPROM(void)	
{
	u8 tempbuf[60];
	u16 calcCRC=0;
	_SaveShipData_t  *pSaveShipDat;
	
	pSaveShipDat = (_SaveShipData_t *)tempbuf;
	//检查是否有未上传的出货结果
	EEPROM_read_n(ShipRes_EEPROMAddr,tempbuf,sizeof(SaveShipDat));
	if((pSaveShipDat->flag)&&pSaveShipDat->len<50)	{
		calcCRC = pSaveShipDat->flag + pSaveShipDat->len + crc16(pSaveShipDat->buf,pSaveShipDat->len);
		if(calcCRC == pSaveShipDat->crc16)	{//校验数据
				memcpy((u8 *)&SaveShipDat,(u8 *)pSaveShipDat,sizeof(SaveShipDat));
				BSP_PRINTF("copy data ok\r\n");
				return 1;
		}
	}
	//检查备份区是否有数据
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
		msg = (message_pkt_t *)OSMboxPend(sysMonitor.Mbox, 50, &err);//50ms执行
		if(err==OS_NO_ERR)    {
				if(msg->Src==MSG_SYSTEM_RESTART)     {
//					BSP_PRINTF("cmd reset\r\n");
					stop_motor();
					OSTimeDlyHMSM(0,0,0,120);
					soft_reset();
				}else if(msg->Src==MSG_SYS_ONLINE)	{
						UploadSysParam();//剛連上server 上報系統參數
						//ReportSysError();//上報故障
						UploadShipResult();//檢查是否有未上傳成功的出貨結果
				}
		}else if(err==OS_TIMEOUT)	{
				SysCheckOnlineState();//未联网情况下，5s检测一次，联网情况下，30min检测一次
				CheckDoorState();//检测关门动作
//				CheckSysError();//检测到故障变化立刻上报
				SysMinEvent();//所有需要30min上报的事件
				CalcInsideTemp();
				CalcOutsideTemp();
				CalLightSensor();
				//CheckMotorMoveState();//电机运转状态检测			
				save_data();
		}
	}
}