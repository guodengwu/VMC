#ifndef __APP_SHIP__
#define __APP_SHIP__

#include "includes.h"
#include "motor.h"
//出货状态
enum SHIP_STATE {
	SHIP_STATE_IDLE = 0,
	SHIP_STATE_BUSY = 1,
	//SHIP_STATE_COMPLETE = 2,
};
//出货结果
enum SHIP_RESULT {
	SHIP_NOMAL = 0,
	MOTOR_NOMAL_NOSHIP = 1,
	SHIP_ERROR_INVALID_COL = 2,
	SHIP_ERROR_DOOR_OPEN = 3,
	//SHIP_NOMAL = 4,
	SHIP_BUSY = 5,
};

typedef struct _appship {
    OS_EVENT           *Sem;
    OS_EVENT           *MBox;
		volatile u8 state;
		_motor_t *pMotor;
}appship_t;

extern appship_t appShip;
void ShipTaskInit (void);

#endif