
#ifndef __TASK_globalvariable_H__
#define __TASK_globalvariable_H__

#include "includes.h"

typedef struct _sys_status	{
	volatile s8 inside_temp;
	//volatile u8 IR_CheckFlag;
}_sys_status_t;

extern _sys_status_t sys_status;
void globalvariable(void);//�û�Ӧ�ó���ȫ�ֱ�����ʼ��

#endif

