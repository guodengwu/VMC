

/* Define to prevent recursive inclusion ----------------------------------- */
#ifndef __TASK_LED_H__
#define __TASK_LED_H__
 
////////////////////////////
//��ͷ�ļ�
#include "includes.h"

extern OS_STK  TASK_LED_STK[MaxStkSize+1];
extern void TaskLed(void * ppdata) reentrant;


#endif

