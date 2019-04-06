
/*
����˵��
���ļ�ΪSTC15xxxϵ�еĶ˿ڳ�ʼ������,�û��������Բ��޸��������.
cocoguojia �޸Ĺ�
*/

#include	"GPIO.h"


//========================================================================
// ����: u8	GPIO_Inilize(u8 GPIO, GPIO_InitTypeDef *GPIOx)
// ����: ��ʼ��IO��.
// ����: GPIOx: �ṹ����,��ο�gpio.h��Ķ���.
// ����: �ɹ�����0, �ղ�������1,���󷵻�2.
// �汾: V1.0, 2012-10-22
//========================================================================
u8	GPIO_Inilize(u8 GPIO, GPIO_InitTypeDef *GPIOx) reentrant
{
	if(GPIO > GPIO_P7)				return 1;	//�ղ���
	if(GPIOx->Mode > GPIO_OUT_PP)	return 2;	//����

	if(GPIO == GPIO_P0)
	{
		if(GPIOx->Mode == GPIO_PullUp)		P0M1 &= ~GPIOx->Pin,	P0M0 &= ~GPIOx->Pin;	 //����׼˫���
		if(GPIOx->Mode == GPIO_HighZ)		P0M1 |=  GPIOx->Pin,	P0M0 &= ~GPIOx->Pin;	 //��������
		if(GPIOx->Mode == GPIO_OUT_OD)		P0M1 |=  GPIOx->Pin,	P0M0 |=  GPIOx->Pin;	 //��©���
		if(GPIOx->Mode == GPIO_OUT_PP)		P0M1 &= ~GPIOx->Pin,	P0M0 |=  GPIOx->Pin;	 //�������
	}
	if(GPIO == GPIO_P1)
	{
		if(GPIOx->Mode == GPIO_PullUp)		P1M1 &= ~GPIOx->Pin,	P1M0 &= ~GPIOx->Pin;	 //����׼˫���
		if(GPIOx->Mode == GPIO_HighZ)		P1M1 |=  GPIOx->Pin,	P1M0 &= ~GPIOx->Pin;	 //��������
		if(GPIOx->Mode == GPIO_OUT_OD)		P1M1 |=  GPIOx->Pin,	P1M0 |=  GPIOx->Pin;	 //��©���
		if(GPIOx->Mode == GPIO_OUT_PP)		P1M1 &= ~GPIOx->Pin,	P1M0 |=  GPIOx->Pin;	 //�������
	}
	if(GPIO == GPIO_P2)
	{
		if(GPIOx->Mode == GPIO_PullUp)		P2M1 &= ~GPIOx->Pin,	P2M0 &= ~GPIOx->Pin;	 //����׼˫���
		if(GPIOx->Mode == GPIO_HighZ)		P2M1 |=  GPIOx->Pin,	P2M0 &= ~GPIOx->Pin;	 //��������
		if(GPIOx->Mode == GPIO_OUT_OD)		P2M1 |=  GPIOx->Pin,	P2M0 |=  GPIOx->Pin;	 //��©���
		if(GPIOx->Mode == GPIO_OUT_PP)		P2M1 &= ~GPIOx->Pin,	P2M0 |=  GPIOx->Pin;	 //�������
	}
	if(GPIO == GPIO_P3)
	{
		if(GPIOx->Mode == GPIO_PullUp)		P3M1 &= ~GPIOx->Pin,	P3M0 &= ~GPIOx->Pin;	 //����׼˫���
		if(GPIOx->Mode == GPIO_HighZ)		P3M1 |=  GPIOx->Pin,	P3M0 &= ~GPIOx->Pin;	 //��������
		if(GPIOx->Mode == GPIO_OUT_OD)		P3M1 |=  GPIOx->Pin,	P3M0 |=  GPIOx->Pin;	 //��©���
		if(GPIOx->Mode == GPIO_OUT_PP)		P3M1 &= ~GPIOx->Pin,	P3M0 |=  GPIOx->Pin;	 //�������
	}
	if(GPIO == GPIO_P4)
	{
		if(GPIOx->Mode == GPIO_PullUp)		P4M1 &= ~GPIOx->Pin,	P4M0 &= ~GPIOx->Pin;	 //����׼˫���
		if(GPIOx->Mode == GPIO_HighZ)		P4M1 |=  GPIOx->Pin,	P4M0 &= ~GPIOx->Pin;	 //��������
		if(GPIOx->Mode == GPIO_OUT_OD)		P4M1 |=  GPIOx->Pin,	P4M0 |=  GPIOx->Pin;	 //��©���
		if(GPIOx->Mode == GPIO_OUT_PP)		P4M1 &= ~GPIOx->Pin,	P4M0 |=  GPIOx->Pin;	 //�������
	}
	if(GPIO == GPIO_P5)
	{
		if(GPIOx->Mode == GPIO_PullUp)		P5M1 &= ~GPIOx->Pin,	P5M0 &= ~GPIOx->Pin;	 //����׼˫���
		if(GPIOx->Mode == GPIO_HighZ)		P5M1 |=  GPIOx->Pin,	P5M0 &= ~GPIOx->Pin;	 //��������
		if(GPIOx->Mode == GPIO_OUT_OD)		P5M1 |=  GPIOx->Pin,	P5M0 |=  GPIOx->Pin;	 //��©���
		if(GPIOx->Mode == GPIO_OUT_PP)		P5M1 &= ~GPIOx->Pin,	P5M0 |=  GPIOx->Pin;	 //�������
	}
	if(GPIO == GPIO_P6)
	{
		if(GPIOx->Mode == GPIO_PullUp)		P6M1 &= ~GPIOx->Pin,	P6M0 &= ~GPIOx->Pin;	 //����׼˫���
		if(GPIOx->Mode == GPIO_HighZ)		P6M1 |=  GPIOx->Pin,	P6M0 &= ~GPIOx->Pin;	 //��������
		if(GPIOx->Mode == GPIO_OUT_OD)		P6M1 |=  GPIOx->Pin,	P6M0 |=  GPIOx->Pin;	 //��©���
		if(GPIOx->Mode == GPIO_OUT_PP)		P6M1 &= ~GPIOx->Pin,	P6M0 |=  GPIOx->Pin;	 //�������
	}
	if(GPIO == GPIO_P7)
	{
		if(GPIOx->Mode == GPIO_PullUp)		P7M1 &= ~GPIOx->Pin,	P7M0 &= ~GPIOx->Pin;	 //����׼˫���
		if(GPIOx->Mode == GPIO_HighZ)		P7M1 |=  GPIOx->Pin,	P7M0 &= ~GPIOx->Pin;	 //��������
		if(GPIOx->Mode == GPIO_OUT_OD)		P7M1 |=  GPIOx->Pin,	P7M0 |=  GPIOx->Pin;	 //��©���
		if(GPIOx->Mode == GPIO_OUT_PP)		P7M1 &= ~GPIOx->Pin,	P7M0 |=  GPIOx->Pin;	 //�������
	}
	return 0;	//�ɹ�
}

