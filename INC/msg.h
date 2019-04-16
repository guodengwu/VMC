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
    MSG_START_SHIP,          		// 邮箱消息来自Slider Motor(滑动电机)任务(Mbox From Slider Motor Task)
    MSG_SHIP_MOTOR_NOMAL,        		// 邮箱消息来自Rotating Motor(旋转电机)任务(Mbox From Rotating Motor Task)
    SYSTEM_MONITOR_TASK,        // 邮箱消息来自System Monitor(系统监控)任务(Mbox From System Monitor Task)
    MSG_SHIP_MOTOR_ABORT,//
    MSG_SYSTEM_RESTART,//阻抗单孔测试
    IMP_AUTO_TEST,
    IMP_TEMPERATURE_TEST,//温度电阻阻抗采集
    IMP_SYS_POWER_TEST,//系统功率采集
    /* ISR Mbox */
    USB_DATA_SEND_TO_SLAVE,                // 邮箱消息来自Usart接收中断
    USB_DATA_SEND_TO_SLAVE_NO_FEEDBACK,
    MSG96LED_CLOSE,
    ACC_INT_ENVENT,//加速度传感器中断事件
    ZMOTOR_RESET_POWERON,
    REFRESH_DEVICE_AREA
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
