#include "motor.h"

_motor_t motor;

void motor_init(void)
{
		motor.timer = Timer1;
	  motor.status.errCode = MOTORSTATUS_NO_ERR;
		motor.status.ship_status = SHIP_IDLE;
		motor.row = 0;
		motor.col = 0;
		motor.timeout = 0;
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
	BIT32 motor_npn,motor_pnp;
	u8 row_tmp,col_tmp;

	row_tmp = pMotor->row-1;
	col_tmp = pMotor->col;
	motor_npn.uword = (1<<row_tmp);
	motor_pnp.uword = (1<<col_tmp);
	
	MOTOR_NPN0 = motor_npn.bits.b0;
	MOTOR_NPN1 = motor_npn.bits.b1;
	MOTOR_NPN2 = motor_npn.bits.b2;
	MOTOR_NPN3 = motor_npn.bits.b3;
	MOTOR_NPN4 = motor_npn.bits.b4;
	MOTOR_NPN5 = motor_npn.bits.b5;
	MOTOR_NPN6 = motor_npn.bits.b6;
	MOTOR_NPN7 = motor_npn.bits.b7;
	MOTOR_NPN8 = motor_npn.bits.b8;
	MOTOR_NPN9 = motor_npn.bits.b9;
	
	MOTOR_PNP0 = motor_pnp.bits.b0;
	MOTOR_PNP1 = motor_pnp.bits.b1;
	MOTOR_PNP2 = motor_pnp.bits.b2;
	MOTOR_PNP3 = motor_pnp.bits.b3;
	MOTOR_PNP4 = motor_pnp.bits.b4;
	MOTOR_PNP5 = motor_pnp.bits.b5;
	MOTOR_PNP6 = motor_pnp.bits.b6;
	MOTOR_PNP7 = motor_pnp.bits.b7;
	MOTOR_PNP8 = motor_pnp.bits.b8;
	MOTOR_PNP9 = motor_pnp.bits.b9;
}
//由timer1定时中断调用，电机运动超时
void motor_timer_handler(void)
{
	motor.status.errCode = MOTORSTATUS_MOVE_TIMEOUT;
}

void start_motor(u8 row, u8 col, u8 param, u8 timeout)
{
	motor.row = row;
	motor.col = col;
	motor.timeout = timeout;
	motor_choose(&motor);
	start_motor_timer(&motor);
}