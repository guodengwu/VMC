

/* Define to prevent recursive inclusion ----------------------------------- */
#ifndef __TASK_TEST_H__
#define __TASK_TEST_H__
 
////////////////////////////
//��ͷ�ļ�
#include "includes.h"

extern OS_STK  TASK_TEST_STK[MaxStkSize+1];
extern void TaskTest(void * ppdata) reentrant;

#endif

