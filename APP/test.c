

//////////////////////////////////
//ͷ�ļ�
#include "includes.h"

//////////////////////////////////
//��ջ
OS_STK  TASK_TEST_STK[MaxStkSize+1]; //�����ջ����


///////////////////////////////////////////////
//����ɨ����ν��յ����ݻ���������û����Ч������
//�������Ч����������Э��ִ�ж���
//����Ч���� �޶���
static void TestStrlamplight(void) reentrant
{
	u8 k[]={"lamp light "}; //��ͷ
	u8 i=0;
	u8 j=0;
 
	while(1<COM1.RX_Cnt)		//������н��յ�����û����
	{
		if(k[j]==RX1_Buffer[i]) //ɨ�軺����
		{
            j++;						    //ÿ����һ����ͷ��++
            if(11==j)						//�����11���ֽڶ�������
            {
                    i++;					//�������Ķ�������++
                    break;
            }
            else
            {
                i++;
            }
	   }
        else//�ڶ���ͷʱ�������һ������ ��������ɨ�� j=0
        {
				j=0;
				i++;
        }	
	}
	
	if(1<COM1.RX_Cnt)				//������н��յ�����û����
	{
		RX1_Buffer[i]-='0';   //ת���ɶ�Ӧ����
		switch(RX1_Buffer[i])
		{
				case 0:
				LED1=1;
				LED2=1;
				LED3=1;
				break;
				
				case 1:
				LED1=0;
				break;
				
				case 2:
				LED2=0;
				break;
				
				case 3:
				LED3=0;
				break;
							
				default:				
				break;
		}
	}

}


///////////////////////////////////////////////
//Test����
void TaskTest(void * ppdata) reentrant
{ 
	ppdata = ppdata;  
	for(;;)
	{               
        if(0==COM1.RX_TimeOut)          //���20ms��û���յ�����
		{
            if(0!=COM1.RX_Cnt)          //������յ�����һ���ֽڵ�����
            {
                    COM1.B_RX_OK=1;     //ʹ����δ���12 ���������ݴ����ʱ����ʱ���Ե��½��յ�����	1
                    TestStrlamplight(); //ɨ��������ݽ��ջ�����
                    COM1.RX_Cnt=0;      //�����������ˣ����ջ�������������
                    COM1.B_RX_OK=0;     //ʹ����δ���12 ���������ݴ����ʱ����ʱ���Ե��½��յ�����	2
            }       
		}
        else
        {
            COM1.RX_TimeOut--;      //��û��20msû�����ݵ�ʱ�� �Ǿ��ٵȵ� �������˼���Ҫ--
        } 
        
        OSTimeDlyHMSM(0,0,0,20);        
    }
}





