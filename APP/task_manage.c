/* Includes -----------------------------------------------------------------*/
#include "includes.h"
#include "app_usart.h"
//////////////////////////////////////////////////////////
OS_STK  TASK_START_STK[MaxStkSize+1]; //�����ջ����

//////////////////////////////////////////////////////////
void TaskCreateOther(void) reentrant;             //�����������������������

#include "tm1640.h"
//////////////////////////////////////////////////////////
//��ʼ���� ����������ٴ�����������
void TaskStart(void * ppdata) reentrant
{
	ppdata = ppdata;
	
    ET0=1;                              //ʹ��ʱ�ӽ����ж�-------�����������¸���
    
    //ʹ��ucos ��ͳ������
    #if (OS_TASK_STAT_EN > 0)
    OSStatInit();                       //ͳ�������ʼ������   
    #endif  
    
    bsp();                              //����Ӳ����ʼ��
    globalvariable();                   //�û�Ӧ�ó���ȫ�ֱ�����ʼ��
		Update_DisString("----");
		BEEP=1;
		OSTimeDlyHMSM(0,0,1,500); 
		Update_DisString("    ");
		BEEP=0;
    TaskCreateOther();			        //��������������
    OSTaskSuspend(OS_PRIO_SELF);	    //suspend but not delete ��������	
}

/////////////////////////////////////////////////////////////////////////////
//���������������� �������ҽ����˸�LED��˸����������
void TaskCreateOther(void) reentrant
{
    //OSTaskCreate(TaskTest, (void * )0, (OS_STK *)&TASK_TEST_STK[0], TEST_TASK_Prio);                  //TEST�����
    //OSTaskCreate(TaskLed,  (void * )0, (OS_STK *)&TASK_LED_STK[0], LED_TASK_Prio);                     //LED��ʾ�����		
		UsartTxTaskInit();
		UsartRxTaskInit();
}


      
