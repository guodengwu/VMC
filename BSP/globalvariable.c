#include "globalvariable.h"

_sys_status_t sys_status;
//////////////////////////////////////////////////////////////////
//用户应用程序全局变量初始化
void globalvariable (void)
{
		//sys_status.IR_CheckFlag = DEF_False;
		sys_status.inside_temp = -50;
		memset(sys_status.IMEI,0,IMEI_LEN);
}

















