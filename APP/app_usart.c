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
#define USE_NEW_PROTO   1
usart_t      usart;

static  OS_STK      AppUsartRxStk       [APP_TASK_USART_RX_STK_SIZE];           // Usart接收任务堆栈
static  OS_STK      AppUsartTxStk       [APP_TASK_USART_TX_STK_SIZE];           // Usart发送任务堆栈
static  INT8U       usart_rx_buf        [USART_RX_BUFF_SIZE];
static  INT8U       usart_tx_buf        [USART_RX_BUFF_SIZE];
static  INT8U       usart_ringbuf       [USART_RX_BUFF_SIZE];

static  message_pkt_t    msg_pkt_usart[2];
static  u8 g_mainboard_status;
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

/*
*********************************************************************************************************
*                                           FUNCTION DEFINITION
*********************************************************************************************************
*/
void usart_mutex_lock (usart_t *pUsart)
{
    INT8U   err;

    OSSemPend(pUsart->lock, 0, &err);
}

void usart_mutex_unlock (usart_t *pUsart)
{
    OSSemPost(pUsart->lock);
}
/*******************************************************************************************************
*                                               Usart配置
********************************************************************************************************/
void USART_SendData(usart_t *pUsart,u16 dat)
{
   // USART_SendData(handle, dat);
}
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
/*
*********************************************************************************************************
*                                     GET 8, 16 or 32-bit DATA
*********************************************************************************************************
*/

INT8U  UsartRxGetINT8U (usart_t *pUsart)
{
    return (pUsart->rx_buf[pUsart->rx_idx++]);
}

INT16U  UsartRxGetINT16U (usart_t *pUsart)
{
    INT16U  lowbyte;
    INT16U  highbyte;

    lowbyte  = UsartRxGetINT8U(pUsart);
    highbyte = UsartRxGetINT8U(pUsart);
    return ((highbyte << 8) | lowbyte);
}

