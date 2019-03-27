




#include "bsp.h"


////////////////////////////////////
//IO配置函数
static void	GPIO_config(void) reentrant
{
	GPIO_InitTypeDef	GPIO_InitStructure;		        //结构定义
	GPIO_InitStructure.Pin  = GPIO_Pin_1|GPIO_Pin_2;	//1 2 管脚
	GPIO_InitStructure.Mode = GPIO_OUT_PP;		        //指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P4,&GPIO_InitStructure);	        //初始化 P4

	GPIO_InitStructure.Pin  = GPIO_Pin_4|GPIO_Pin_5;    //4 5 管脚
	GPIO_InitStructure.Mode = GPIO_OUT_PP;		        //指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P3,&GPIO_InitStructure);	        //初始化 P3

	//初始化时 灯全灭
	LED1=1;
	LED2=1;
	LED3=1;
	LED4=1;
}



/*************  串口1初始化函数 *****************/
static void	UART_config(void) reentrant
{
	COMx_InitDefine		COMx_InitStructure;					//结构定义	
	COMx_InitStructure.UART_Mode      = UART_8bit_BRTx;		//模式,       UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
	COMx_InitStructure.UART_BRT_Use   = BRT_Timer2;			//使用波特率,   BRT_Timer1, BRT_Timer2 (注意: 串口2固定使用BRT_Timer2)
	COMx_InitStructure.UART_BaudRate  = 9600ul;			    //波特率, 一般 110 ~ 115200
	COMx_InitStructure.UART_RxEnable  = ENABLE;				//接收允许,   ENABLE或DISABLE
	COMx_InitStructure.BaudRateDouble = DISABLE;			//波特率加倍, ENABLE或DISABLE
	COMx_InitStructure.UART_Interrupt = ENABLE;				//中断允许,   ENABLE或DISABLE
	COMx_InitStructure.UART_Polity    = PolityLow;			//中断优先级, PolityLow,PolityHigh
	COMx_InitStructure.UART_P_SW      = UART1_SW_P30_P31;	//切换端口,   UART1_SW_P30_P31,UART1_SW_P36_P37,UART1_SW_P16_P17(必须使用内部时钟)
	COMx_InitStructure.UART_RXD_TXD_Short = DISABLE;		//内部短路RXD与TXD, 做中继, ENABLE,DISABLE
	USART_Configuration(USART1, &COMx_InitStructure);		//初始化串口1 USART1,USART2
}


///////////////////////////////////////////////////////////
//函数名:   bsp
//功能:    板载硬件初始化
void bsp(void) reentrant
{
    GPIO_config();          //IO配置函数
    UART_config();
    GPIO_PWMInit(GPIO_PWM5_2,GPIO_PullUp);//LED1设置为准双向口 凡是跟PWM相关的IO口上电都是高阻输入态
    LED1=1;                 //上电后，灯全灭	
    LED2=1;    
    LED3=1;                
    LED4=1;              
}
