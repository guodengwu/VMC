#ifndef __APP_SHIP__
#define __APP_SHIP__

#include "includes.h"
#include "motor.h"

enum SHIP_MSG_ERR {
	SHIP_NOMAL = 0,
	MOTOR_NOMAL_NOSHIP = 1,
	SHIP_ERROR_INVALID_COL = 2,
	SHIP_ERROR_DOOR_OPEN = 3,
	SHIP_MOTOR_STUCK = 4,
};
typedef struct _appship {
    OS_EVENT           *lock;
    OS_EVENT           *MBox;
	
		_motor_t *pMotor;
}appship_t;

extern appship_t appShip;
void ShipTaskInit (void);

#endif