#ifndef __CPU_BSP_H__
#define __CPU_BSP_H__

////////////////////////////
//��ͷ�ļ�
#include "includes.h"

/////////////////////////////////
//LED�ܽ�Ԥ����
#define     LED1    P42
#define     LED2    P41
#define     LED3    P35   
#define     LED4    P37  
#define     BEEP    P31  

#define TM1640_SCLK    P66
#define TM1640_DIO     P67

extern void bsp(void) reentrant;

#endif
