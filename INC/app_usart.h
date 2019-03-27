/*
H********************************************************************************************************
*                                      AutoSampler System Firmware
*
*                    (c) Copyright 2010-2013 ACEA Biosciences, Inc.
*                    All rights reserved.  Protected by international copyright laws.
*
* File      :  app_usart.h
* By        :  AlexShi
* Email     :  shiweining123@gmail.com
* Version   :  V1.00.00
* Compiler  :  IAR
*********************************************************************************************************
* Note(s)   :
*
*
*
*********************************************************************************************************
*/

#ifndef USART_H
#define USART_H

#include "includes.h"	

/*
*********************************************************************************************************
*                                          INCLUDE HEADER FILES
*********************************************************************************************************
*/
#define USART_BUFF_SIZE                      128u
#define USART_TX_BUFF_SIZE        USART_BUFF_SIZE
#define USART_RX_BUFF_SIZE        USART_BUFF_SIZE
#define USART_BAUDRATE                      115200uL	



/*
*********************************************************************************************************
*                                                  MACROS
*********************************************************************************************************
*/


#define usart_rx_int_enable(pUsart)     ;//USART_ITConfig(pUsart->Usart, USART_IT_RXNE, ENABLE)
#define usart_rx_int_disable(pUsart)    ;//USART_ITConfig(pUsart->Usart, USART_IT_RXNE, DISABLE)
#define usart_tx_int_enable(pUsart)     ;//USART_ITConfig(pUsart->Usart, USART_IT_TXE, ENABLE)
#define usart_tx_int_disable(pUsart)    ;// USART_ITConfig(pUsart->Usart, USART_IT_TXE, DISABLE)

/*
*********************************************************************************************************
*                                            MACROS FUNCTIONS
*********************************************************************************************************
*/




/*
*********************************************************************************************************
*                                             TYPE DEFINITION
*********************************************************************************************************
*/

typedef struct _usart {
    u8      *Usart;
    OS_EVENT           *lock;
    OS_EVENT           *sem;
    OS_EVENT           *mbox;

    INT8U               rx_state;
    INT32U              rx_idx;
    INT32U              rx_cnt;
    INT32U              rx_len;
    INT32U              rx_size;
    INT16U              rx_crc;
    INT16U              rx_err;
    INT8U              *rx_buf;
    u8 (*rx_indicate)(struct _usart *pUsart, INT8U rx_dat);

    INT8U               tx_state;
    INT32U              tx_idx;
    INT32U              tx_len;
    INT32U              tx_size;
    INT16U              tx_crc;
    INT16U              tx_err;
    INT8U              *tx_buf;
    void (*tx_complete)(struct _usart *pUsart);
} usart_t;

#define MSG_SYSTEM_ACK_CMD              0x78    // 系统应答命令
#define MSG_ABNORMAL_CMD                0x79    // 消息错误命令


/*
*********************************************************************************************************
*                                            EXTERN VARIABLES
*********************************************************************************************************
*/




/*
*********************************************************************************************************
*                                            GLOBAL CONSTANTS
*********************************************************************************************************
*/




/*
*********************************************************************************************************
*                                            GLOBAL VARIABLES
*********************************************************************************************************
*/

extern   usart_t      usart;


/*
*********************************************************************************************************
*                                           FUNCTION PROTOTYPES
*********************************************************************************************************
*/
void usart_mutex_lock    (usart_t *pUsart);
void usart_mutex_unlock  (usart_t *pUsart);
INT8U   uart_rx_byte             (usart_t *pUsart, void *buf);
INT8U   UartRxGetINT8U           (usart_t *pUsart);
INT16U  UartRxGetINT16U          (usart_t *pUsart);
INT32U  UartRxGetINT32U          (usart_t *pUsart);
void    UartTxStoINT8U           (usart_t *pUsart, INT8U tx_dat);
void    UartTxStoINT16U          (usart_t *pUsart, INT16U tx_dat);
void    UartTxStoINT32U          (usart_t *pUsart, INT32U tx_dat);
void    UartTxStoData            (usart_t *pUsart, void *pbuf, INT32U len);
void    UsartTxStoStr            (usart_t *pUsart, INT8U *s);
void    usart_tx_start           (usart_t *pUsart, message_pkt_t *pmsg);
void    usart_set_rx_indicate    (usart_t *pUsart, void (*rx_ind)(usart_t *pUsart, INT8U rx_dat));
void    usart_set_tx_complete    (usart_t *pUsart, void (*tx_com)(usart_t *pUsart));
//void    usart_printf             (const INT8U *fmt, ...);
void    get_hole_idx             (INT8U *x, INT8U *y);
void    UsartRxTaskInit          (void);
void    UsartTxTaskInit          (void);
void    UsartInit                (void);
//void    UsartSendAck        (message_pkt_t *pMsg, INT8U ack);


#endif    /* USART_H */

/*
*********************************************************************************************************
*                                                No More!
*********************************************************************************************************
*/