INT32U  UsartRxGetINT32U (usart_t *pUsart)
{
    INT32U  highword;
    INT32U  lowword;

    lowword  = UsartRxGetINT16U(pUsart);
    highword = UsartRxGetINT16U(pUsart);
    return ((highword << 16) | lowword);
}
//u8 uart_buf_rx[20];
//u8 uart_buf_cnt=0;
/*******************************************************************************************************
*
********************************************************************************************************/
static u8 message_rx_handler(usart_t *pUsart, INT8U rx_dat)
{ 
  u8 ret=0;
  
  switch (pUsart->rx_state) {
      case IG_RX_STATE_SD0:                     /* waiting for start first  start delimiter (SD0)  */
          if (rx_dat == IG_PROTOCOL_RX_SD0) {
              pUsart->rx_state = IG_RX_STATE_SD1;
              pUsart->rx_crc	 = rx_dat;
              pUsart->rx_idx   = 0;
              pUsart->rx_cnt   = 0;
              //uart_buf_cnt = 0;
          }
          break;

      case IG_RX_STATE_SD1:                     /* waiting for start second start delimiter (SD1)  */
          if (rx_dat == IG_PROTOCOL_RX_SD1) {
              pUsart->rx_state = IG_RX_STATE_LEN0;
              pUsart->rx_crc	 += rx_dat;
          } else {
              pUsart->rx_state = IG_RX_STATE_SD0;
          }
          break;
      case IG_RX_STATE_LEN0:                    /* waiting for 'len' low byte                      */
          pUsart->rx_len       = rx_dat;
          pUsart->rx_crc       += rx_dat;
          pUsart->rx_state     = IG_RX_STATE_LEN1;
          break;
      case IG_RX_STATE_LEN1:					 /* waiting for 'len' low byte						*/
          pUsart->rx_len        |= rx_dat<<8;
          pUsart->rx_crc	+= rx_dat;
          pUsart->rx_state	= IG_RX_STATE_LEN2;
          break;
      case IG_RX_STATE_LEN2:					 /* waiting for 'len' low byte						*/
          pUsart->rx_len	|= rx_dat<<16;
          pUsart->rx_crc	+= rx_dat;
          pUsart->rx_state	= IG_RX_STATE_LEN3;
          break;

      case IG_RX_STATE_LEN3:                    /* waiting for 'len' high byte                     */
          pUsart->rx_len      |= rx_dat<<24;
          if ((pUsart->rx_len < IG_EXTENT_LEN) || (pUsart->rx_len > pUsart->rx_size)) {
              pUsart->rx_state = IG_RX_STATE_SD0;/* Can not handle this size ...                    */
              pUsart->rx_err   = IG_MSG_ERR_LENGTH;
              usart_rx_int_disable(pUsart);
              ret=TRUE;
          } else {
              pUsart->rx_crc  += rx_dat;
              pUsart->rx_state = IG_RX_STATE_DATA;
          }
          break;
      case IG_RX_STATE_DATA:                    /* waiting for data                                */
          if (pUsart->rx_cnt < pUsart->rx_size) {
              pUsart->rx_buf[pUsart->rx_cnt++] = rx_dat;
          }
          pUsart->rx_crc += rx_dat;
          if ((pUsart->rx_len - IG_END_LEN) <= pUsart->rx_cnt) {
              pUsart->rx_state = IG_RX_STATE_CHKSUM0;
          }
          break;

      case IG_RX_STATE_CHKSUM0:                 /* waiting for checksum low byte                   */
          if ((pUsart->rx_crc & 0xff) == rx_dat) {
              pUsart->rx_state = IG_RX_STATE_CHKSUM1;
          } else {
              pUsart->rx_state = IG_RX_STATE_SD0;
              pUsart->rx_err   = IG_MSG_ERR_CHECKSUM;
              usart_rx_int_disable(pUsart);
              ret=TRUE;
          }
          break;

      case IG_RX_STATE_CHKSUM1:                 /* waiting for checksum                            */
           if ((pUsart->rx_crc >>8) == rx_dat) {
              pUsart->rx_state = IG_RX_STATE_END;
          } else {
              pUsart->rx_state = IG_RX_STATE_SD0;
              pUsart->rx_err   = IG_MSG_ERR_CHECKSUM;
              usart_rx_int_disable(pUsart);
              ret=TRUE;
          }
          break;

      case IG_RX_STATE_END:
          if (rx_dat != IG_PROTOCOL_RX_END) {   /* End delimiter ?                                 */
              pUsart->rx_err = IG_MSG_ERR_ETX_WORD;             
          }
          usart_rx_int_disable(pUsart);
          ret=TRUE;
          pUsart->rx_state = IG_RX_STATE_SD0;
          break;

      default:
          pUsart->rx_state = IG_RX_STATE_SD0;
          pUsart->rx_err   = IG_MSG_ERR_STATE;
          usart_rx_int_disable(pUsart);
          ret=TRUE;
          break;
  }
  return ret;
}

