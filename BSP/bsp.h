#ifndef __CPU_BSP_H__
#define __CPU_BSP_H__

////////////////////////////
//总头文件
#include "includes.h"

/////////////////////////////////
//LED管脚预定义
#define     LED1    P42
#define     LED2    P41
#define     LED3    P35   
#define     LED4    P37  
#define     BEEP    P31  

extern void bsp(void) reentrant;

#endif
