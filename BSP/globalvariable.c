#include "globalvariable.h"

_sys_status_t sys_status;
_imei_t IMEI;
_syserror_t SysError;
//////////////////////////////////////////////////////////////////
//ÓÃ»§Ó¦ÓÃ³ÌĞòÈ«¾Ö±äÁ¿³õÊ¼»¯
void globalvariable (void)
{
		sys_status.IR_CheckFlag = DEF_False;//»õÎï¼ì²â±êÖ¾
		sys_status.inside_temp = -50;//ÏäÄÚÎÂ¶È£¬Ä¬ÈÏ-50¶È
		sys_status.outside_temp = 0;
		sys_status.light_ctrl_enable = DEF_True;//µÆ¹â¿ØÖÆÊ¹ÄÜ
		sys_status.online_state = DEF_False;//ÁªÍø×´Ì
		sys_status.door_state = DEF_True;
		sys_status.pIMEI = &IMEI;//¶©µ¥ºÅ
		SysError.IR = 0;
		SysError.Compressor = 0;
		SysError.TempSensor = 0;
		SysError.flag = DEF_False;//´íÎó±ê¼Ç£¬Ä¬ÈÏÎŞ´íÎó
		sys_status.pError = &SysError;
}

















