

//////////////////////////////////
//头文件
#include "includes.h"

//////////////////////////////////
//堆栈
OS_STK  TASK_TEST_STK[MaxStkSize+1]; //任务堆栈声明


///////////////////////////////////////////////
//分析扫描这次接收的数据缓存区里有没有有效的命令
//如果有有效的命令则按照协议执行动作
//无有效命令 无动作
static void TestStrlamplight(void) reentrant
{
	u8 k[]={"lamp light "}; //字头
	u8 i=0;
	u8 j=0;
 
	while(1<COM1.RX_Cnt)		//如果还有接收的数据没处理
	{
		if(k[j]==RX1_Buffer[i]) //扫描缓存区
		{
            j++;						    //每对上一个字头就++
            if(11==j)						//如果这11个字节都对上了
            {
                    i++;					//缓存区的读计数再++
                    break;
            }
            else
            {
                i++;
            }
	   }
        else//在对字头时，如果有一个出错 就重新来扫描 j=0
        {
				j=0;
				i++;
        }	
	}
	
	if(1<COM1.RX_Cnt)				//如果还有接收的数据没处理
	{
		RX1_Buffer[i]-='0';   //转换成对应数字
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
//Test任务
void TaskTest(void * ppdata) reentrant
{ 
	ppdata = ppdata;  
	for(;;)
	{               
        if(0==COM1.RX_TimeOut)          //如果20ms都没接收到数据
		{
            if(0!=COM1.RX_Cnt)          //如果接收到至少一个字节的数据
            {
                    COM1.B_RX_OK=1;     //使能这段代码12 可以在数据处理的时候暂时忽略掉新接收的数据	1
                    TestStrlamplight(); //扫描分析数据接收缓冲区
                    COM1.RX_Cnt=0;      //处理完数据了，接收缓存区长度清零
                    COM1.B_RX_OK=0;     //使能这段代码12 可以在数据处理的时候暂时忽略掉新接收的数据	2
            }       
		}
        else
        {
            COM1.RX_TimeOut--;      //还没有20ms没有数据的时候 那就再等等 但别忘了计数要--
        } 
        
        OSTimeDlyHMSM(0,0,0,20);        
    }
}





