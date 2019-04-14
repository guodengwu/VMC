/*
C********************************************************************************************************
*                                      AutoSampler System Firmware
*
*                    (c) Copyright 2010-2013 ACEA Biosciences, Inc.
*                    All rights reserved.  Protected by international copyright laws.
*
* File      :  app_usart.c
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
#define USART_GLOBALS

/*
*********************************************************************************************************
*                                          INCLUDE HEADER FILES
*********************************************************************************************************
*/
#include <stdarg.h>
#include "includes.h"
#include "app_usart.h"
#include "protocol.h"
/*
*********************************************************************************************************
*                                              LOCAL MACROS
*********************************************************************************************************
*/
#define USART_Q_SIZE    10
usart_t      usart;

OS_STK      AppUsartRxStk       [APP_TASK_USART_RX_STK_SIZE];           // Usart接收任务堆栈
OS_STK      AppUsartTxStk       [APP_TASK_USART_TX_STK_SIZE];           // Usart发送任务堆栈
static INT8U       usart_rx_buf        [USART_RX_BUFF_SIZE];
static INT8U       usart_tx_buf        [USART_TX_BUFF_SIZE];
static  INT8U       usart_ringbuf       [USART_RX_BUFF_SIZE];

static  message_pkt_t    msg_pkt_usart[2];
//static BIT32 uart_rx_sn;
static u32 uart_tx_sn;
struct uart_rx_sn_t	{
	u8 ubyte[4];
}uart_rx_sn;
/*
*********************************************************************************************************
*                                        LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static u8 message_rx_handler  (usart_t *pUsart, INT8U rx_dat);
static void message_tx_handler  (usart_t *pUsart);
static void UsartCmdParsePkt    (usart_t *pUsart);
static void AppUsartRxTask      (void *parg);
static void AppUsartTxTask      (void *parg);

/*******************************************************************************************************
*                                               Usart配置
********************************************************************************************************/
/*
void USART_SendBuf(USART_TypeDef *handle,INT8U *buf,INT8U len)
{
  u8 i;
  
  for(i=0;i<len;i++)
  {
      USART_SendData(handle, buf[i]);
      while(USART_GetFlagStatus(handle, USART_FLAG_TXE) == RESET);
  }
}*/

