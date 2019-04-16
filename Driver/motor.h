#ifndef _MOTOR_H
#define _MOTOR_H

#include "includes.h"

enum {
	MOTORSTATUS_NO_ERR = 0,//无故障
	MOTORSTATUS_NO_MOTOR = 1,//电机未接入或者霍尔板有问题
	MOTORSTATUS_MOVE_TIMEOUT = 2,//电机运行超时
	MOTORSTATUS_ID_ERR = 3,//电机变化错误
};

enum {
	SHIP_IDLE = 0,
	SHIPING = 1,
	SHIP_OK = 2,
	SHIP_FAILED = 3,
};

typedef struct {
    volatile INT8U   ship_status;
    volatile INT8U   errCode;
    //INT8U   abort_type;
} motor_state_t;

typedef struct _motor {
	u8 timer;
	motor_state_t status;
	u8 row;
	u8 col;
	u8 timeout;
	volatile u16 timecnt;
}_motor_t;

extern _motor_t motor;

void motor_init(void);
u8 start_motor(u8 row, u8 col);
void stop_motor(_motor_t *pMotor);


#endif