#include "app_sysmonitor.h"
#include "motor.h"
#include "app_ship.h"

OS_STK      AppSysMonitorStk       [APP_TASK_SYS_MONITOR_STK_SIZE];           // Usart接收任务堆栈
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
		data_buf[len++] = sys_status.pTempCtrl->flag;//温度控制标志
		data_buf[len++] = sys_status.pTempCtrl->inside_temp;//
		data_buf[len++] = sys_status.pTempCtrl->outside_temp;//;
		data_buf[len++] = IO_LIGHT_CTRL;
		data_buf[len++] = IO_FOG_CTRL;
		data_buf[len++] = IO_DOOR_STATE;
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
	}
	if((door_state_bk == DOOR_OPEN) && (sys_status.door_state == DOOR_CLOSE))	{//关门时上报参数信息
		UploadSysParam();
	}
	door_state_bk = sys_status.door_state;
}
//上报系统故障
static u8 ReportSysError(void)
{
	u8 len=0;
	if(sys_status.online_state == DEF_True)	{
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
				ReportSysError();	
			}
	}else	{
		count = 0;
	}
}
	
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
		if(sys_status.pTempCtrl->flag == DEF_True)	{//温度控制使能，VMC自动调节温度
			if(sys_status.pTempCtrl->inside_temp > sys_status.pTempCtrl->tInsideTempH)	{//室内温度不在要求范围内，打开压缩机
					IO_RELAY = 1;
			}
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
//static u8 startup_flag;
static void SysMonitorTask(void *parg)
{
	INT8U err;
	message_pkt_t *msg;
	parg = parg;
	
	SysMonitorInit();
	while (DEF_True)
	{
		msg = (message_pkt_t *)OSMboxPend(sysMonitor.Mbox, 50, &err);//50ms执行
		if(err==OS_NO_ERR)    {
				if(msg->Src==MSG_SYSTEM_RESTART)     {
//					BSP_PRINTF("cmd reset\r\n");
					stop_motor();
					OSTimeDlyHMSM(0,0,0,500);
					soft_reset();
				}else if(msg->Src==MSG_SYS_ONLINE)	{
						UploadSysParam();//剛連上server 上報系統參數
						ReportSysError();//上報故障
						UploadShipResult();//檢查是否有未上傳成功的出貨結果
				}else if(msg->Src==MSG_SYS_SAVE_SHIPRESULT)	{//保存出貨結果
					
				}
		}else if(err==OS_TIMEOUT)	{
				SysCheckOnlineState();//未联网情况下，5s检测一次，联网情况下，30min检测一次
				CheckDoorState();//检测关门动作
				CheckSysError();//检测到故障变化立刻上报
				ReportSysError30min();//有故障30min上报一次
				/*if(startup_flag==0)	{//开机且连上网络后，上报一次系统参数
					if(UploadSysParam()==1)	{//参数上报成功
						startup_flag = 1;
					}
				}*/
				CalcInsideTemp();
				CalcOutsideTemp();
				CalLightSensor();
				//CheckMotorMoveState();//电机运转状态检测			
		}
	}
}