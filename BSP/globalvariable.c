#include "globalvariable.h"

_sys_status_t sys_status;
_imei_t IMEI;
_syserror_t SysError;
//////////////////////////////////////////////////////////////////
//�û�Ӧ�ó���ȫ�ֱ�����ʼ��
void globalvariable (void)
{
		sys_status.IR_CheckFlag = DEF_False;//�������־
		sys_status.inside_temp = -50;//�����¶ȣ�Ĭ��-50��
		sys_status.outside_temp = 0;
		sys_status.light_ctrl_enable = DEF_True;//�ƹ����ʹ��
		sys_status.online_state = DEF_False;//����״�
		sys_status.door_state = DEF_True;
		sys_status.pIMEI = &IMEI;//������
		SysError.IR = 0;
		SysError.Compressor = 0;
		SysError.TempSensor = 0;
		SysError.flag = DEF_False;//�����ǣ�Ĭ���޴���
		sys_status.pError = &SysError;
}

















