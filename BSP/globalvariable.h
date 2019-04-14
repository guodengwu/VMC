
#ifndef __TASK_globalvariable_H__
#define __TASK_globalvariable_H__

#include "includes.h"

#define IMEI_LEN		100

typedef struct _sys_status	{
	volatile s8 inside_temp;
	//volatile u8 IR_CheckFlag;
	u8 IMEI[IMEI_LEN];
}_sys_status_t;

extern _sys_status_t sys_status;
void globalvariable(void);//用户应用程序全局变量初始化

#endif

