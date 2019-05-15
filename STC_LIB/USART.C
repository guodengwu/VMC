#include "includes.h"
#include "app_usart.h"

COMx_Define	COM1,COM2,COM3,COM4;
u8	xdata TX1_Buffer[COM_TX1_Lenth];	//发送缓冲
u8 	xdata RX1_Buffer[COM_RX1_Lenth];	//接收缓冲
//u8	xdata TX2_Buffer[COM_TX2_Lenth];	//发送缓冲
//u8 	xdata RX2_Buffer[COM_RX2_Lenth];	//接收缓冲
char	xdata TX3_Buffer[COM_TX3_Lenth];	//发送缓冲
u8 	xdata RX3_Buffer[COM_RX3_Lenth];	//接收缓冲
//u8	xdata TX4_Buffer[COM_TX4_Lenth];	//发送缓冲
//u8 	xdata RX4_Buffer[COM_RX4_Lenth];	//接收缓冲
RINGBUFF_T uart1_rxring;
RINGBUFF_T uart3_rxring;

void	UART1_Init(COMx_InitDefine *COMx)
{
	USART_Configuration(USART1, COMx);		//初始化串口2 USART1,USART2
	RingBuffer_Init(&uart1_rxring, RX1_Buffer, 1, COM_RX1_Lenth);
	//PrintString2("STC15F2K60S2 UART2 Test Prgramme!\r\n");	//SUART2发送一个字符串
}
void	UART3_Init(COMx_InitDefine *COMx)
{
	USART_Configuration(USART3, COMx);		//初始化串口2 USART1,USART2
	RingBuffer_Init(&uart3_rxring, RX3_Buffer, 1, COM_RX3_Lenth);
	//PrintString2("STC15F2K60S2 UART2 Test Prgramme!\r\n");	//SUART2发送一个字符串
}
u8 USART_Configuration(u8 UARTx, COMx_InitDefine *COMx) reentrant
{
	u8	i;
	u32	j;
	
	if(UARTx == USART1)
	{
		COM1.id = 1;
		COM1.TX_read    = 0;
		COM1.TX_write   = 0;
		COM1.B_TX_busy  = 0;
		COM1.RX_Cnt     = 0;
		COM1.RX_TimeOut = 0;
		COM1.B_RX_OK    = 0;
		for(i=0; i<COM_TX1_Lenth; i++)	TX1_Buffer[i] = 0;
		for(i=0; i<COM_RX1_Lenth; i++)	RX1_Buffer[i] = 0;

		if(COMx->UART_Mode > UART_9bit_BRTx)	return 2;	//模式错误
		if(COMx->UART_Polity == PolityHigh)		PS = 1;	//高优先级中断
		else									PS = 0;	//低优先级中断
		SCON = (SCON & 0x3f) | COMx->UART_Mode;
		if((COMx->UART_Mode == UART_9bit_BRTx) ||(COMx->UART_Mode == UART_8bit_BRTx))	//可变波特率
		{
			j = (MAIN_Fosc / 4) / COMx->UART_BaudRate;	//按1T计算
			if(j >= 65536UL)	return 2;	//错误
			j = 65536UL - j;
			if(COMx->UART_BRT_Use == BRT_Timer1)
			{
				TR1 = 0;
				AUXR &= ~0x01;		//S1 BRT Use Timer1;
				TMOD &= ~(1<<6);	//Timer1 set As Timer
				TMOD &= ~0x30;		//Timer1_16bitAutoReload;
				AUXR |=  (1<<6);	//Timer1 set as 1T mode
				TH1 = (u8)(j>>8);
				TL1 = (u8)j;
				ET1 = 0;	//禁止中断
				TMOD &= ~0x40;	//定时
				INT_CLKO &= ~0x02;	//不输出时钟
				TR1  = 1;
			}
			else if(COMx->UART_BRT_Use == BRT_Timer2)
			{
				AUXR &= ~(1<<4);	//Timer stop
				AUXR |= 0x01;		//S1 BRT Use Timer2;
				AUXR &= ~(1<<3);	//Timer2 set As Timer
				AUXR |=  (1<<2);	//Timer2 set as 1T mode
				TH2 = (u8)(j>>8);
				TL2 = (u8)j;
				IE2  &= ~(1<<2);	//禁止中断
				AUXR &= ~(1<<3);	//定时
				AUXR |=  (1<<4);	//Timer run enable
			}
			else return 2;	//错误
		}
		else if(COMx->UART_Mode == UART_ShiftRight)
		{
			if(COMx->BaudRateDouble == ENABLE)	AUXR |=  (1<<5);	//固定波特率SysClk/2
			else								AUXR &= ~(1<<5);	//固定波特率SysClk/12
		}
		else if(COMx->UART_Mode == UART_9bit)	//固定波特率SysClk*2^SMOD/64
		{
			if(COMx->BaudRateDouble == ENABLE)	PCON |=  (1<<7);	//固定波特率SysClk/32
			else								PCON &= ~(1<<7);	//固定波特率SysClk/64
		}
		if(COMx->UART_Interrupt == ENABLE)	ES = 1;	//允许中断
		else								ES = 0;	//禁止中断
		if(COMx->UART_RxEnable == ENABLE)	REN = 1;	//允许接收
		else								REN = 0;	//禁止接收
		P_SW1 = (P_SW1 & 0x3f) | (COMx->UART_P_SW & 0xc0);	//切换IO
		if(COMx->UART_RXD_TXD_Short == ENABLE)	PCON2 |=  (1<<4);	//内部短路RXD与TXD, 做中继, ENABLE,DISABLE
		else									PCON2 &= ~(1<<4);
		return	0;
	}

	/*else if(UARTx == USART2)
	{
		COM2.id = 2;
		COM2.TX_read    = 0;
		COM2.TX_write   = 0;
		COM2.B_TX_busy  = 0;
		COM2.RX_Cnt     = 0;
		COM2.RX_TimeOut = 0;
		COM2.B_RX_OK    = 0;
		for(i=0; i<COM_TX2_Lenth; i++)	TX2_Buffer[i] = 0;
		for(i=0; i<COM_RX2_Lenth; i++)	RX2_Buffer[i] = 0;

		if((COMx->UART_Mode == UART_9bit_BRTx) ||(COMx->UART_Mode == UART_8bit_BRTx))	//可变波特率
		{
			if(COMx->UART_Polity == PolityHigh)		IP2 |=  1;	//高优先级中断
			else									IP2 &= ~1;	//低优先级中断
			if(COMx->UART_Mode == UART_9bit_BRTx)	S2CON |=  (1<<7);	//9bit
			else									S2CON &= ~(1<<7);	//8bit
			j = (MAIN_Fosc / 4) / COMx->UART_BaudRate;	//按1T计算
			if(j >= 65536UL)	return 2;	//错误
			j = 65536UL - j;
			AUXR &= ~(1<<4);	//Timer stop
			AUXR &= ~(1<<3);	//Timer2 set As Timer
			AUXR |=  (1<<2);	//Timer2 set as 1T mode
			TH2 = (u8)(j>>8);
			TL2 = (u8)j;
			IE2  &= ~(1<<2);	//禁止中断
			AUXR |=  (1<<4);	//Timer run enable
		}
		else	return 2;	//模式错误
		if(COMx->UART_Interrupt == ENABLE)	IE2   |=  1;		//允许中断
		else								IE2   &= ~1;		//禁止中断
		if(COMx->UART_RxEnable == ENABLE)	S2CON |=  (1<<4);	//允许接收
		else								S2CON &= ~(1<<4);	//禁止接收
		P_SW2 = (P_SW2 & ~1) | (COMx->UART_P_SW & 0x01);	//切换IO		
	}*/
	else if(UARTx == USART3)
	{
		COM3.id = 3;
		COM3.TX_read    = 0;
		COM3.TX_write   = 0;
		COM3.B_TX_busy  = 0;
		COM3.RX_Cnt     = 0;
		COM3.RX_TimeOut = 0;
		COM3.B_RX_OK    = 0;
		for(i=0; i<COM_TX3_Lenth; i++)	TX3_Buffer[i] = 0;
		for(i=0; i<COM_RX3_Lenth; i++)	RX3_Buffer[i] = 0;

		if((COMx->UART_Mode == UART_9bit_BRTx) ||(COMx->UART_Mode == UART_8bit_BRTx))	//可变波特率
		{
			if(COMx->UART_Polity == PolityHigh)		IP2 |=  1;	//高优先级中断
			else									IP2 &= ~1;	//低优先级中断
			if(COMx->UART_Mode == UART_9bit_BRTx)	S3CON |=  (1<<7);	//9bit
			else									S3CON &= ~(1<<7);	//8bit
			j = (MAIN_Fosc / 4) / COMx->UART_BaudRate;	//按1T计算
			if(j >= 65536UL)	return 2;	//错误
			j = 65536UL - j;			
			T4T3M &= ~(1<<3);	//Timer stop
			T4T3M &= ~(1<<2);	//Timer3 set As Timer
			T4T3M |=  (1<<1);	//Timer3 set as 1T mode		
			S3CON |=  (1<<6);//S3ST3 1: Timer3 set as timer			
			TH3 = (u8)(j>>8);
			TL3 = (u8)j;
			IE2  &= ~(1<<3);	//禁止中断
			T4T3M |= (1<<3);	//Timer run enable
		}
		else	return 2;	//模式错误
		if(COMx->UART_Interrupt == ENABLE)	IE2   |=  (1<<3);		//允许中断
		else								IE2   &= ~(1<<3);		//禁止中断
		if(COMx->UART_RxEnable == ENABLE)	S3CON |=  (1<<4);	//允许接收
		else								S3CON &= ~(1<<4);	//禁止接收
		P_SW2 = (P_SW2 & ~(1<<1)) | COMx->UART_P_SW;	//切换IO
	}
	/*else if(UARTx == USART4)
	{
		COM4.id = 4;
		COM4.TX_read    = 0;
		COM4.TX_write   = 0;
		COM4.B_TX_busy  = 0;
		COM4.RX_Cnt     = 0;
		COM4.RX_TimeOut = 0;
		COM4.B_RX_OK    = 0;
		for(i=0; i<COM_TX4_Lenth; i++)	TX4_Buffer[i] = 0;
		for(i=0; i<COM_RX4_Lenth; i++)	RX4_Buffer[i] = 0;

		if((COMx->UART_Mode == UART_9bit_BRTx) ||(COMx->UART_Mode == UART_8bit_BRTx))	//可变波特率
		{
			if(COMx->UART_Polity == PolityHigh)		IP2 |=  1;	//高优先级中断
			else									IP2 &= ~1;	//低优先级中断
			if(COMx->UART_Mode == UART_9bit_BRTx)	S4CON |=  (1<<7);	//9bit
			else									S4CON &= ~(1<<7);	//8bit
			j = (MAIN_Fosc / 4) / COMx->UART_BaudRate;	//按1T计算
			if(j >= 65536UL)	return 2;	//错误
			j = 65536UL - j;
			AUXR &= ~(1<<4);	//Timer stop
			AUXR &= ~(1<<3);	//Timer2 set As Timer
			AUXR |=  (1<<2);	//Timer2 set as 1T mode
			TH4 = (u8)(j>>8);
			TL4 = (u8)j;
			IE2  &= ~(1<<4);	//禁止中断
			AUXR |=  (1<<4);	//Timer run enable
		}
		else	return 2;	//模式错误
		if(COMx->UART_Interrupt == ENABLE)	IE2   |=  (1<<4);		//允许中断
		else								IE2   &= ~(1<<4);		//禁止中断
		if(COMx->UART_RxEnable == ENABLE)	S4CON |=  (1<<4);	//允许接收
		else								S4CON &= ~(1<<4);	//禁止接收
		P_SW2 = (P_SW2 & ~(1<<2)) | COMx->UART_P_SW;	//切换IO		
	}*/
	return 0;
}
#if 0
/*************** 装载串口发送缓冲 *******************************/
void TX1_write2buff(u8 dat)	reentrant//写入发送缓冲，指针+1
{
	TX1_Buffer[COM1.TX_write] = dat;	//装发送缓冲
	if(++COM1.TX_write >= COM_TX1_Lenth)	COM1.TX_write = 0;

	if(COM1.B_TX_busy == 0)		//空闲
	{  
		COM1.B_TX_busy = 1;		//标志忙
		TI = 1;					//触发发送中断
	}
}

