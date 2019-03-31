#ifndef _APP_SYSMONITOR_H
#define _APP_SYSMONITOR_H

#include "includes.h"

typedef struct _sysMonitor {
    OS_EVENT           *lock;
    OS_EVENT           *sem;
    OS_EVENT           *mbox;
}_sysMonitor_t;

void SysMonitorTaskInit (void);
	
#endif