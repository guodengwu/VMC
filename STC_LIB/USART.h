
#ifndef __USART_H
#define __USART_H	 

#include	"config.h"

#define	COM_TX1_Lenth	30
#define	COM_RX1_Lenth	50
#define	COM_TX2_Lenth	30
#define	COM_RX2_Lenth	50
#define	COM_TX3_Lenth	30
#define	COM_RX3_Lenth	50
#define	COM_TX4_Lenth	2
#define	COM_RX4_Lenth	2

#define	USART1	1
#define	USART2	2
#define	USART3	3
#define	USART4	4

#define	UART_ShiftRight	0		//ͬ����λ���
#define	UART_8bit_BRTx	(1<<6)	//8λ����,�ɱ䲨����
#define	UART_9bit		(2<<6)	//9λ����,�̶�������
#define	UART_9bit_BRTx	(3<<6)	//9λ����,�ɱ䲨����

#define	UART1_SW_P30_P31	0
#define	UART1_SW_P36_P37	(1<<6)
#define	UART1_SW_P16_P17	(2<<6)	//����ʹ���ڲ�ʱ��
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
	u8	id;				//���ں�

	u8	TX_read;		//���Ͷ�ָ��
	u8	TX_write;		//����дָ��
	u8	B_TX_busy;		//æ��־

	u8 	RX_Cnt;			//�����ֽڼ���
	u8	RX_TimeOut;		//���ճ�ʱ
	u8	B_RX_OK;		//���տ����
} COMx_Define; 

typedef struct
{ 
	u8	UART_Mode;			//ģʽ,         UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
	u8	UART_BRT_Use;		//ʹ�ò�����,   BRT_Timer1,BRT_Timer2
	u32	UART_BaudRate;		//������,       ENABLE,DISABLE
	u8	Morecommunicate;	//���ͨѶ����, ENABLE,DISABLE
	u8	UART_RxEnable;		//�������,   ENABLE,DISABLE
	u8	BaudRateDouble;		//�����ʼӱ�, ENABLE,DISABLE
	u8	UART_Interrupt;		//�жϿ���,   ENABLE,DISABLE
	u8	UART_Polity;		//���ȼ�,     PolityLow,PolityHigh
	u8	UART_P_SW;			//�л��˿�,   UART1_SW_P30_P31,UART1_SW_P36_P37,UART1_SW_P16_P17(����ʹ���ڲ�ʱ��)
	u8	UART_RXD_TXD_Short;	//�ڲ���·RXD��TXD, ���м�, ENABLE,DISABLE

} COMx_InitDefine; 

extern	COMx_Define	COM1,COM2;
extern	u8	xdata TX1_Buffer[COM_TX1_Lenth];	//���ͻ���
extern	u8 	xdata RX1_Buffer[COM_RX1_Lenth];	//���ջ���
extern	u8	xdata TX2_Buffer[COM_TX2_Lenth];	//���ͻ���
extern	u8 	xdata RX2_Buffer[COM_RX2_Lenth];	//���ջ���

u8	USART_Configuration(u8 UARTx, COMx_InitDefine *COMx)reentrant;
void TX1_write2buff(u8 dat)reentrant;	//д�뷢�ͻ��壬ָ��+1
void TX2_write2buff(u8 dat)reentrant;	//д�뷢�ͻ��壬ָ��+1
void PrintString(u8 *puts) reentrant;
void	UART1_Init(COMx_InitDefine *COMx);
void	UART3_Init(COMx_InitDefine *COMx);
void USART_SendByte(u8 UARTx,u8 dat);
//void COMx_write2buff(COMx_Define *COMx, u8 dat);	//д�뷢�ͻ��壬ָ��+1
//void PrintString(COMx_Define *COMx, u8 *puts);

#endif

