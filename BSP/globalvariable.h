
#ifndef __TASK_globalvariable_H__
#define __TASK_globalvariable_H__

#include "includes.h"

#define IMEI_LEN		100
#define DOOR_OPEN			DEF_True
#define DOOR_CLOSE		DEF_False

typedef struct _imei	{
	char dat[IMEI_LEN];
	u8 len;
}_imei_t;

typedef struct _syserror	{
	u8 IR;
	u8 Compressor;
	u8 TempSensor;
	u8 flag;
}_syserror_t;

typedef struct _sys_status	{
	volatile s8 inside_temp;
	volatile s8 outside_temp;
	volatile u8 IR_CheckFlag;
	volatile u8 light_ctrl_enable;
	volatile u8 online_state;//联网状态
	volatile u8 door_state;
	s16 tTemp;//目标温度
	_imei_t *pIMEI;
	_syserror_t *pError;
}_sys_status_t;

extern _sys_status_t sys_status;
void globalvariable(void);//用户应用程序全局变量初始化

#endif

