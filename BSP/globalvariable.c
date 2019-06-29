#include "globalvariable.h"

_sys_status_t sys_status;
_imei_t IMEI;
_syserror_t SysError;
_systempctrl_t TempCtrl;
_HuaShuangCtrl_t HuaShuangCtrl;
_SaveShipData_t SaveShipDat;

//////////////////////////////////////////////////////////////////
//�û�Ӧ�ó���ȫ�ֱ�����ʼ��
void globalvariable (void)
{
		sys_status.IR_CheckFlag = DEF_False;//�������־
		sys_status.light_ctrl_enable = DEF_True;//�ƹ����ʹ��
		sys_status.online_state = DEF_False;//����״�
		sys_status.door_state = DOOR_OPEN;
		sys_status.pIMEI = &IMEI;//������
		SysError.IR = 0;
		SysError.Compressor = 0;
		SysError.TempSensor = 0;
		SysError.flag = DEF_False;//�����ǣ�Ĭ���޴���
		sys_status.pError = &SysError;
		TempCtrl.inside_temp = -50;//Ĭ�������¶�
		TempCtrl.outside_temp = -50;
		TempCtrl.tInsideTempL = 0;//�����¶ȿ��Ʒ�Χ
		TempCtrl.tInsideTempH = 0;
		TempCtrl.flag = DEF_False;//Ĭ��VMC���Զ������¶�
		sys_status.pTempCtrl = &TempCtrl;
		SaveShipDat.flag = DEF_False;
		flash_savedat.type = SAVE_NONE;
	
		HuaShuangCtrl.enable = DEF_False;
		HuaShuangCtrl.runflag = DEF_False;
}

















