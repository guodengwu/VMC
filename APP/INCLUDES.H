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
* 作者   : Jean J. Labrosse
* 改编   : 杨屹 gdtyy@ri.gdt.com.cn 巨龙公司系统集成开发部 2002.09.27
*********************************************************************************************************
*/

//uC/OS-II核心
#ifndef __INCLUDES__
#define __INCLUDES__


#include    "..\uCOSII251\os_cpu.h"
#include    "..\uCOSII251\os_cfg.h"
#include    "..\uCOSII251\ucos_ii.h"


#include    "app_cfg.h"

//用户头文件加在最后面
//Your head file add here.

#include    "..\BSP\bsp.h"
#include    "..\BSP\globalvariable.h"

#include    "..\STC_LIB\config.h"
#include    "..\STC_LIB\my_include.h"
#include    "..\APP\task_manage.h"
#include    "..\APP\test.h"
#include    "..\APP\led.h"


#endif



