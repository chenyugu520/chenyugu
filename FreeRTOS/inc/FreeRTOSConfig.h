/*
 * FreeRTOS Kernel V10.2.1
 * Copyright (C) 2019 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */

#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

/*-----------------------------------------------------------
 * Application specific definitions.
 *
 * These definitions should be adjusted for your particular hardware and
 * application requirements.
 *
 * THESE PARAMETERS ARE DESCRIBED WITHIN THE 'CONFIGURATION' SECTION OF THE
 * FreeRTOS API DOCUMENTATION AVAILABLE ON THE FreeRTOS.org WEB SITE. 
 *
 * See http://www.freertos.org/a00110.html
 *----------------------------------------------------------*/

/* Ensure stdint is only used by the compiler, and not the assembler. */
#if defined(__ICCARM__) || defined(__CC_ARM) || defined(__TASKING__) || defined(__GNUC__)
	#include <stdint.h>
	extern volatile uint32_t ulHighFrequencyTimerTicks;
	extern uint32_t SystemCoreClock;
#endif
#define configUSE_TIMERS    			1		//?a锟斤拷1锟斤拷?xEventGroupClearBitsFromISRo锟斤拷锟斤拷y??锟斤拷锟斤拷?锟斤拷锟疥¨锟斤拷??锟斤拷锟斤拷?
#define configUSE_PREEMPTION			1  //为1时RTOS使用抢占式调度器，为0时RTOS使用协作式调度器（时间片）
#define configUSE_IDLE_HOOK				0
#define configUSE_TICK_HOOK				0
//支持动态内存申请
#define configSUPPORT_DYNAMIC_ALLOCATION        1    
//支持静态内存
#define configSUPPORT_STATIC_ALLOCATION					0		
#define configCPU_CLOCK_HZ				( SystemCoreClock ) //写入实际的CPU内核时钟频率，也就是CPU指令执行频率，通常称为Fcclk。配置此值是为了正确的配置系统节拍中断周期。
#define configTICK_RATE_HZ				( ( portTickType ) 1000 ) // RTOS 系统节拍中断的频率。即一秒中断的次数，每次中断RTOS都会进行任务调度。
#define configMAX_PRIORITIES			( 8 )  //配置应用程序有效的优先级数目。任何数量的任务都可以共享一个优先级，使用协程可以单独的给与它们优先权。
#define configMINIMAL_STACK_SIZE		( ( unsigned short ) 128 )  //定义空闲任务使用的堆栈大小。通常此值不应小于对应处理器演示例程文件FreeRTOSConfig.h中定义的数值。
#define configTOTAL_HEAP_SIZE			( ( size_t ) ( 32 * 1024 ) ) // RTOS内核总计可用的有效的RAM大小。仅在你使用官方下载包中附带的内存分配策略时，才有可能用到此值。
#define configMAX_TASK_NAME_LEN			( 16 ) //调用任务函数时，需要设置描述任务信息的字符串，这个宏用来定义该字符串的最大长度。这里定义的长度包括字符串结束符’\0’。
#define configUSE_TRACE_FACILITY		1 	//设置成1表示启动可视化跟踪调试，会激活一些附加的结构体成员和函数。
#define configUSE_16_BIT_TICKS			0   //定义系统节拍计数器的变量类型，即定义portTickType是表示16位变量还是32位变量。
#define configIDLE_SHOULD_YIELD			1 //这个参数控制任务在空闲优先级中的行为。
#define configUSE_TASK_NOTIFICATIONS    1  //设置宏configUSE_TASK_NOTIFICATIONS为1（或不定义宏configUSE_TASK_NOTIFICATIONS）将会开启任务通知功能，有关的API函数也会被编译。
#define configUSE_MUTEXES				1   //设置为1表示使用互斥量，设置成0表示忽略互斥量。
#define configQUEUE_REGISTRY_SIZE		8 //定义可以记录的队列和信号量的最大数目。
#define configCHECK_FOR_STACK_OVERFLOW	0  //堆栈溢出检测机制,推荐仅在开发或测试阶段使用栈溢出检查，因为堆栈溢出检测会增大上下文切换开销。
#define configUSE_RECURSIVE_MUTEXES		1  //设置成1表示使用递归互斥量，设置成0表示不使用。
#define configUSE_MALLOC_FAILED_HOOK	0
#define configUSE_APPLICATION_TASK_TAG	        0
#define configUSE_COUNTING_SEMAPHORES	        1  //设置成1表示使用计数信号量，设置成0表示不使用。
//#define configGENERATE_RUN_TIME_STATS	            //设置宏configGENERATE_RUN_TIME_STATS为1使能运行时间统计功能。          
                    