/*******************************************************************************************************
*
********************************************************************************************************/
static u8 message_rx_handler(usart_t *pUsart, INT8U rx_dat)
{ 
  u8 ret=FALSE;
  
  switch (pUsart->rx_state) {
      case IG_RX_STATE_SD0:                     /* waiting for start first  start delimiter (SD0)  */
          if (rx_dat == IG_PROTOCOL_RX_SD0) {
              pUsart->rx_state = IG_RX_STATE_LEN0;
              //pUsart->rx_crc	 = rx_dat;
              pUsart->rx_idx   = 0;
              pUsart->rx_cnt   = 0;
          }
          break;
			case IG_RX_STATE_LEN0:                    /* waiting for 'len' low byte                      */
          pUsart->rx_len       = rx_dat<<8;
          pUsart->rx_crc       = rx_dat;
          pUsart->rx_state     = IG_RX_STATE_LEN1;
          break;
			case IG_RX_STATE_LEN1:   //接收数据包长度信息
          pUsart->rx_len       |= rx_dat;
					if ((pUsart->rx_len < IG_CMDANDSN_LEN) || (pUsart->rx_len > pUsart->rx_size)) {
              pUsart->rx_state = IG_RX_STATE_SD0;/* Can not handle this size ...                    */
              pUsart->rx_err   = IG_MSG_ERR_LENGTH;
          } else {
              pUsart->rx_crc  += rx_dat;
              pUsart->rx_state = IG_RX_STATE_CMD;
          }
          break;
			case IG_RX_STATE_CMD:                     /* waiting for start second start delimiter (SD1)  */
					pUsart->rx_buf[pUsart->rx_cnt++] = rx_dat;
          pUsart->rx_crc       += rx_dat;
          pUsart->rx_state     = IG_RX_STATE_SN0;
          break;
			case IG_RX_STATE_SN0:                     /* waiting for start second start delimiter (SD1)  */
					pUsart->rx_buf[pUsart->rx_cnt++]       = rx_dat;
          pUsart->rx_crc       += rx_dat;
          pUsart->rx_state     = IG_RX_STATE_SN1;
          break;
			case IG_RX_STATE_SN1:                     /* waiting for start second start delimiter (SD1)  */
					pUsart->rx_buf[pUsart->rx_cnt++]       = rx_dat;
          pUsart->rx_crc       += rx_dat;
          pUsart->rx_state     = IG_RX_STATE_SN2;
					break;
			case IG_RX_STATE_SN2:                     /* waiting for start second start delimiter (SD1)  */
					pUsart->rx_buf[pUsart->rx_cnt++]       = rx_dat;
          pUsart->rx_crc       += rx_dat;
          pUsart->rx_state     = IG_RX_STATE_SN3;			
					break;
			case IG_RX_STATE_SN3:                     /* waiting for start second start delimiter (SD1)  */
					pUsart->rx_buf[pUsart->rx_cnt++]       = rx_dat;
          pUsart->rx_crc       += rx_dat;
          pUsart->rx_state     = IG_RX_STATE_DATA;		
					break;
      case IG_RX_STATE_DATA: //接收数据包内容
          if (pUsart->rx_cnt < pUsart->rx_size) {
              pUsart->rx_buf[pUsart->rx_cnt++] = rx_dat;
          }
          pUsart->rx_crc += rx_dat;
          if (pUsart->rx_len <= pUsart->rx_cnt) {//接收数据包完成
              pUsart->rx_state = IG_RX_STATE_CHKSUM;
          }
          break;
      case IG_RX_STATE_CHKSUM:                 /* waiting for checksum                            */
           if ((pUsart->rx_crc & 0xff) == rx_dat) {
							pUsart->rx_state = IG_RX_STATE_END;//
          } else {
              pUsart->rx_state = IG_RX_STATE_SD0;
              pUsart->rx_err   = IG_MSG_ERR_CHECKSUM;
          }
          break;
			case IG_RX_STATE_END:
          if (rx_dat != IG_PROTOCOL_RX_END) {   /* End delimiter ?                                 */
              pUsart->rx_err = IG_MSG_ERR_ETX_WORD;             
          }
          //usart_rx_int_disable(pUsart);
          ret=TRUE;
          pUsart->rx_state = IG_RX_STATE_SD0;
          break;
      default:
          pUsart->rx_state = IG_RX_STATE_SD0;
          pUsart->rx_err   = IG_MSG_ERR_STATE;
          break;
  }
  return ret;
}
//单字节发送数据，被uart中断调用
static void message_tx_handler(usart_t *pUsart)
{
  INT8U  tx_dat;

  switch (pUsart->tx_state) {
      case IG_TX_STATE_SD0://机器类型
          if (pUsart->tx_len > IG_END_LEN) {                /* Packet in buffer or string waiting to be send ? */
              USART_SendByte(pUsart->Usart, IG_PROTOCOL_TX_SD0);
              pUsart->tx_state  = IG_TX_STATE_LEN0;
              pUsart->tx_idx    = 0;
              //pUsart->tx_crc	  = IG_PROTOCOL_TX_SD0;
              //SysCommStateLed(LED_ON);
          } else {                                /* If there is nothing to do end transmission      */
              usart_tx_int_disable(pUsart);       /* No more data to send, disable Tx interrupts     */
              mutex_unlock(pUsart->lock);
              //break;
          }
          break;
			case IG_TX_STATE_LEN0:                    /* Include the packet length in the packet         */
					tx_dat = (pUsart->tx_len>>8)&0xff;
          USART_SendByte(pUsart->Usart, tx_dat);
          pUsart->tx_state  = IG_TX_STATE_LEN1;
          pUsart->tx_crc    = tx_dat;
          break;
			case IG_TX_STATE_LEN1:                    /* Include the packet length in the packet         */
					tx_dat = (pUsart->tx_len)&0xff;
          USART_SendByte(pUsart->Usart, tx_dat);
          pUsart->tx_state  = IG_TX_STATE_CMD;
          pUsart->tx_crc    += tx_dat;
          break;
      case IG_TX_STATE_CMD:					 //返回码
          tx_dat = pUsart->tx_buf[pUsart->tx_idx++];
					USART_SendByte(pUsart->Usart, tx_dat);
          pUsart->tx_state  = IG_TX_STATE_SN0;
          pUsart->tx_crc	 += tx_dat;
          break;        
      case IG_TX_STATE_SN0:                    /* Include the packet length in the packet         */
          tx_dat = pUsart->tx_buf[pUsart->tx_idx++];
					USART_SendByte(pUsart->Usart, tx_dat);
          pUsart->tx_state  = IG_TX_STATE_SN1;
          pUsart->tx_crc    += tx_dat;
          break;
			case IG_TX_STATE_SN1:                    /* Include the packet length in the packet         */
					tx_dat = pUsart->tx_buf[pUsart->tx_idx++];
					USART_SendByte(pUsart->Usart, tx_dat);
          pUsart->tx_state  = IG_TX_STATE_SN2;
          pUsart->tx_crc    += tx_dat;
          break;
			case IG_TX_STATE_SN2:                    /* Include the packet length in the packet         */
					tx_dat = pUsart->tx_buf[pUsart->tx_idx++];
					USART_SendByte(pUsart->Usart, tx_dat);
          pUsart->tx_state  = IG_TX_STATE_SN3;
          pUsart->tx_crc    += tx_dat;
          break;
			case IG_TX_STATE_SN3:                    /* Include the packet length in the packet         */
					tx_dat = pUsart->tx_buf[pUsart->tx_idx++];
					USART_SendByte(pUsart->Usart, tx_dat);
          pUsart->tx_state  = IG_TX_STATE_DATA;
          pUsart->tx_crc    += tx_dat;
          break;
      case IG_TX_STATE_DATA:
					if (pUsart->tx_len >= IG_CMDANDSN_LEN) {
						tx_dat = pUsart->tx_buf[pUsart->tx_idx++];
						USART_SendByte(pUsart->Usart, tx_dat);
						pUsart->tx_crc   += tx_dat;
						if (pUsart->tx_idx >= pUsart->tx_len) {   /* See if we are done sending the packet           */
								pUsart->tx_state  = IG_TX_STATE_CHKSUM;
								pUsart->tx_len    = 0;
						}
					}else	{
						usart_tx_int_disable(pUsart);       /* No more data to send, disable Tx interrupts     */
						pUsart->tx_state  = IG_TX_STATE_SD0;
						mutex_unlock(pUsart->lock);
					}
          break;
      case IG_TX_STATE_CHKSUM:  //发送校验和
          USART_SendByte(pUsart->Usart, pUsart->tx_crc&0xff);
          pUsart->tx_state  = IG_TX_STATE_END;					
          break;
			case IG_TX_STATE_END:  //发送校验和
					USART_SendByte(pUsart->Usart, IG_PROTOCOL_TX_END);
					usart_tx_int_disable(pUsart);       /* No more data to send, disable Tx interrupts     */
					pUsart->tx_state  = IG_TX_STATE_SD0;
					mutex_unlock(pUsart->lock);
					break;
      default:
          pUsart->tx_state  = IG_TX_STATE_SD0;
          pUsart->tx_err    = IG_MSG_ERR_NONE;
					mutex_unlock(pUsart->lock);
          usart_tx_int_disable(pUsart);
          break;
  }
}
//数据打包，并启动发送
void usart_tx_start(usart_t *pUsart, message_pkt_t *pmsg)
{
    INT16U len;
		u8 BCD[4];

    mutex_lock(pUsart->lock);
    pUsart->tx_buf[0] = pmsg->Cmd&(~0x10);//返回码
    len = pmsg->dLen;	
		DecToBCD(uart_tx_sn++,BCD,8);
		memcpy(pUsart->tx_buf+1, BCD, 4);
    if (len) {
        memcpy(pUsart->tx_buf + IG_CMDANDSN_LEN, pmsg->Data, len);
    }
    pUsart->tx_len = len + IG_CMDANDSN_LEN;
    usart_tx_int_enable(pUsart);//enable uart tx
}
//ACK
static void UsartSendAck (message_pkt_t *pMsg, INT8U ack)
{
		INT16U len;
		usart_t *pUsart = &usart;
	
		if(ack==MSG_SYSTEM_CMD_NONE)	{//不需要回复ACK
			return;
		}
		
		mutex_lock(pUsart->lock);
    pUsart->tx_buf[0] = pMsg->Cmd;//返回码
    len = 1;
		memcpy(pUsart->tx_buf+1, uart_rx_sn.ubyte, 4);
    if (len) {
        memcpy(pUsart->tx_buf + IG_CMDANDSN_LEN, &ack, 1);
    }
    pUsart->tx_len = len + IG_CMDANDSN_LEN;
    usart_tx_int_enable(pUsart);//enable uart tx
}
#if 0
static void UsartSendError (message_pkt_t *pMsg, INT16U err)
{
    static INT16U err_code;

    err_code     = err;
    pMsg->Cmd    = MSG_ABNORMAL_CMD;
    pMsg->Data   = &err_code;
    pMsg->dLen= 2;
    OSMboxPost(usart.mbox, pMsg);
}
#endif
/*
*********************************************************************************************************
*                                       uC/OS-View COMMAND HANDLER
*                                            (Parse Packet)
*********************************************************************************************************
*/
static  void  UsartCmdParsePkt (usart_t *pUsart)
{
    INT8U cmd,ACK,ret = MSG_FEEDBACK_DISABLE;   
    
    msg_pkt_usart[0].Src = USART_MSG_RX_TASK;
    msg_pkt_usart[1].Src = USART_MSG_RX_TASK;
    cmd = UsartRxGetINT8U(pUsart->rx_buf,&pUsart->rx_idx);           /* First byte contains command      */
		uart_rx_sn.ubyte[0] = UsartRxGetINT8U(pUsart->rx_buf,&pUsart->rx_idx);
		uart_rx_sn.ubyte[1] = UsartRxGetINT8U(pUsart->rx_buf,&pUsart->rx_idx);
		uart_rx_sn.ubyte[2] = UsartRxGetINT8U(pUsart->rx_buf,&pUsart->rx_idx);
		uart_rx_sn.ubyte[3] = UsartRxGetINT8U(pUsart->rx_buf,&pUsart->rx_idx);

    if (pUsart->rx_err == IG_MSG_ERR_NONE) {
        msg_pkt_usart[0].Cmd = cmd;
        msg_pkt_usart[1].Cmd = cmd;
    } else {
        pUsart->rx_err = IG_MSG_ERR_NONE;        // clear rx error
        return;
    }
		ret = protocol_process(pUsart,msg_pkt_usart,&ACK);//协议解析
		UsartSendAck(&msg_pkt_usart[1], ACK);
		if(ret==MSG_FEEDBACK_ENABLE)	{
			//OSMboxPost(pUsart->mbox, &msg_pkt_usart[1]);
		}
}