//////////////////////////////////////////////////////////////
//���IAP15W4KxxS4ϵ�мӵ� cocoguojia
//��ʼ��PWM�йص�IO��
//��Ϊ������PWM�йص�IO���ϵ���Ǹ�������̬
//u8 GPIO_PWM ��H�ļ�
//u8 PuHzOdPp=GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
u8 GPIO_PWMInit(u8 GPIO_PWM,u8 PuHzOdPp) reentrant
{
    GPIO_InitTypeDef	GPIO_InitStructure;		        //�ṹ����

    switch(GPIO_PWM)
    {
        case GPIO_PWM2:
        GPIO_InitStructure.Pin  = GPIO_Pin_7;
        GPIO_InitStructure.Mode = PuHzOdPp;		            //ָ��IO������������ʽ,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
        GPIO_Inilize(GPIO_P3,&GPIO_InitStructure);	        
        break;
        case GPIO_PWM3:
        GPIO_InitStructure.Pin  = GPIO_Pin_1;
        GPIO_InitStructure.Mode = PuHzOdPp;		            //ָ��IO������������ʽ,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
        GPIO_Inilize(GPIO_P2,&GPIO_InitStructure);	        
        break;
        case GPIO_PWM4:
        GPIO_InitStructure.Pin  = GPIO_Pin_2;
        GPIO_InitStructure.Mode = PuHzOdPp;		            //ָ��IO������������ʽ,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
        GPIO_Inilize(GPIO_P2,&GPIO_InitStructure);	        
        break;
        case GPIO_PWM5:
        GPIO_InitStructure.Pin  = GPIO_Pin_3;
        GPIO_InitStructure.Mode = PuHzOdPp;		            //ָ��IO������������ʽ,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
        GPIO_Inilize(GPIO_P2,&GPIO_InitStructure);	        
        break;
        case GPIO_PWM6:
        GPIO_InitStructure.Pin  = GPIO_Pin_6;
        GPIO_InitStructure.Mode = PuHzOdPp;		            //ָ��IO������������ʽ,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
        GPIO_Inilize(GPIO_P1,&GPIO_InitStructure);	        
        break;
        case GPIO_PWM7:
        GPIO_InitStructure.Pin  = GPIO_Pin_7;
        GPIO_InitStructure.Mode = PuHzOdPp;		            //ָ��IO������������ʽ,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
        GPIO_Inilize(GPIO_P1,&GPIO_InitStructure);	        
        break;
        
        
        case GPIO_PWM2_2:
        GPIO_InitStructure.Pin  = GPIO_Pin_7;
        GPIO_InitStructure.Mode = PuHzOdPp;		            //ָ��IO������������ʽ,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
        GPIO_Inilize(GPIO_P2,&GPIO_InitStructure);	        
        break;
        case GPIO_PWM3_2:
        GPIO_InitStructure.Pin  = GPIO_Pin_5;
        GPIO_InitStructure.Mode = PuHzOdPp;		            //ָ��IO������������ʽ,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
        GPIO_Inilize(GPIO_P4,&GPIO_InitStructure);	        
        break;
        case GPIO_PWM4_2:
        GPIO_InitStructure.Pin  = GPIO_Pin_4;
        GPIO_InitStructure.Mode = PuHzOdPp;		            //ָ��IO������������ʽ,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
        GPIO_Inilize(GPIO_P4,&GPIO_InitStructure);	        
        break;
        case GPIO_PWM5_2:
        GPIO_InitStructure.Pin  = GPIO_Pin_2;
        GPIO_InitStructure.Mode = PuHzOdPp;		            //ָ��IO������������ʽ,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
        GPIO_Inilize(GPIO_P4,&GPIO_InitStructure);	        
        break;
        case GPIO_PWM6_2:
        GPIO_InitStructure.Pin  = GPIO_Pin_7;
        GPIO_InitStructure.Mode = PuHzOdPp;		            //ָ��IO������������ʽ,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
        GPIO_Inilize(GPIO_P0,&GPIO_InitStructure);	        
        break;
        case GPIO_PWM7_2:
        GPIO_InitStructure.Pin  = GPIO_Pin_6;
        GPIO_InitStructure.Mode = PuHzOdPp;		            //ָ��IO������������ʽ,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
        GPIO_Inilize(GPIO_P0,&GPIO_InitStructure);	        
        break;
        
        default:
        return 1;
        break;
    }
    return 0;
}
