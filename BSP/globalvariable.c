#include "globalvariable.h"

_sys_status_t sys_status;
_imei_t IMEI;
_syserror_t SysError;
//////////////////////////////////////////////////////////////////
//用户应用程序全局变量初始化
void globalvariable (void)
{
		sys_status.IR_CheckFlag = DEF_False;//货物检测标志
		sys_status.inside_temp = -50;//箱内温度，默认-50度
		sys_status.outside_temp = 0;
		sys_status.light_ctrl_enable = DEF_True;//灯光控制使能
		sys_status.online_state = DEF_False;//联网状�
		sys_status.door_state = DEF_True;
		sys_status.pIMEI = &IMEI;//订单号
		SysError.IR = 0;
		SysError.Compressor = 0;
		SysError.TempSensor = 0;
		SysError.flag = DEF_False;//错误标记，默认无错误
		sys_status.pError = &SysError;
}

















