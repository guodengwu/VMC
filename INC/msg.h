#ifndef __MSG_H__
#define __MSG_H__

#include    "os_cpu.h"
#include    "os_cfg.h"
#include "sys_defs.h"
#include "sys_types.h"
#include "sys_bits.h"
#include    "ucos_ii.h"

typedef enum _message_src{
    /* Task Mbox */
    USART_MSG_RX_TASK = 0,      // ������Ϣ����Usart Rx(���ڽ���)����(Mbox From Usart Message Receive Task)
    MSG_START_SHIP,          		// ������Ϣ����Slider Motor(�������)����(Mbox From Slider Motor Task)
    MSG_SHIP_MOTOR_NOMAL,        		// ������Ϣ����Rotating Motor(��ת���)����(Mbox From Rotating Motor Task)
    SYSTEM_MONITOR_TASK,        // ������Ϣ����System Monitor(ϵͳ���)����(Mbox From System Monitor Task)
    MSG_SHIP_MOTOR_ABORT,//
    MSG_SYSTEM_RESTART,//�迹���ײ���
    IMP_AUTO_TEST,
    IMP_TEMPERATURE_TEST,//�¶ȵ����迹�ɼ�
    IMP_SYS_POWER_TEST,//ϵͳ���ʲɼ�
    /* ISR Mbox */
    USB_DATA_SEND_TO_SLAVE,                // ������Ϣ����Usart�����ж�
    USB_DATA_SEND_TO_SLAVE_NO_FEEDBACK,
    MSG96LED_CLOSE,
    ACC_INT_ENVENT,//���ٶȴ������ж��¼�
    ZMOTOR_RESET_POWERON,
    REFRESH_DEVICE_AREA
} message_src_t;

typedef struct _message_pkt {
    message_src_t   Src;        // ��Ϣ��Դ
    u8           Cmd;        // ����
    u16 			 pLen;		//��������
    u16          dLen;    // ��Ϣ����
    u8         *Data;       // ��Ϣ����
} message_pkt_t;

void msg_init(void);
void mutex_lock (OS_EVENT *lock);
void mutex_unlock (OS_EVENT *lock);
INT8U  UsartRxGetINT8U (u8 *buf,u32 *idx);
INT16U  UsartRxGetINT16U (u8 *buf,u32 *idx);
INT32U  UsartRxGetINT32U (u8 *buf,u32 *idx);
u16 crc16(u8 *buf,u8 len);


#endif
