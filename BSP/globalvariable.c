#include "globalvariable.h"

_sys_status_t sys_status;
_imei_t IMEI;
_syserror_t SysError;
_systempctrl_t TempCtrl;
_HuaShuangCtrl_t HuaShuangCtrl;
_SaveShipData_t SaveShipDat;

//////////////////////////////////////////////////////////////////
//用户应用程序全局变量初始化
void globalvariable (void)
{
		sys_status.IR_CheckFlag = DEF_False;//货物检测标志
		sys_status.light_ctrl_enable = DEF_True;//灯光控制使能
		sys_status.online_state = DEF_False;//联网状�
		sys_status.door_state = DOOR_OPEN;
		sys_status.pIMEI = &IMEI;//订单号
		SysError.IR = 0;
		SysError.Compressor = 0;
		SysError.TempSensor = 0;
		SysError.flag = DEF_False;//错误标记，默认无错误
		sys_status.pError = &SysError;
		TempCtrl.inside_temp = -50;//默认室内温度
		TempCtrl.outside_temp = -50;
		TempCtrl.tInsideTempL = 0;//室内温度控制范围
		TempCtrl.tInsideTempH = 0;
		TempCtrl.flag = DEF_False;//默认VMC不自动调节温度
		sys_status.pTempCtrl = &TempCtrl;
		SaveShipDat.flag = DEF_False;
		flash_savedat.type = SAVE_NONE;
	
		HuaShuangCtrl.enable = DEF_False;
		HuaShuangCtrl.runflag = DEF_False;
}

















