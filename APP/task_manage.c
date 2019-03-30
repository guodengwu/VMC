/* Includes -----------------------------------------------------------------*/
#include "includes.h"
#include "app_usart.h"
//////////////////////////////////////////////////////////
OS_STK  TASK_START_STK[MaxStkSize+1]; //ÈÎÎñ¶ÑÕ»ÉùÃ÷

//////////////////////////////////////////////////////////
void TaskCreateOther(void) reentrant;             //ÓÃÕâ¸öº¯ÊıÀ´´´½¨ÆäËûÈÎÎñ

#include "tm1640.h"
//////////////////////////////////////////////////////////
//ÆğÊ¼ÈÎÎñ ÓÖÕâ¸öÈÎÎñÔÙ´´½¨ÆäËûÈÎÎñ
void TaskStart(void * ppdata) reentrant
{
	ppdata = ppdata;
	
    ET0=1;                              //Ê¹ÄÜÊ±ÖÓ½ÚÅÄÖĞ¶Ï-------¸ù¾İÑîÒÙÎÄÕÂ¸ü¸Ä
    
    //Ê¹ÄÜucos µÄÍ³¼ÆÈÎÎñ
    #if (OS_TASK_STAT_EN > 0)
    OSStatInit();                       //Í³¼ÆÈÎÎñ³õÊ¼»¯º¯Êı   
    #endif  
    
    bsp();                              //°åÔØÓ²¼ş³õÊ¼»¯
    globalvariable();                   //ÓÃ»§Ó¦ÓÃ³ÌĞòÈ«¾Ö±äÁ¿³õÊ¼»¯
		Update_DisString("----");
		BEEP=1;
		OSTimeDlyHMSM(0,0,1,500); 
		Update_DisString("    ");
		BEEP=0;
    TaskCreateOther();			        //½¨Á¢ÆäËûµÄÈÎÎñ
    OSTaskSuspend(OS_PRIO_SELF);	    //suspend but not delete ¹ÒÆğÈÎÎñ	
}

/////////////////////////////////////////////////////////////////////////////
//´´½¨ÆäËûËùÓĞÈÎÎñ ÔÚÕâÀïÎÒ½¨Á¢ÁË¸öLEDÉÁË¸µÄÈÎÎñµ±Àı×Ó
void TaskCreateOther(void) reentrant
{
    //OSTaskCreate(TaskTest, (void * )0, (OS_STK *)&TASK_TEST_STK[0], TEST_TASK_Prio);                  //TESTÈÎÎññ
    //OSTaskCreate(TaskLed,  (void * )0, (OS_STK *)&TASK_LED_STK[0], LED_TASK_Prio);                     //LEDÏÔÊ¾ÈÎÎññ		
		UsartTxTaskInit();
		UsartRxTaskInit();
}


      
