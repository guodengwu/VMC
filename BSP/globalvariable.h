
#ifndef __TASK_globalvariable_H__
#define __TASK_globalvariable_H__

#include "includes.h"

#define IMEI_LEN		50
#define DOOR_OPEN			DEF_True
#define DOOR_CLOSE		DEF_False
#define TEMP_RANGE		3//温度误差

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

typedef struct _systempctrl	{
	volatile s8 inside_temp;
	volatile s8 outside_temp;
	s8 tInsideTempL;//目标温度-最低
	s8 tInsideTempH;//目标温度-最高
	u8 flag;
}_systempctrl_t;

typedef struct _sys_status	{
	volatile u8 IR_CheckFlag;
	volatile u8 light_ctrl_enable;
	volatile u8 online_state;//联网状态
	volatile u8 door_state;	
	volatile u8 ShipStuckFlag;
	_imei_t *pIMEI;
	_syserror_t *pError;
	_systempctrl_t  *pTempCtrl;
}_sys_status_t;

typedef struct _SaveShipData	{
	u8 flag;
	u8 len;
	u8 buf[50];
	u16 crc16;
}_SaveShipData_t;
extern _sys_status_t sys_status;
extern _SaveShipData_t SaveShipDat;
void globalvariable(void);//用户应用程序全局变量初始化

#endif

