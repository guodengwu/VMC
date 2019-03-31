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
static  INT8U       usart_rx_buf        [USART_RX_BUFF_SIZE];
static  INT8U       usart_tx_buf        [USART_RX_BUFF_SIZE];
static  INT8U       usart_ringbuf       [USART_RX_BUFF_SIZE];

static  message_pkt_t    msg_pkt_usart[2];
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
              pUsart->rx_state = IG_RX_STATE_SD1;
              pUsart->rx_crc	 = rx_dat;
              pUsart->rx_idx   = 0;
              pUsart->rx_cnt   = 0;
          }
          break;
      case IG_RX_STATE_SD1:                     /* waiting for start second start delimiter (SD1)  */
          if (rx_dat == IG_PROTOCOL_RX_SD1) {
              pUsart->rx_state = IG_RX_STATE_CMD;
              pUsart->rx_crc	 += rx_dat;
          } else {
              pUsart->rx_state = IG_RX_STATE_SD0;
          }
          break;
			case IG_RX_STATE_CMD:                     /* waiting for start second start delimiter (SD1)  */
					pUsart->rx_cmd       = rx_dat;
          pUsart->rx_crc       += rx_dat;
          pUsart->rx_state     = IG_RX_STATE_LEN;
          break;
      case IG_RX_STATE_LEN:   //接收数据包长度信息
          pUsart->rx_len       = rx_dat;
					if (pUsart->rx_len == 0) {//无数据包
						  pUsart->rx_state = IG_RX_STATE_CHKSUM;/* Can not handle this size ...                    */
              //pUsart->rx_err   = IG_MSG_ERR_LENGTH;
					}else	{
							pUsart->rx_crc       += rx_dat;
							pUsart->rx_state     = IG_RX_STATE_DATA;
					}
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
              //pUsart->rx_state = IG_RX_STATE_END;
						  ret=TRUE;//数据包解析成功
							pUsart->rx_state = IG_RX_STATE_SD0;//准备接收下一帧
          } else {
              pUsart->rx_state = IG_RX_STATE_SD0;
              pUsart->rx_err   = IG_MSG_ERR_CHECKSUM;
          }
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
          //if (pUsart->tx_len > IG_END_LEN) {                /* Packet in buffer or string waiting to be send ? */
              USART_SendByte(pUsart->Usart, IG_PROTOCOL_TX_SD0);
              pUsart->tx_state  = IG_TX_STATE_SD1;
              pUsart->tx_idx    = 0;
              pUsart->tx_crc	  = IG_PROTOCOL_TX_SD0;
              //SysCommStateLed(LED_ON);
          //} else {                                /* If there is nothing to do end transmission      */
             // usart_tx_int_disable(pUsart);       /* No more data to send, disable Tx interrupts     */
              //usart_mutex_unlock(pUsart);
              //break;
          //}
          break;
      case IG_TX_STATE_SD1://机器编号
          USART_SendByte(pUsart->Usart, IG_PROTOCOL_TX_SD1);
          //if (pUsart->tx_len > IG_END_LEN) {
              pUsart->tx_state = IG_TX_STATE_CMD;
              pUsart->tx_crc	 += IG_PROTOCOL_TX_SD1;
          //}
          break;
      case IG_TX_STATE_CMD:					 //返回码
          USART_SendByte(pUsart->Usart, pUsart->tx_cmd);
          pUsart->tx_state  = IG_TX_STATE_LEN;
          pUsart->tx_crc	 += pUsart->tx_cmd;
          break;        
      case IG_TX_STATE_LEN:					 //数据内容长度
          USART_SendByte(pUsart->Usart, pUsart->tx_len);
          pUsart->tx_state  = IG_TX_STATE_DATA;
          pUsart->tx_crc	 += pUsart->tx_len;
          break;
      case IG_TX_STATE_DATA:
					if (pUsart->tx_len > 0) {
						tx_dat = pUsart->tx_buf[pUsart->tx_idx];
						USART_SendByte(pUsart->Usart, tx_dat);
						pUsart->tx_crc   += tx_dat;
						pUsart->tx_idx++;
						if (pUsart->tx_idx >= pUsart->tx_len) {   /* See if we are done sending the packet           */
								pUsart->tx_state  = IG_TX_STATE_CHKSUM;
								pUsart->tx_len    = 0;
						}
					}
          break;
      case IG_TX_STATE_CHKSUM:  //发送校验和
          USART_SendByte(pUsart->Usart, pUsart->tx_crc&0xff);
          pUsart->tx_state  = IG_TX_STATE_END;					
          break;
			case IG_TX_STATE_END:  //发送校验和
					usart_tx_int_disable(pUsart);       /* No more data to send, disable Tx interrupts     */
					pUsart->tx_state  = IG_TX_STATE_SD0;
					mutex_unlock(pUsart->lock);
					break;
      default:
          pUsart->tx_state  = IG_TX_STATE_SD0;
          pUsart->tx_err    = IG_MSG_ERR_NONE;
          //SysCommStateLed(LED_OFF);
          break;
  }
}
//数据打包，并启动发送
void usart_tx_start(usart_t *pUsart, message_pkt_t *pmsg)
{
    INT16U len;

    mutex_lock(pUsart->lock);
    pUsart->tx_cmd = pmsg->Cmd|0xA0;//返回码
    len = pmsg->dLen;
    if (len) {
        memcpy(&pUsart->tx_buf, pmsg->Data, len);
    }
    pUsart->tx_len = len;
    usart_tx_int_enable(pUsart);//enable uart tx
}
#if 0
/*******************************************************************************************************
*                                           Uart发送处理函数
********************************************************************************************************/
static void UsartSendAck (message_pkt_t *pMsg, INT8U ack)
{
    static INT8U ack_buf[2];
    u8 len=0;
    
    if(ack==ENABLE)	{//正常ack
        ack_buf[len++] = ack;
    }else	{//异常ack
        ack_buf[len++] = pMsg->Cmd;
        ack_buf[len++] = ack;
        pMsg->Cmd = IG_MSG_ACK_ERROR_CMD;
    }
    //ack_code = ack;
    pMsg->Data   = &ack_buf;
    pMsg->dLen= len;
    OSMboxPost(usart.mbox, pMsg);
}

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
    INT8U cmd;   
    
    msg_pkt_usart[0].Src = USART_MSG_RX_TASK;
    msg_pkt_usart[1].Src = USART_MSG_RX_TASK;
    //cmd = UsartRxGetINT8U(pUsart);                                 /* First byte contains command      */
		cmd = pUsart->rx_cmd;
    if (pUsart->rx_err == IG_MSG_ERR_NONE) {
        msg_pkt_usart[0].Cmd = cmd;
        msg_pkt_usart[1].Cmd = cmd;
    } else {
        pUsart->rx_err = IG_MSG_ERR_NONE;        // clear rx error
        return;
    }
		protocol_process(pUsart,msg_pkt_usart);//协议解析
}

/*******************************************************************************************************
*                                          Usart接收任务初始化
********************************************************************************************************/
void UsartRxTaskInit (void)
{
    INT8U  err;
	
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
