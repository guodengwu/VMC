#ifndef _APP_DISPLAY_H
#define _APP_DISPLAY_H

#include "includes.h"

enum UI_NUM {
	UI_NONE=0,
	UI_MOTORNUM=1,
	UI_INSIDETEMP=2,
	UI_OUTSIDETEMP=3,
};

typedef struct _display {
    OS_EVENT           *lock;
    OS_EVENT           *sem;
    OS_EVENT           *mbox;
	
		u8 ui_flag;
}_display_t;

void DisplayTaskInit (void);

#endif