/*******************************************************************************************************
*
********************************************************************************************************/
static void message_tx_handler(usart_t *pUsart)
{
  INT8U  tx_dat;

  switch (pUsart->tx_state) {
      case IG_TX_STATE_SD0:
          if (pUsart->tx_len > IG_END_LEN) {                /* Packet in buffer or string waiting to be send ? */
              USART_SendData(pUsart->Usart, IG_PROTOCOL_TX_SD0);
              pUsart->tx_state  = IG_TX_STATE_SD1;
              pUsart->tx_idx    = 0;
              pUsart->tx_crc	  = IG_PROTOCOL_TX_SD0;
              //SysCommStateLed(LED_ON);
          } else {                                /* If there is nothing to do end transmission      */
              usart_tx_int_disable(pUsart);       /* No more data to send, disable Tx interrupts     */
              usart_mutex_unlock(pUsart);
              //SysCommStateLed(LED_OFF);               // todo: set hook for tx end
              break;
          }
          break;

      case IG_TX_STATE_SD1:
          USART_SendData(pUsart->Usart, IG_PROTOCOL_TX_SD1);
          if (pUsart->tx_len > IG_END_LEN) {
              pUsart->tx_state = IG_TX_STATE_LEN0;
              pUsart->tx_crc	 += IG_PROTOCOL_TX_SD1;
          }
          break;
#if USE_NEW_PROTO==1
      case IG_TX_STATE_LEN0:                    /* Include the packet length in the packet         */
          USART_SendData(pUsart->Usart, pUsart->tx_len&0xff);
          pUsart->tx_state  = IG_TX_STATE_LEN1;
          pUsart->tx_crc    += pUsart->tx_len&0xFF;
          break;
      case IG_TX_STATE_LEN1:                    /* Include the packet length in the packet         */
          USART_SendData(pUsart->Usart, pUsart->tx_len>>8);
          pUsart->tx_state  = IG_TX_STATE_LEN2;
          pUsart->tx_crc   += pUsart->tx_len>>8;
          break;
      case IG_TX_STATE_LEN2:					 /* Include the packet length in the packet 		*/
          USART_SendData(pUsart->Usart, pUsart->tx_len>>16);
          pUsart->tx_state  = IG_TX_STATE_LEN3;
          pUsart->tx_crc	 += pUsart->tx_len>>16;
          break;        
      case IG_TX_STATE_LEN3:					 /* Include the packet length in the packet 		*/
          USART_SendData(pUsart->Usart, pUsart->tx_len>>16);
          pUsart->tx_state  = IG_TX_STATE_DATA;
          pUsart->tx_crc	 += pUsart->tx_len>>24;
          break;
#else
      case IG_TX_STATE_LEN0:					 /* Include the packet length in the packet 		*/
          USART_SendData(pUsart->Usart, pUsart->tx_len>>8);
          pUsart->tx_state  = IG_TX_STATE_LEN1;
          pUsart->tx_crc	 += pUsart->tx_len>>8;
          break;        
      case IG_TX_STATE_LEN1:					 /* Include the packet length in the packet 		*/
          USART_SendData(pUsart->Usart, pUsart->tx_len&0xff);
          pUsart->tx_state  = IG_TX_STATE_DATA;
          pUsart->tx_crc	 += pUsart->tx_len&0xff;
          break;
#endif
      case IG_TX_STATE_DATA:
          tx_dat = pUsart->tx_buf[pUsart->tx_idx];
          USART_SendData(pUsart->Usart, tx_dat);
          pUsart->tx_crc   += tx_dat;
          pUsart->tx_idx++;
          if (pUsart->tx_idx >= (pUsart->tx_len-IG_END_LEN)) {   /* See if we are done sending the packet           */
              pUsart->tx_state  = IG_TX_STATE_CHKSUM0;
              pUsart->tx_len    = 0;
          }
          break;

      case IG_TX_STATE_CHKSUM0:                 /* Epilog ... checksum & end delimiter             */
          USART_SendData(pUsart->Usart, pUsart->tx_crc&0xff);
          pUsart->tx_state  = IG_TX_STATE_CHKSUM1;
          break;

      case IG_TX_STATE_CHKSUM1:                 /* Epilog ... checksum & end delimiter             */
          USART_SendData(pUsart->Usart, pUsart->tx_crc>>8);
          pUsart->tx_state  = IG_TX_STATE_END;
          break;

      case IG_TX_STATE_END:
          USART_SendData(pUsart->Usart, IG_PROTOCOL_TX_END);
          pUsart->tx_state  = IG_TX_STATE_SD0;
          pUsart->tx_len    = 0;
          pUsart->tx_err    = IG_MSG_ERR_NONE;
          break;

      default:
          pUsart->tx_state  = IG_TX_STATE_SD0;
          pUsart->tx_err    = IG_MSG_ERR_NONE;
          //SysCommStateLed(LED_OFF);
          break;
  }
}

void usart_tx_start(usart_t *pUsart, message_pkt_t *pmsg)
{
    INT16U len;

    usart_mutex_lock(pUsart);
    pUsart->tx_buf[0] = pmsg->Cmd;
    len = pmsg->dLen;
    if (len) {
        memcpy(&pUsart->tx_buf[1], pmsg->Data, len);
    }
    pUsart->tx_len = len + IG_EXTENT_LEN;
    usart_tx_int_enable(pUsart);
}

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

/*
*********************************************************************************************************
*                                       uC/OS-View COMMAND HANDLER
*                                            (Parse Packet)
*********************************************************************************************************
*/
extern void (*ack_callback)(message_pkt_t *msg,u8 ack);
extern void (*error_callback)(message_pkt_t *msg,u16 err);