void TX2_write2buff(u8 dat) reentrant	//写入发送缓冲，指针+1
{
	TX2_Buffer[COM2.TX_write] = dat;	//装发送缓冲
	if(++COM2.TX_write >= COM_TX2_Lenth)	COM2.TX_write = 0;

	if(COM2.B_TX_busy == 0)		//空闲
	{  
		COM2.B_TX_busy = 1;		//标志忙
		SET_TI2();				//触发发送中断
	}
}
#endif
void TX3_write2buff(char dat) reentrant	//写入发送缓冲，指针+1
{
	TX3_Buffer[COM3.TX_write] = dat;	//装发送缓冲
	if(++COM3.TX_write >= COM_TX3_Lenth)	COM3.TX_write = 0;

	if(COM3.B_TX_busy == 0)		//空闲
	{  
		COM3.B_TX_busy = 1;		//标志忙
		SET_TI3();				//触发发送中断
	}
}

/*void PrintString2(u8 *puts) reentrant
{
    for (; *puts != 0;	puts++)  TX2_write2buff(*puts); 	//遇到停止符0结束
}*/

/*
void COMx_write2buff(COMx_Define *COMx, u8 dat)	//写入发送缓冲，指针+1
{
	if(COMx->id == 1)	TX1_write2buff(dat);
	if(COMx->id == 2)	TX2_write2buff(dat);
}

void PrintString(COMx_Define *COMx, u8 *puts)
{
    for (; *puts != 0;	puts++)  COMx_write2buff(COMx,*puts); 	//遇到停止符0结束
}
*/
//中断发送数据
void USART_SendByte(u8 UARTx,u8 dat)
{
   if(UARTx==USART1)	{
			SBUF = dat;
			//TI = 1;					//触发发送中断
	 }else    if(UARTx==USART2)	{
			S2BUF = dat;
			//SET_TI2();				//触发发送中断
	 }else    if(UARTx==USART3)	{
			S3BUF = dat;
			//SET_TI3();				//触发发送中断
	 }else    if(UARTx==USART4)	{
			S4BUF = dat;
			//SET_TI4();				//触发发送中断
	 }
}
/********************* UART1中断函数************************/
void UART1_int (void) interrupt UART1_VECTOR
{
	u8 rxdat;
	
  OSIntEnter();					// Must be called first at every hardware interrupt entry point 
	if(RI)
	{
		RI = 0;
		rxdat = SBUF;
		RingBuffer_Insert(&uart1_rxring, &rxdat);
		OSSemPost(usart.sem);
	}

	if(TI)
	{
		TI = 0;
		if (usart.tx_complete != NULL) usart.tx_complete(&usart);
		//BEEP=1;
			/*	if(COM1.TX_read != COM1.TX_write)
		{
		 	SBUF = TX1_Buffer[COM1.TX_read];
			if(++COM1.TX_read >= COM_TX1_Lenth)		COM1.TX_read = 0;
		}
		else	COM1.B_TX_busy = 0;*/
	}
  OSIntExit();					// Must be called finally at every hardware interupt exit point 
}
#if 0
/********************* UART2中断函数************************/
void UART2_int (void) interrupt UART2_VECTOR
{
    OSIntExit();					// Must be called finally at every hardware interupt exit point 
	if(RI2)
	{
		CLR_RI2();
		if(COM2.B_RX_OK == 0)
		{
			if(COM2.RX_Cnt >= COM_RX2_Lenth)	COM2.RX_Cnt = 0;
			RX2_Buffer[COM2.RX_Cnt++] = S2BUF;
			COM2.RX_TimeOut = TimeOutSet2;
		}
	}

	if(TI2)
	{
		CLR_TI2();
		if(COM2.TX_read != COM2.TX_write)
		{
		 	S2BUF = TX2_Buffer[COM2.TX_read];
			if(++COM2.TX_read >= COM_TX2_Lenth)		COM2.TX_read = 0;
		}
		else	COM2.B_TX_busy = 0;
	}
  OSIntExit();					// Must be called finally at every hardware interupt exit point 
}
#endif
#if 0
/********************* UART3中断函数************************/
void UART3_int (void) interrupt UART3_VECTOR
{
	u8 rxdat;
	
  OSIntExit();					// Must be called finally at every hardware interupt exit point 
	if(RI3)//接收中断有效
	{
			CLR_RI3();
			rxdat = S3BUF;
			RingBuffer_Insert(&uart3_rxring, &rxdat);
			OSSemPost(usart.sem);
	}

	if(TI3)//发送中断有效
	{
		CLR_TI3();/* Clear the USART1 transmit interrupt                  */
		if (usart.tx_complete != NULL) usart.tx_complete(&usart);
	}
  OSIntExit();					// Must be called finally at every hardware interupt exit point 
}
#else
/********************* UART3中断函数************************/
void UART3_int (void) interrupt UART3_VECTOR
{
//	u8 rxdat;
	
  OSIntExit();					// Must be called finally at every hardware interupt exit point 
	if(RI3)//接收中断有效
	{
		CLR_RI3();
		if(COM3.B_RX_OK == 0)
		{
			if(COM3.RX_Cnt >= COM_RX3_Lenth)	COM3.RX_Cnt = 0;
			RX3_Buffer[COM3.RX_Cnt++] = S3BUF;
		}
	}

	if(TI3)//发送中断有效
	{
		CLR_TI3();/* Clear the USART1 transmit interrupt                  */
		if(COM3.TX_read != COM3.TX_write)
		{
		 	S3BUF = TX3_Buffer[COM3.TX_read];
			if(++COM3.TX_read >= COM_TX3_Lenth)		COM3.TX_read = 0;
		}
		else	COM3.B_TX_busy = 0;
		//if (usart.tx_complete != NULL) usart.tx_complete(&usart);
	}
  OSIntExit();					// Must be called finally at every hardware interupt exit point 
}
#endif
