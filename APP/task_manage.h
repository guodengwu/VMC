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

extern OS_STK  TASK_START_STK[MaxStkSize+1];  //�����ջ

extern void TaskStart(void * ppdata) reentrant;          //��ʼ���� ����������ٴ�����������

#endif

/************************ (C) COPYLEFT 2010 Leafgrass ************************/
