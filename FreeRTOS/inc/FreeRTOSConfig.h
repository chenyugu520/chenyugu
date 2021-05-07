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
#define configUSE_TIMERS    			1		//?a��1��?xEventGroupClearBitsFromISRo����y??����?���ꡧ��??����?
#define configUSE_PREEMPTION			1  //Ϊ1ʱRTOSʹ����ռʽ��������Ϊ0ʱRTOSʹ��Э��ʽ��������ʱ��Ƭ��
#define configUSE_IDLE_HOOK				0
#define configUSE_TICK_HOOK				0
//֧�ֶ�̬�ڴ�����
#define configSUPPORT_DYNAMIC_ALLOCATION        1    
//֧�־�̬�ڴ�
#define configSUPPORT_STATIC_ALLOCATION					0		
#define configCPU_CLOCK_HZ				( SystemCoreClock ) //д��ʵ�ʵ�CPU�ں�ʱ��Ƶ�ʣ�Ҳ����CPUָ��ִ��Ƶ�ʣ�ͨ����ΪFcclk�����ô�ֵ��Ϊ����ȷ������ϵͳ�����ж����ڡ�
#define configTICK_RATE_HZ				( ( portTickType ) 1000 ) // RTOS ϵͳ�����жϵ�Ƶ�ʡ���һ���жϵĴ�����ÿ���ж�RTOS�������������ȡ�
#define configMAX_PRIORITIES			( 8 )  //����Ӧ�ó�����Ч�����ȼ���Ŀ���κ����������񶼿��Թ���һ�����ȼ���ʹ��Э�̿��Ե����ĸ�����������Ȩ��
#define configMINIMAL_STACK_SIZE		( ( unsigned short ) 128 )  //�����������ʹ�õĶ�ջ��С��ͨ����ֵ��ӦС�ڶ�Ӧ��������ʾ�����ļ�FreeRTOSConfig.h�ж������ֵ��
#define configTOTAL_HEAP_SIZE			( ( size_t ) ( 32 * 1024 ) ) // RTOS�ں��ܼƿ��õ���Ч��RAM��С��������ʹ�ùٷ����ذ��и������ڴ�������ʱ�����п����õ���ֵ��
#define configMAX_TASK_NAME_LEN			( 16 ) //����������ʱ����Ҫ��������������Ϣ���ַ��������������������ַ�������󳤶ȡ����ﶨ��ĳ��Ȱ����ַ�����������\0����
#define configUSE_TRACE_FACILITY		1 	//���ó�1��ʾ�������ӻ����ٵ��ԣ��ἤ��һЩ���ӵĽṹ���Ա�ͺ�����
#define configUSE_16_BIT_TICKS			0   //����ϵͳ���ļ������ı������ͣ�������portTickType�Ǳ�ʾ16λ��������32λ������
#define configIDLE_SHOULD_YIELD			1 //����������������ڿ������ȼ��е���Ϊ��
#define configUSE_TASK_NOTIFICATIONS    1  //���ú�configUSE_TASK_NOTIFICATIONSΪ1���򲻶����configUSE_TASK_NOTIFICATIONS�����Ὺ������֪ͨ���ܣ��йص�API����Ҳ�ᱻ���롣
#define configUSE_MUTEXES				1   //����Ϊ1��ʾʹ�û����������ó�0��ʾ���Ի�������
#define configQUEUE_REGISTRY_SIZE		8 //������Լ�¼�Ķ��к��ź����������Ŀ��
#define configCHECK_FOR_STACK_OVERFLOW	0  //��ջ���������,�Ƽ����ڿ�������Խ׶�ʹ��ջ�����飬��Ϊ��ջ������������������л�������
#define configUSE_RECURSIVE_MUTEXES		1  //���ó�1��ʾʹ�õݹ黥���������ó�0��ʾ��ʹ�á�
#define configUSE_MALLOC_FAILED_HOOK	0
#define configUSE_APPLICATION_TASK_TAG	        0
#define configUSE_COUNTING_SEMAPHORES	        1  //���ó�1��ʾʹ�ü����ź��������ó�0��ʾ��ʹ�á�
//#define configGENERATE_RUN_TIME_STATS	            //���ú�configGENERATE_RUN_TIME_STATSΪ1ʹ������ʱ��ͳ�ƹ��ܡ�          
                    
#define configENABLE_BACKWARD_COMPATIBILITY 1  //ͷ�ļ�FreeRTOS.h����һϵ��#define�궨�壬����ӳ��汾V8.0.0��V8.0.0֮ǰ�汾�������������֡���Щ�����ȷ��RTOS�ں�������V8.0.0�����ϰ汾ʱ��֮ǰ��Ӧ�ô��벻�����κ��޸ġ�
/* Co-routine definitions. */
#define configUSE_CO_ROUTINES 		        0  //���ó�1��ʾʹ��Э�̣�0��ʾ��ʹ��Э�̡����ʹ��Э�̣������ڹ����а���croutine.c�ļ���
#define configMAX_CO_ROUTINE_PRIORITIES        ( 2 )   //Ӧ�ó���Э�̣�Co-routines������Ч���ȼ���Ŀ���κ���Ŀ��Э�̶����Թ���һ�����ȼ���

/* Software timer definitions. ����?t??����?��?����?*/
#define configUSE_TIMERS				1   //���ó�1ʹ�������ʱ����Ϊ0��ʹ�������ʱ�����ܡ���ϸ������FreeRTOS software timers ��
#define configTIMER_TASK_PRIORITY		( 2 )   //���������ʱ������/�ػ����̵����ȼ�����ϸ������FreeRTOS software timers ��
#define configTIMER_QUEUE_LENGTH		10  //���������ʱ��������еĳ��ȡ���ϸ������FreeRTOS software timers��
#define configTIMER_TASK_STACK_DEPTH	        ( configMINIMAL_STACK_SIZE * 2 )   //���������ʱ������/�ػ���������Ķ�ջ��ȣ���ϸ������FreeRTOS software timers ��

/* Set the following definitions to 1 to include the API function, or zero
to exclude the API function. */

#define INCLUDE_vTaskPrioritySet		1
#define INCLUDE_uxTaskPriorityGet		1
#define INCLUDE_vTaskDelete				1
#define INCLUDE_vTaskCleanUpResources	0
#define INCLUDE_vTaskSuspend			1
#define INCLUDE_vTaskDelayUntil			1
#define INCLUDE_vTaskDelay				1
#define INCLUDE_xTimerPendFunctionCall  1     //?a��1��?xEventGroupClearBitsFromISRo����y??����?���ꡧ��??����?
#define INCLUDE_xEventGroupSetBitFromISR  1		//?a��1��?xEventGroupClearBitsFromISRo����y??����?���ꡧ��??����?
/* Cortex-M specific definitions. */

/******************************************************************
            FreeRTOS���ж��йص�����ѡ��                                                 
******************************************************************/
#ifdef __NVIC_PRIO_BITS
	#define configPRIO_BITS       		__NVIC_PRIO_BITS
#else
	#define configPRIO_BITS       		4                  
#endif
//�ж�������ȼ�
#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY			15     

//ϵͳ�ɹ��������ж����ȼ�
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