#define configENABLE_BACKWARD_COMPATIBILITY 1  //头文件FreeRTOS.h包含一系列#define宏定义，用来映射版本V8.0.0和V8.0.0之前版本的数据类型名字。这些宏可以确保RTOS内核升级到V8.0.0或以上版本时，之前的应用代码不用做任何修改。
/* Co-routine definitions. */
#define configUSE_CO_ROUTINES 		        0  //设置成1表示使用协程，0表示不使用协程。如果使用协程，必须在工程中包含croutine.c文件。
#define configMAX_CO_ROUTINE_PRIORITIES        ( 2 )   //应用程序协程（Co-routines）的有效优先级数目，任何数目的协程都可以共享一个优先级。

/* Software timer definitions. 锟斤拷锟斤拷?t??锟斤拷锟斤拷?锟斤拷?锟斤拷锟斤拷?*/
#define configUSE_TIMERS				1   //设置成1使用软件定时器，为0不使用软件定时器功能。详细描述见FreeRTOS software timers 。
#define configTIMER_TASK_PRIORITY		( 2 )   //设置软件定时器服务/守护进程的优先级。详细描述见FreeRTOS software timers 。
#define configTIMER_QUEUE_LENGTH		10  //设置软件定时器命令队列的长度。详细描述见FreeRTOS software timers。
#define configTIMER_TASK_STACK_DEPTH	        ( configMINIMAL_STACK_SIZE * 2 )   //设置软件定时器服务/守护进程任务的堆栈深度，详细描述见FreeRTOS software timers 。

/* Set the following definitions to 1 to include the API function, or zero
to exclude the API function. */

#define INCLUDE_vTaskPrioritySet		1
#define INCLUDE_uxTaskPriorityGet		1
#define INCLUDE_vTaskDelete				1
#define INCLUDE_vTaskCleanUpResources	0
#define INCLUDE_vTaskSuspend			1
#define INCLUDE_vTaskDelayUntil			1
#define INCLUDE_vTaskDelay				1
#define INCLUDE_xTimerPendFunctionCall  1     //?a锟斤拷1锟斤拷?xEventGroupClearBitsFromISRo锟斤拷锟斤拷y??锟斤拷锟斤拷?锟斤拷锟疥¨锟斤拷??锟斤拷锟斤拷?
#define INCLUDE_xEventGroupSetBitFromISR  1		//?a锟斤拷1锟斤拷?xEventGroupClearBitsFromISRo锟斤拷锟斤拷y??锟斤拷锟斤拷?锟斤拷锟疥¨锟斤拷??锟斤拷锟斤拷?
/* Cortex-M specific definitions. */

/******************************************************************
            FreeRTOS与中断有关的配置选项                                                 
******************************************************************/
#ifdef __NVIC_PRIO_BITS
	#define configPRIO_BITS       		__NVIC_PRIO_BITS
#else
	#define configPRIO_BITS       		4                  
#endif
//中断最低优先级
#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY			15     

//系统可管理的最高中断优先级
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY	5 

#define configKERNEL_INTERRUPT_PRIORITY 		( configLIBRARY_LOWEST_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )	/* 240 */

#define configMAX_SYSCALL_INTERRUPT_PRIORITY 	( configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )
																							


/* This is the value being used as per the ST library which permits 16
priority values, 0 to 15.  This must correspond to the
configKERNEL_INTERRUPT_PRIORITY setting.  Here 15 corresponds to the lowest
NVIC value of 255. */
#define configLIBRARY_KERNEL_INTERRUPT_PRIORITY	15

#define vPortSVCHandler SVC_Handler
#define xPortPendSVHandler PendSV_Handler
#define xPortSysTickHandler SysTick_Handler

//#define vAssertCalled(char,int) printf("Error:%s,%d\r\n",char,int)
//#define configASSERT(x) if((x)==0) vAssertCalled(__FILE__,__LINE__) 



#if ( configUSE_TRACE_FACILITY == 1 )

#include "trcRecorder.h"
#endif
#endif /* FREERTOS_CONFIG_H */


