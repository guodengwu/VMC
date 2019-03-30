/*
*********************************************************************************************************
*                                               uC/OS-II
*                                               实时内核
*
*                        (c) Copyright 1992-1998, Jean J. Labrosse, Plantation, FL
*                                               版权所有
*
*                                            MCU-51 专用代码
*                                           KEIL C51大模式编译
*
* 文件名 : INCLUDES.H
*********************************************************************************************************
*/

//uC/OS-II核心
#ifndef __INCLUDES__
#define __INCLUDES__

#include    <string.h> 

#include    "os_cpu.h"
#include    "os_cfg.h"
#include    "ucos_ii.h"
#include    "app_cfg.h"

#include  "msg.h"
//用户头文件加在最后面
//Your head file add here.
#include    "..\BSP\bsp.h"
#include    "..\BSP\globalvariable.h"
#include    "..\STC_LIB\config.h"
//#include    "..\STC_LIB\my_include.h"
#include    "..\APP\task_manage.h"
#include    "..\APP\test.h"
#include    "..\APP\led.h"

#include	"GPIO.h"
#include  "USART.h"
#include "ring_buffer.h"
#include  "msg.h"

#endif



