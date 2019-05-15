#include "motor.h"
#include "app_ship.h"

_motor_t motor;
static  message_pkt_t    msg_pkt_motor;

void motor_init(void)
{
		motor.timer = Timer1;
	  motor.status.is_run = MotorState_Stop;
		motor.row = 0;
		motor.col = 0;
		motor.timecnt = 0;
//		motor.plusecnt = 0;
		motor.timeout = 60;//单位100ms
}

static void start_motor_timer(_motor_t *pMotor)
{
	TimerSetValue(pMotor->timer,pMotor->timeout);
	TimerStart(pMotor->timer);
}

static void stop_motor_timer(_motor_t *pMotor)
{
	TimerStop(pMotor->timer);
}

static void motor_choose(_motor_t *pMotor)
{
	BIT8 motor_npn_l,motor_pnp_l;
	BIT8 motor_npn_h,motor_pnp_h;
	u16 row_tmp=0,col_tmp=0;

	motor_npn_l.ubyte=0;
	motor_pnp_l.ubyte=0;
	motor_npn_h.ubyte=0;
	motor_pnp_h.ubyte=0;
	
	row_tmp = pMotor->row-1;
	col_tmp = pMotor->col-1;
	if(row_tmp>=8)
		motor_pnp_h.ubyte = (1<<(row_tmp-8));//货盘
	else
		motor_pnp_l.ubyte = (1<<(row_tmp));//货盘
	if(col_tmp>=8)
		motor_npn_h.ubyte = (1<<(col_tmp-8));//货道
	else
		motor_npn_l.ubyte = (1<<(col_tmp));//货道
	if(10 == pMotor->row)	{
		GPIO_InitTypeDef	GPIO_InitStructure;
		GPIO_InitStructure.Pin  = GPIO_Pin_2;
		GPIO_InitStructure.Mode = GPIO_OUT_PP;//
		GPIO_Inilize(GPIO_P4,&GPIO_InitStructure);
	}
	
	MOTOR_NPN0 = motor_npn_l.bits.b0;
	MOTOR_NPN1 = motor_npn_l.bits.b1;
	MOTOR_NPN2 = motor_npn_l.bits.b2;
	MOTOR_NPN3 = motor_npn_l.bits.b3;
	MOTOR_NPN4 = motor_npn_l.bits.b4;
	MOTOR_NPN5 = motor_npn_l.bits.b5;
	MOTOR_NPN6 = motor_npn_l.bits.b6;
	MOTOR_NPN7 = motor_npn_l.bits.b7;
	MOTOR_NPN8 = motor_npn_h.bits.b0;
	MOTOR_NPN9 = motor_npn_h.bits.b1;
	
	MOTOR_PNP0 = motor_pnp_l.bits.b0;
	MOTOR_PNP1 = motor_pnp_l.bits.b1;
	MOTOR_PNP2 = motor_pnp_l.bits.b2;
	MOTOR_PNP3 = motor_pnp_l.bits.b3;
	MOTOR_PNP4 = motor_pnp_l.bits.b4;
	MOTOR_PNP5 = motor_pnp_l.bits.b5;
	MOTOR_PNP6 = motor_pnp_l.bits.b6;
	MOTOR_PNP7 = motor_pnp_l.bits.b7;
	MOTOR_PNP8 = motor_pnp_h.bits.b0;
	MOTOR_PNP9 = motor_pnp_h.bits.b1;
	start_motor_timer(pMotor);
//	motor.plusecnt = 0;
	pMotor->status.is_run = MotorState_Run;
	pMotor->status.abort_type = MotorAbort_NONE;
}
//由timer1定时中断调用，电机运动超时
void motor_timeout_handler(void)
{
	if(motor.status.is_run != MotorState_Run)
		return;
	motor.timecnt ++;
	if(motor.timecnt >= motor.timeout)	{
		motor.status.is_run = MotorState_Stop;
		motor.status.abort_type = MotorAbort_TIMEOUT;
		msg_pkt_motor.Src = MSG_SHIP_MOTOR_ABORT;//出货过程电机运转超时
		OSMboxPost(appShip.MBox, &msg_pkt_motor);
	}
}

