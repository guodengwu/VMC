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
    USART_MSG_RX_TASK = 0,      // 邮箱消息来自Usart Rx(串口接收)任务(Mbox From Usart Message Receive Task)
	USART_MSG_ACK_TASK,
    MSG_START_SHIP,          		// 邮箱消息来自Slider Motor(滑动电机)任务(Mbox From Slider Motor Task)
    MSG_SHIP_MOTOR_NOMAL,        		// 邮箱消息来自Rotating Motor(旋转电机)任务(Mbox From Rotating Motor Task)
    SYSTEM_MONITOR_TASK,        // 邮箱消息来自System Monitor(系统监控)任务(Mbox From System Monitor Task)
    MSG_SHIP_MOTOR_ABORT,//
    MSG_SYSTEM_RESTART,//重启
		MSG_SYS_ONLINE,
	MSG_SYS_SAVE_SHIPRESULT,
} message_src_t;

typedef struct _message_pkt {
    message_src_t   Src;        // 消息来源
    u8           Cmd;        // 命令
    u16 			 pLen;		//参数长度
    u16          dLen;    // 消息长度
    u8         *Data;       // 消息参数
} message_pkt_t;

void msg_init(void);
void mutex_lock (OS_EVENT *lock);
void mutex_unlock (OS_EVENT *lock);
INT8U  UsartRxGetINT8U (u8 *buf,u32 *idx);
INT16U  UsartRxGetINT16U (u8 *buf,u32 *idx);
INT32U  UsartRxGetINT32U (u8 *buf,u32 *idx);
u16 crc16(u8 *buf,u8 len);


#endif
