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
#define USART_TX_BUFF_SIZE        COM_TX1_Lenth
#define USART_RX_BUFF_SIZE        COM_RX1_Lenth
#define USART_BAUDRATE                      115200uL	



/*
*********************************************************************************************************
*                                                  MACROS
*********************************************************************************************************
*/


#define usart_rx_int_enable(pUsart)     ;//USART_ITConfig(pUsart->Usart, USART_IT_RXNE, ENABLE)
#define usart_rx_int_disable(pUsart)    ;//USART_ITConfig(pUsart->Usart, USART_IT_RXNE, DISABLE)
#define usart_tx_int_enable(pUsart)     (TI = 1);//USART_ITConfig(pUsart->Usart, USART_IT_TXE, ENABLE)
#define usart_tx_int_disable(pUsart)    (TI = 0);// USART_ITConfig(pUsart->Usart, USART_IT_TXE, DISABLE)

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
    u8      Usart;
    OS_EVENT           *lock;
    OS_EVENT           *sem;
    OS_EVENT           *ack_sem;
		OS_EVENT           *Str_Q;

    INT8U               rx_state;
    INT32U              rx_idx;
    INT8U              rx_cnt;
		//INT8U              rx_cmd;
    INT16U              rx_len;
    INT16U              rx_size;
    INT8U              rx_crc;
    INT16U              rx_err;
    INT8U              *rx_buf;
    u8 (*rx_indicate)(struct _usart *pUsart, INT8U rx_dat);

    INT8U               tx_state;
    INT8U              tx_idx;
    INT16U              tx_len;
		//INT8U              tx_cmd;
    INT16U              tx_size;
    INT8U              tx_crc;
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
//void    usart_tx_start           (usart_t *pUsart, message_pkt_t *pmsg);
void    usart_set_rx_indicate    (usart_t *pUsart, void (*rx_ind)(usart_t *pUsart, INT8U rx_dat));
void    usart_set_tx_complete    (usart_t *pUsart, void (*tx_com)(usart_t *pUsart));
//void    usart_printf             (const INT8U *fmt, ...);
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
