




#include "bsp.h"


////////////////////////////////////
//IO���ú���
static void	GPIO_config(void) reentrant
{
	GPIO_InitTypeDef	GPIO_InitStructure;		        //�ṹ����
	GPIO_InitStructure.Pin  = GPIO_Pin_1|GPIO_Pin_2;	//1 2 �ܽ�
	GPIO_InitStructure.Mode = GPIO_OUT_PP;		        //ָ��IO������������ʽ,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P4,&GPIO_InitStructure);	        //��ʼ�� P4

	GPIO_InitStructure.Pin  = GPIO_Pin_4|GPIO_Pin_5;    //4 5 �ܽ�
	GPIO_InitStructure.Mode = GPIO_OUT_PP;		        //ָ��IO������������ʽ,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P3,&GPIO_InitStructure);	        //��ʼ�� P3

	//��ʼ��ʱ ��ȫ��
	LED1=1;
	LED2=1;
	LED3=1;
	LED4=1;
}



/*************  ����1��ʼ������ *****************/
static void	UART_config(void) reentrant
{
	COMx_InitDefine		COMx_InitStructure;					//�ṹ����	
	COMx_InitStructure.UART_Mode      = UART_8bit_BRTx;		//ģʽ,       UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
	COMx_InitStructure.UART_BRT_Use   = BRT_Timer2;			//ʹ�ò�����,   BRT_Timer1, BRT_Timer2 (ע��: ����2�̶�ʹ��BRT_Timer2)
	COMx_InitStructure.UART_BaudRate  = 9600ul;			    //������, һ�� 110 ~ 115200
	COMx_InitStructure.UART_RxEnable  = ENABLE;				//��������,   ENABLE��DISABLE
	COMx_InitStructure.BaudRateDouble = DISABLE;			//�����ʼӱ�, ENABLE��DISABLE
	COMx_InitStructure.UART_Interrupt = ENABLE;				//�ж�����,   ENABLE��DISABLE
	COMx_InitStructure.UART_Polity    = PolityLow;			//�ж����ȼ�, PolityLow,PolityHigh
	COMx_InitStructure.UART_P_SW      = UART1_SW_P30_P31;	//�л��˿�,   UART1_SW_P30_P31,UART1_SW_P36_P37,UART1_SW_P16_P17(����ʹ���ڲ�ʱ��)
	COMx_InitStructure.UART_RXD_TXD_Short = DISABLE;		//�ڲ���·RXD��TXD, ���м�, ENABLE,DISABLE
	USART_Configuration(USART1, &COMx_InitStructure);		//��ʼ������1 USART1,USART2
}


///////////////////////////////////////////////////////////
//������:   bsp
//����:    ����Ӳ����ʼ��
void bsp(void) reentrant
{
    GPIO_config();          //IO���ú���
    UART_config();
    GPIO_PWMInit(GPIO_PWM5_2,GPIO_PullUp);//LED1����Ϊ׼˫��� ���Ǹ�PWM��ص�IO���ϵ綼�Ǹ�������̬
    LED1=1;                 //�ϵ�󣬵�ȫ��	
    LED2=1;    
    LED3=1;                
    LED4=1;              
}
