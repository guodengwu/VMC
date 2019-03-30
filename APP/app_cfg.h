//uC/OS-IIºËÐÄ
#ifndef __APP_CFG__H
#define __APP_CFG__H

//#define START_TASK_Prio 4
#define TEST_TASK_Prio 5
#define LED_TASK_Prio 6

#define  APP_SYSTEM_START_TASK_PRIO                         2       // ??????????????
#define  APP_USART_TX_TASK_PRIO                             3       // Usart?????????????
#define  APP_USART_RX_TASK_PRIO                             4       // Usart?????????????

#define  APP_TASK_STK_SIZE                              MaxStkSize
#define  APP_TASK_SYS_MONITOR_STK_SIZE                 100
#define  APP_TASK_Z_MOTOR_STK_SIZE                   		APP_TASK_STK_SIZE
#define  APP_TASK_USART_RX_STK_SIZE                     APP_TASK_STK_SIZE
#define  APP_TASK_USART_TX_STK_SIZE                     APP_TASK_STK_SIZE*2

#endif