/*******************************************************************************************************
*                                          Usart接收任务初始化
********************************************************************************************************/
void UsartRxTaskInit (void)
{
//    INT8U  err;
	
    (void)OSTaskCreate(AppUsartRxTask,
                       (void          * ) 0,
                       (OS_STK        * )&AppUsartRxStk[0],                                 /* Set Top-Of-Stack     */
                       (INT8U           ) APP_USART_RX_TASK_PRIO);
}

/*******************************************************************************************************
*                                          Usart发送任务初始化
********************************************************************************************************/
void UsartTxTaskInit (void)
{
    //INT8U  err;

    (void)OSTaskCreate(AppUsartTxTask,
                       (void          * ) 0,
                       (OS_STK        * )&AppUsartTxStk[0],                                 /* Set Top-Of-Stack     */
                       (INT8U           ) APP_USART_TX_TASK_PRIO);
}

/*******************************************************************************************************
*                                            Usart接收初始化
********************************************************************************************************/
static void UsartInit (void)
{
		usart.Usart 			 = USART1;
    usart.lock         = OSSemCreate(1);
    usart.sem          = OSSemCreate(0);
    usart.mbox         = OSMboxCreate((void *)0);
    usart.rx_state     = IG_RX_STATE_SD0;
    usart.rx_idx       = 0;
    usart.rx_cnt       = 0;
    usart.rx_len       = 0;
    usart.rx_size      = USART_RX_BUFF_SIZE;
    usart.rx_crc       = 0;
    usart.rx_err       = IG_MSG_ERR_NONE;
    usart.rx_buf       = usart_rx_buf;
  
		usart.tx_state     = IG_TX_STATE_SD0;
    usart.tx_idx       = 0;
    usart.tx_len       = 0;
    usart.tx_size      = USART_TX_BUFF_SIZE;
    usart.tx_crc       = 0;
    usart.tx_err       = IG_MSG_ERR_NONE;
    usart.tx_buf       = usart_tx_buf;
    //usart.rx_indicate  = &message_rx_handler;
    usart.tx_complete  = &message_tx_handler;
}
extern RINGBUFF_T uart1_rxring;
//extern RINGBUFF_T uart3_rxring;
/*******************************************************************************************************
*                                              Usart接收任务
********************************************************************************************************/
static void AppUsartRxTask(void *parg)
{
    INT8U err,rxdat;
    parg = parg;
	   		
    while (DEF_True)
    {
        OSSemPend(usart.sem, 0, &err);
        if(err==OS_NO_ERR)    {
          if(RingBuffer_Pop(&uart1_rxring, (INT8U *)&rxdat))    {			
            if(message_rx_handler(&usart, rxdat))  {
              UsartCmdParsePkt(&usart);       
            }
          }
				}
    }
}

/*******************************************************************************************************
*                                              Usart发送任务
********************************************************************************************************/
static void AppUsartTxTask(void *parg)
{
    INT8U err;
    message_pkt_t *msg;
    parg = parg;

    UsartInit();

    while (DEF_True)
    {
        msg = (message_pkt_t *)OSMboxPend(usart.mbox, 0, &err);
        if(err==OS_NO_ERR)    {
            if(msg->Src==USART_MSG_RX_TASK)     {
                usart_tx_start(&usart, msg);
            }
        }
    }
}
/*
*********************************************************************************************************
*                                               No More!
**********************************************************************************************************/
