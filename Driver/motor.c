#include "motor.h"
#include "app_ship.h"

_motor_t motor;
static  message_pkt_t    msg_pkt_motor;

void motor_init(void)
{
		motor.timer = Timer1;
	  motor.status.errCode = MOTORSTATUS_NO_ERR;
		motor.status.ship_status = SHIP_IDLE;
		motor.row = 0;
		motor.col = 0;
		motor.timecnt = 0;
		motor.timeout = 27;//单位100ms
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
	motor_pnp_l.ubyte = (1<<(row_tmp&0xff));//货盘
	motor_npn_l.ubyte = (1<<(col_tmp&0xff));//货道
	motor_pnp_h.ubyte = (1<<(row_tmp>>8));//货盘
	motor_npn_h.ubyte = (1<<(col_tmp>>8));//货道
	
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
}
//由timer1定时中断调用，电机运动超时
void motor_timer_handler(void)
{
	//motor.status.errCode = MOTORSTATUS_MOVE_TIMEOUT;
	motor.timecnt ++;
	if(motor.timecnt >= motor.timeout)	{
		stop_motor(&motor);
		stop_motor_timer(&motor);
		msg_pkt_motor.Src = MSG_SHIP_MOTOR_NOMAL;//出货过程电机正常停止
		OSMboxPost(appShip.MBox, &msg_pkt_motor);
	}
}

u8 start_motor(u8 row, u8 col)
{
	if(row>0&&row<=10&&col>0&&col<=10)	{
		motor.timecnt = 0;
		motor.row = row;
		motor.col = col;
		motor_choose(&motor);
		return 1;
	}else	{
		return 0;
	}
}

void stop_motor(_motor_t *pMotor)
{
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
}