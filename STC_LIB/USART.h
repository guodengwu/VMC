
#ifndef __USART_H
#define __USART_H	 

#include	"config.h"

#define	COM_TX1_Lenth	200
#define	COM_RX1_Lenth	200
#define	COM_TX2_Lenth	30
#define	COM_RX2_Lenth	30
#define	COM_TX3_Lenth	60
#define	COM_RX3_Lenth	50
#define	COM_TX4_Lenth	2
#define	COM_RX4_Lenth	2

#define	USART1	1
#define	USART2	2
#define	USART3	3
#define	USART4	4

#define	UART_ShiftRight	0		//同步移位输出
#define	UART_8bit_BRTx	(1<<6)	//8位数据,可变波特率
#define	UART_9bit		(2<<6)	//9位数据,固定波特率
#define	UART_9bit_BRTx	(3<<6)	//9位数据,可变波特率

#define	UART1_SW_P30_P31	0
#define	UART1_SW_P36_P37	(1<<6)
#define	UART1_SW_P16_P17	(2<<6)	//必须使用内部时钟
#define	UART2_SW_P10_P11	0//P1.0/RxD2,P1.1/TxD2
#define	UART2_SW_P46_P47	(1<<0)//P4.6/RxD2_2,P4.7/TxD2_2
#define	UART3_SW_P00_P01	0//P0.0/RxD3,P0.1/TxD3
#define	UART3_SW_P50_P51	(1<<1)//P5.0/RxD3_2,P5.1/TxD3_2
#define	UART4_SW_P02_P03	0//P0.2/RxD4,P0.3/TxD4
#define	UART4_SW_P52_P53	(1<<2)//P5.2/RxD4_2,P5.3/TxD4_2

#define	TimeOutSet1		1
#define	TimeOutSet2		1

#define	BRT_Timer1	1
#define	BRT_Timer2	2

typedef struct
{ 
	u8	id;				//串口号

	u8	TX_read;		//发送读指针
	u8	TX_write;		//发送写指针
	u8	B_TX_busy;		//忙标志

	u8 	RX_Cnt;			//接收字节计数
	u8	RX_TimeOut;		//接收超时
	u8	B_RX_OK;		//接收块完成
} COMx_Define; 

typedef struct
{ 
	u8	UART_Mode;			//模式,         UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
	u8	UART_BRT_Use;		//使用波特率,   BRT_Timer1,BRT_Timer2
	u32	UART_BaudRate;		//波特率,       ENABLE,DISABLE
	u8	Morecommunicate;	//多机通讯允许, ENABLE,DISABLE
	u8	UART_RxEnable;		//允许接收,   ENABLE,DISABLE
	u8	BaudRateDouble;		//波特率加倍, ENABLE,DISABLE
	u8	UART_Interrupt;		//中断控制,   ENABLE,DISABLE
	u8	UART_Polity;		//优先级,     PolityLow,PolityHigh
	u8	UART_P_SW;			//切换端口,   UART1_SW_P30_P31,UART1_SW_P36_P37,UART1_SW_P16_P17(必须使用内部时钟)
	u8	UART_RXD_TXD_Short;	//内部短路RXD与TXD, 做中继, ENABLE,DISABLE

} COMx_InitDefine; 

extern	COMx_Define	COM1,COM2;
extern	u8	xdata TX1_Buffer[COM_TX1_Lenth];	//发送缓冲
extern	u8 	xdata RX1_Buffer[COM_RX1_Lenth];	//接收缓冲
extern	u8	xdata TX2_Buffer[COM_TX2_Lenth];	//发送缓冲
extern	u8 	xdata RX2_Buffer[COM_RX2_Lenth];	//接收缓冲

u8	USART_Configuration(u8 UARTx, COMx_InitDefine *COMx)reentrant;
void TX1_write2buff(u8 dat)reentrant;	//写入发送缓冲，指针+1
void TX2_write2buff(u8 dat)reentrant;	//写入发送缓冲，指针+1
void PrintString(u8 *puts) reentrant;
void	UART1_Init(COMx_InitDefine *COMx);
void	UART3_Init(COMx_InitDefine *COMx);
void USART_SendByte(u8 UARTx,u8 dat);
//void COMx_write2buff(COMx_Define *COMx, u8 dat);	//写入发送缓冲，指针+1
//void PrintString(COMx_Define *COMx, u8 *puts);

#endif