u8 start_motor(u8 row, u8 col)
{
	if(row>0&&row<=10&&col>0&&col<=10)	{
		motor.timecnt = 0;
		motor.row = row;
		motor.col = col;	
		motor.checkmovedelay = 0;
		motor_choose(&motor);
		Ext_Enable(EXT_INT0);//开启货物检测
		//Ext_Enable(EXT_INT1);//开启整圈检测
		return 1;
	}else	{
		return 0;
	}
}

void stop_motor()
{
	//stop_motor_timer(&motor);
	///Ext_Disable(EXT_INT0);//关闭货物检测
	//Ext_Disable(EXT_INT1);//关闭整圈检测
	motor.status.is_run = MotorState_Stop;
	MOTOR_NPN0 = 0;//1货道
	MOTOR_NPN1 = 0;
	MOTOR_NPN2 = 0;
	MOTOR_NPN3 = 0;
	MOTOR_NPN4 = 0;
	MOTOR_NPN5 = 0;
	MOTOR_NPN6 = 0;
	MOTOR_NPN7 = 0;
	MOTOR_NPN8 = 0;
	MOTOR_NPN9 = 0;

	MOTOR_PNP0 = 0;//1货盘
	MOTOR_PNP1 = 0;
	MOTOR_PNP2 = 0;
	MOTOR_PNP3 = 0;
	MOTOR_PNP4 = 0;
	MOTOR_PNP5 = 0;
	MOTOR_PNP6 = 0;
	MOTOR_PNP7 = 0;
	MOTOR_PNP8 = 0;
	MOTOR_PNP9 = 0;
	if(10 == motor.row)	{
		GPIO_InitTypeDef	GPIO_InitStructure;
		GPIO_InitStructure.Pin  = GPIO_Pin_2;
		GPIO_InitStructure.Mode = GPIO_HighZ;//
		GPIO_Inilize(GPIO_P4,&GPIO_InitStructure);
	}
}
#if 0
//检测电机是否运转/卡死 测量MOTOR_AD的反馈电压：小于0.2V--未检测到电机；0.2-1.5V--ok；>1.5V--堵转
//被系统监控任务调用，50ms执行一次
void CheckMotorMoveState(void)
{
	u16 Vad;
	float temp;
	//static u8 movecnt;
	//static u8 movecnt1,movecnt2,movecnt3;
	
	if(motor.status.is_run == MotorState_Run)	{
		motor.checkmovedelay++;//movecnt++;
		if(motor.checkmovedelay>=3)	{//移动150ms后，再计算
				temp = Cal_Vol(ADC_CH6,3);
				Vad = (u16)(temp*100);//放大100倍		
				if((Vad<=20)&&(motor.status.is_run == MotorState_Run))	{//电机未检测到转动					
					stop_motor();
					motor.status.abort_type = MotorAbort_UNDETECTED;
					msg_pkt_motor.Src = MSG_SHIP_MOTOR_ABORT;//出货过程电机异常停止
					OSMboxPost(appShip.MBox, &msg_pkt_motor);
				}/*else if(Vad>20&&Vad<=170)	{//ok
					//motor.status.abort_type = MotorAbort_NONE;			
				}else if(Vad>170)	{//					
					stop_motor();
					//BEEP=1;
					motor.status.abort_type = MotorAbort_Stuck;
					msg_pkt_motor.Src = MSG_SHIP_MOTOR_ABORT;//出货过程电机异常停止
					OSMboxPost(appShip.MBox, &msg_pkt_motor);
				}*/
		}
	}else	{
		motor.checkmovedelay = 0;
	}
}
#endif