static  void  UsartCmdParsePkt (usart_t *pUsart)
{
    INT8U cmd;   
    
    msg_pkt_usart[0].Src = USART_MSG_RX_TASK;
    msg_pkt_usart[1].Src = USART_MSG_RX_TASK;
    cmd = UsartRxGetINT8U(pUsart);                                 /* First byte contains command      */

    if (pUsart->rx_err == IG_MSG_ERR_NONE) {
        msg_pkt_usart[0].Cmd = cmd;
        msg_pkt_usart[1].Cmd = cmd;
    } else {
        UsartSendError(&msg_pkt_usart[0], pUsart->rx_err);
        pUsart->rx_err = IG_MSG_ERR_NONE;        // clear rx error
        return;
    }
    ack_callback = UsartSendAck;
    error_callback = UsartSendError;
    protocol_process(pUsart,msg_pkt_usart);
}

/*******************************************************************************************************
*                                          Usart接收任务初始化
********************************************************************************************************/
void UsartRxTaskInit (void)
{
    INT8U  err;

    #if OS_TASK_CREATE_EXT_EN > 0
    #if OS_STK_GROWTH == 1
    (void)OSTaskCreateExt((void (*)(void *)) AppUsartRxTask,
                          (void          * ) 0,                      /* No arguments passed to AppUsartRxTask()     */
                          (OS_STK        * )&AppUsartRxStk[APP_TASK_USART_RX_STK_SIZE - 1], /* Set Top-Of-Stack     */
                          (INT8U           ) APP_USART_RX_TASK_PRIO, /* Set priority level                          */
                          (INT16U          ) APP_USART_RX_TASK_ID,   /* Set task id                                 */
                          (OS_STK        * )&AppUsartRxStk[0],       /* Set Bottom-Of-Stack                         */
                          (INT32U          ) APP_TASK_USART_RX_STK_SIZE,                    /* Set Stack size       */
                          (void          * ) 0,                      /* No TCB extension                            */
                          (INT16U          )(OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK)); /* Enable stack checking + clear stack  */
    #else
    (void)OSTaskCreateExt((void (*)(void *)) AppUsartRxTask,
                          (void          * ) 0,                      /* No arguments passed to AppUsartRxTask()     */
                          (OS_STK        * )&AppUsartRxStk[0],       /* Set Top-Of-Stack                            */
                          (INT8U           ) APP_USART_RX_TASK_PRIO, /* Set priority level                          */
                          (INT16U          ) APP_USART_RX_TASK_ID,   /* Set task id                                 */
                          (OS_STK        * )&AppUsartRxStk[APP_TASK_USART_RX_STK_SIZE - 1], /* Set Bottom-Of-Stack  */
                          (INT32U          ) APP_TASK_USART_RX_STK_SIZE,                    /* Set Stack size       */
                          (void          * ) 0,                      /* No TCB extension                            */
                          (INT16U          )(OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK)); /* Enable stack checking + clear stack  */

    #endif

    #else
    #if OS_STK_GROWTH == 1
    (void)OSTaskCreate((void (*)(void *)) AppUsartRxTask,
                       (void          * ) 0,
                       (OS_STK        * )&AppUsartRxStk[APP_TASK_USART_RX_STK_SIZE - 1],    /* Set Top-Of-Stack     */
                       (INT8U           ) APP_USART_RX_TASK_PRIO);
    #else
    (void)OSTaskCreate((void (*)(void *)) AppUsartRxTask,
                       (void          * ) 0,
                       (OS_STK        * )&AppUsartRxStk[0],                                 /* Set Top-Of-Stack     */
                       (INT8U           ) APP_USART_RX_TASK_PRIO);
    #endif
    #endif
     
    #if OS_TASK_NAME_SIZE > 7
    OSTaskNameSet(APP_USART_RX_TASK_PRIO, (INT8U *)"RX-Task", &err);
    #endif
}

