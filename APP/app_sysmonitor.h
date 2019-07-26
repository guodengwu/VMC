#ifndef _APP_SYSMONITOR_H
#define _APP_SYSMONITOR_H

#include "includes.h"

typedef struct _sysMonitor {
   // OS_EVENT           *lock;
    //OS_EVENT           *sem;
    OS_EVENT           *Mbox;
}_sysMonitor_t;

void SysMonitorTaskInit (void);
void SysHuaShuangIOCtrl(u8 onoff);
extern _sysMonitor_t sysMonitor;
#endif
