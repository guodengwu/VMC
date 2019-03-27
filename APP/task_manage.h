/************************ (C) COPYLEFT 2010 Leafgrass *************************

* File Name          : task_manage.h
* Author             : Librae
* Last Modified Date : 08/15/2010
* Description        : Tasks management.

******************************************************************************/

/* Define to prevent recursive inclusion ----------------------------------- */
#ifndef __TASK_MANAGE_H__
#define __TASK_MANAGE_H__

#include "includes.h"

extern OS_STK  TASK_START_STK[MaxStkSize+1];  //任务堆栈

extern void TaskStart(void * ppdata) reentrant;          //起始任务 又这个任务再创建其他任务

#endif

/************************ (C) COPYLEFT 2010 Leafgrass ************************/
