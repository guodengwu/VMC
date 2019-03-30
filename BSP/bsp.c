#include "bsp.h"
#include "tm1640.h"
////////////////////////////////////
//IO���ú���
static void	GPIO_config(void) reentrant
{
	GPIO_InitTypeDef	GPIO_InitStructure;		        //�ṹ����
	
	GPIO_InitStructure.Pin  = GPIO_Pin_1|GPIO_Pin_7;    //4 5 �ܽ�
	GPIO_InitStructure.Mode = GPIO_OUT_PP;		        //ָ��IO������������ʽ,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P3,&GPIO_InitStructure);	        //��ʼ�� P3
	
	GPIO_InitStructure.Pin  = GPIO_Pin_6|GPIO_Pin_7;    //4 5 �ܽ�
	GPIO_InitStructure.Mode = GPIO_OUT_PP;		        //ָ��IO������������ʽ,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P6,&GPIO_InitStructure);	        //��ʼ�� P3
	BEEP=0;
	LED4=0;
	TM1640_SCLK = 0;
	TM1640_DIO = 0;
}
#if 0
/*************  ����1��ʼ������ *****************/
static void	UART1_config(void) reentrant
{
	COMx_InitDefine		COMx_InitStructure;					//�ṹ����	
	COMx_InitStructure.UART_Mode      = UART_8bit_BRTx;		//ģʽ,       UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
	COMx_InitStructure.UART_BRT_Use   = BRT_Timer1;			//ʹ�ò�����,   BRT_Timer1, BRT_Timer2 (ע��: ����2�̶�ʹ��BRT_Timer2)
	COMx_InitStructure.UART_BaudRate  = 9600ul;			    //������, һ�� 110 ~ 115200
	COMx_InitStructure.UART_RxEnable  = ENABLE;				//��������,   ENABLE��DISABLE
	COMx_InitStructure.BaudRateDouble = DISABLE;			//�����ʼӱ�, ENABLE��DISABLE
	COMx_InitStructure.UART_Interrupt = ENABLE;				//�ж�����,   ENABLE��DISABLE
	COMx_InitStructure.UART_Polity    = PolityLow;			//�ж����ȼ�, PolityLow,PolityHigh
	COMx_InitStructure.UART_P_SW      = UART1_SW_P30_P31;	//�л��˿�,   UART1_SW_P30_P31,UART1_SW_P36_P37,UART1_SW_P16_P17(����ʹ���ڲ�ʱ��)
	COMx_InitStructure.UART_RXD_TXD_Short = DISABLE;		//�ڲ���·RXD��TXD, ���м�, ENABLE,DISABLE
	UART_Init(USART1, &COMx_InitStructure);		//��ʼ������1 USART1,USART2
}
#endif
/*************  ����3��ʼ������ *****************/
void	UART3_config(void)
{
	COMx_InitDefine		COMx_InitStructure;					//�ṹ����
	COMx_InitStructure.UART_Mode      = UART_8bit_BRTx;		//ģʽ,       UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
	COMx_InitStructure.UART_BaudRate  = 38400ul;			//������,     110 ~ 115200
	COMx_InitStructure.UART_RxEnable  = ENABLE;				//��������,   ENABLE��DISABLE
	COMx_InitStructure.UART_Interrupt = ENABLE;				//�ж�����,   ENABLE��DISABLE
	COMx_InitStructure.UART_Polity    = PolityLow;			//�ж����ȼ�, PolityLow,PolityHigh
	COMx_InitStructure.UART_P_SW      = UART3_SW_P00_P01;	//�л��˿�,   UART3_SW_P00_P01,UART3_SW_P50_P51
	UART3_Init(&COMx_InitStructure);
}
///////////////////////////////////////////////////////////
//������:   bsp
//����:    ����Ӳ����ʼ��
void bsp(void) reentrant
{
    GPIO_config();          //IO���ú���
    UART3_config();\
		Init_Display();
	//PrintString("test");
    /*GPIO_PWMInit(GPIO_PWM5_2,GPIO_PullUp);//LED1����Ϊ׼˫��� ���Ǹ�PWM��ص�IO���ϵ綼�Ǹ�������̬
    LED1=1;                 //�ϵ�󣬵�ȫ��	
    LED2=1;    
    LED3=1;                
    LED4=1;   */           
}