/*******************************************************************************************************
*                                          Usart发送任务初始化
********************************************************************************************************/
void UsartTxTaskInit (void)
{
    //INT8U  err;

    #if OS_TASK_CREATE_EXT_EN > 0
    #if OS_STK_GROWTH == 1
    (void)OSTaskCreateExt((void (*)(void *)) AppUsartTxTask,
                          (void          * ) 0,                      /* No arguments passed to AppUsartTxTask()     */
                          (OS_STK        * )&AppUsartTxStk[APP_TASK_USART_TX_STK_SIZE - 1], /* Set Top-Of-Stack     */
                          (INT8U           ) APP_USART_TX_TASK_PRIO, /* Set priority level                          */
                          (INT16U          ) APP_USART_TX_TASK_ID,   /* Set task id                                 */
                          (OS_STK        * )&AppUsartTxStk[0],       /* Set Bottom-Of-Stack                         */
                          (INT32U          ) APP_TASK_USART_TX_STK_SIZE,                    /* Set Stack size       */
                          (void          * ) 0,                      /* No TCB extension                            */
                          (INT16U          )(OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK)); /* Enable stack checking + clear stack  */
    #else
    (void)OSTaskCreateExt((void (*)(void *)) AppUsartTxTask,
                          (void          * ) 0,                      /* No arguments passed to AppUsartTxTask()     */
                          (OS_STK        * )&AppUsartTxStk[0],       /* Set Top-Of-Stack                            */
                          (INT8U           ) APP_USART_TX_TASK_PRIO, /* Set priority level                          */
                          (INT16U          ) APP_USART_TX_TASK_ID,   /* Set task id                                 */
                          (OS_STK        * )&AppUsartTxStk[APP_TASK_USART_TX_STK_SIZE - 1], /* Set Bottom-Of-Stack  */
                          (INT32U          ) APP_TASK_USART_TX_STK_SIZE,                    /* Set Stack size       */
                          (void          * ) 0,                      /* No TCB extension                            */
                          (INT16U          )(OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK)); /* Enable stack checking + clear stack  */

    #endif

    #else
    #if OS_STK_GROWTH == 1
    (void)OSTaskCreate((void (*)(void *)) AppUsartTxTask,
                       (void          * ) 0,
                       (OS_STK        * )&AppUsartTxStk[APP_TASK_USART_TX_STK_SIZE - 1],    /* Set Top-Of-Stack     */
                       (INT8U           ) APP_USART_TX_TASK_PRIO);
    #else
    (void)OSTaskCreate((void (*)(void *)) AppUsartTxTask,
                       (void          * ) 0,
                       (OS_STK        * )&AppUsartTxStk[0],                                 /* Set Top-Of-Stack     */
                       (INT8U           ) APP_USART_TX_TASK_PRIO);
    #endif
    #endif

    #if OS_TASK_NAME_SIZE > 7
    OSTaskNameSet(APP_USART_TX_TASK_PRIO, (INT8U *)"TX-Task", &err);
    #endif
}

/*******************************************************************************************************
*                                            Usart接收初始化
********************************************************************************************************/
static void UsartInit (void)
{
    //INT8U err;
    
    //usart.Usart        = UART_TO_MAINBOARD;
    usart.lock         = OSSemCreate(1);
    //OSEventNameSet(usart.lock, (INT8U *)"UartLock", &err);
    usart.sem          = OSSemCreate(0);
    //OSEventNameSet(usart.sem, (INT8U *)"UartSem", &err);
    usart.mbox         = OSMboxCreate((void *)0);
    //OSEventNameSet(usart.mbox, (INT8U *)"UartMbox", &err);
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
    usart.rx_indicate  = &message_rx_handler;
    usart.tx_complete  = &message_tx_handler;
}

static u8 mainboard_detect(void)
{
//    g_mainboard_status = GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0);
    return g_mainboard_status;
}

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
//          if(RingBuffer_Pop(&uart_rxring, (INT8U *)&rxdat))    {
            if(message_rx_handler(&usart, rxdat))  {
              UsartCmdParsePkt(&usart);       
            }
          //}
//          USART_ITConfig(usart.Usart, USART_IT_RXNE, ENABLE);
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

//    RingBuffer_Init(&uart_rxring, usart_ringbuf, 1, BUFF_SIZE);
    UsartInit();
    uart3_init(115200);
    //m_status= mainboard_detect();紫-蓝，黄-红，蓝-绿

    while (DEF_TRUE)
    {
        msg = (message_pkt_t *)OSMboxPend(usart.mbox, 0, &err);
        if(err==OS_ERR_NONE)    {
          /*m_status = mainboard_detect();
          if(!m_status) {
            delay_ms(1);
            m_status = mainboard_detect();
          }
          if(m_status) */       {
            if(msg->Src==USART_MSG_RX_TASK)     {
                usart_tx_start(&usart, msg);
            }
            /*else if(msg->Src==USB_MSG_RX_TASK)  {
                usart_passthrough(&usart, msg);
            }*/
          }
        }
    }
}

/*
*********************************************************************************************************
*                                               No More!
*********************************************************************************************************
*/
