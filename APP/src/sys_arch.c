/*
 * Copyright (c) 2017 Simon Goldschmidt
 * All rights reserved. 
 *
 *
 */
//#include "debug.h"

#include <lwip/opt.h>
#include <lwip/arch.h>

#include "lwip/init.h"
#include "lwip/netif.h"
#include "lwip/sio.h"
#include "ethernetif.h"

#if !NO_SYS
#include "sys_arch.h"
#endif
#include <lwip/stats.h>
#include <lwip/debug.h>
#include <lwip/sys.h>
#include "lwip/dhcp.h"
#include <string.h>

int errno;


u32_t lwip_sys_now;

struct sys_timeouts {
  struct sys_timeo *next;
};

struct timeoutlist
{
	struct sys_timeouts timeouts;
	xTaskHandle pid;
};

#define SYS_THREAD_MAX 4

static struct timeoutlist s_timeoutlist[SYS_THREAD_MAX];

static u16_t s_nextthread = 0;
//================================================sys_jiffies==================================================
//获取系统节拍数
u32_t
sys_jiffies(void)
{
  lwip_sys_now = xTaskGetTickCount();
  return lwip_sys_now;
}
//================================================sys_jiffies==================================================

//================================================sys_now==================================================
//获取系统节拍数
u32_t
sys_now(void)
{
  lwip_sys_now = xTaskGetTickCount();
  return lwip_sys_now;
}
//================================================sys_now==================================================

//================================================sys_init==================================================
void
sys_init(void)
{
	int i;
	// Initialize the the per-thread sys_timeouts structures
	// make sure there are no valid pids in the list
  //初始化每个线程的sys_timeouts结构,确保列表中没有有效的pid
	for(i = 0; i < SYS_THREAD_MAX; i++)
	{
		s_timeoutlist[i].pid = 0;
		s_timeoutlist[i].timeouts.next = NULL;
	}
	// keep track of how many threads have been created跟踪已创建的线程数
	s_nextthread = 0;
}
//================================================sys_init==================================================\

//================================================sys_arch_timeouts==================================================
struct sys_timeouts *sys_arch_timeouts(void)
{
	int i;
	xTaskHandle pid;
	struct timeoutlist *tl;
	pid = xTaskGetCurrentTaskHandle( );//xTask获取当前任务句柄
	for(i = 0; i < s_nextthread; i++)
	{
		tl = &(s_timeoutlist[i]);
		if(tl->pid == pid)
		{
			return &(tl->timeouts);
		}
	}
	return NULL;
}

//================================================sys_arch_timeouts==================================================

//================================================sys_arch_protect==================================================
 //封装 开始临时区
sys_prot_t sys_arch_protect(void)
{
	vPortEnterCritical();
	return 1;
}
//================================================sys_arch_protect==================================================

//================================================sys_arch_unprotect==================================================
//封装 退出临时区
void sys_arch_unprotect(sys_prot_t pval)
{
	( void ) pval;
	vPortExitCritical();
}
//================================================sys_arch_unprotect==================================================

#if !NO_SYS
//================================================sys_sem_new==================================================
//创建信号量sem，count小于等于1，创建二进制信号量并发送；否则创建计数信号量并将初始值与最大值设为count
err_t sys_sem_new(sys_sem_t *sem, u8_t count)
{
  /* 创建 sem */
  if(count <= 1)
  {    
    *sem = xSemaphoreCreateBinary();  //创建二进制信号量
    if(count == 1)
    {
      sys_sem_signal(sem);//新创建的信号量是无效的，需要释放一个信号量
    }
  }
  else
    *sem = xSemaphoreCreateCounting(count,count); //创建计数信号量
  
#if SYS_STATS
	++lwip_stats.sys.sem.used;
 	if (lwip_stats.sys.sem.max < lwip_stats.sys.sem.used) {
		lwip_stats.sys.sem.max = lwip_stats.sys.sem.used;
	}
#endif /* SYS_STATS */
  
  if(*sem != SYS_SEM_NULL)
    return ERR_OK;
  else
  {
#if SYS_STATS
    ++lwip_stats.sys.sem.err;
#endif /* SYS_STATS */
    printf("[sys_arch]:new sem fail!\n");
    return ERR_MEM;
  }
}
//================================================sys_sem_new==================================================


//================================================sys_sem_free==================================================
//删除信号量sem,并将sem设置为无效的
void
sys_sem_free(sys_sem_t *sem)
{
#if SYS_STATS
   --lwip_stats.sys.sem.used;
#endif /* SYS_STATS */
  /* 删除 sem */
  vSemaphoreDelete(*sem);
  *sem = SYS_SEM_NULL;//删除之后置空
}
//================================================sys_sem_free==================================================

//================================================sys_sem_valid=================================================
////返回信号量是否有效 
int sys_sem_valid(sys_sem_t *sem)                                               
{
  return (*sem != SYS_SEM_NULL);                                    
}
//================================================sys_sem_valid=================================================

//================================================sys_sem_set_invalid=================================================
 //将sem设置为无效的,不返回
void
sys_sem_set_invalid(sys_sem_t *sem)
{
  *sem = SYS_SEM_NULL;
}
//================================================sys_sem_set_invalid=================================================


//================================================sys_arch_sem_wait=================================================
/*  如果timeout参数不为零，则返回值为等待信号量所花费的毫秒数。如果信号量未在指定时间内发出信号，返回值为
    SYS_ARCH_TIMEOUT。如果线程不必等待信号量该函数返回零。 */
u32_t
sys_arch_sem_wait(sys_sem_t *sem, u32_t timeout)
{
  u32_t wait_tick = 0;
  u32_t start_tick = 0 ;
  
  //看看信号量是否有效
  if(*sem == SYS_SEM_NULL)
    return SYS_ARCH_TIMEOUT;
  
  //首先获取开始等待信号量的时钟节拍
  start_tick = xTaskGetTickCount();
  
  //timeout != 0，需要将ms换成系统的时钟节拍
  if(timeout != 0)
  {
    //将ms转换成时钟节拍
    wait_tick = timeout / portTICK_PERIOD_MS;
    if (wait_tick == 0)
      wait_tick = 1;
  }
  else
    wait_tick = portMAX_DELAY;  //一直阻塞
  
  //等待成功，计算等待的时间，否则就表示等待超时
  if(xSemaphoreTake(*sem, wait_tick) == pdTRUE) //等待获取信号量
    return ((xTaskGetTickCount()-start_tick)*portTICK_RATE_MS);
  else
    return SYS_ARCH_TIMEOUT;
}
//================================================sys_arch_sem_wait=================================================

//================================================sys_sem_signal=================================================
 //发送信号量，释放信号量
void
sys_sem_signal(sys_sem_t *sem)
{
  if(xSemaphoreGive( *sem ) != pdTRUE)
    printf("[sys_arch]:sem signal fail!\n");
}
//创建互斥信号量
err_t
sys_mutex_new(sys_mutex_t *mutex)
{
  /* 创建 sem */   
  *mutex = xSemaphoreCreateMutex(); 
  if(*mutex != SYS_MRTEX_NULL)
    return ERR_OK;
  else
  {
    printf("[sys_arch]:new mutex fail!\n");
    return ERR_MEM;
  }
}
//================================================sys_sem_signal=================================================

//================================================sys_mutex_free=================================================
//删除互斥信号量
void
sys_mutex_free(sys_mutex_t *mutex)
{
  vSemaphoreDelete(*mutex); 
}
//================================================sys_mutex_free=================================================

//================================================sys_mutex_set_invalid=================================================
//设置互斥信号量无效的
void
sys_mutex_set_invalid(sys_mutex_t *mutex)
{
  *mutex = SYS_MRTEX_NULL;  
}
//================================================sys_mutex_set_invalid=================================================

//================================================sys_mutex_lock=================================================
//获取互斥信号量，死等
void
sys_mutex_lock(sys_mutex_t *mutex)  
{
  xSemaphoreTake(*mutex,/* 互斥量句柄 */
                 portMAX_DELAY); /* 等待时间 */
}
//================================================sys_mutex_lock=================================================


//================================================sys_mutex_unlock=================================================
//给出互斥信号量，释放互斥信号量
void
sys_mutex_unlock(sys_mutex_t *mutex)
{
  xSemaphoreGive( *mutex );
}
//================================================sys_mutex_unlock=================================================

//================================================sys_thread_new=================================================
//封装创建任务函数，并返回任务句柄
sys_thread_t
sys_thread_new(const char *name, lwip_thread_fn function, void *arg, int stacksize, int prio) 
{
  sys_thread_t handle = NULL;
  BaseType_t xReturn = pdPASS;
  /* 创建任务 */
  xReturn = xTaskCreate((TaskFunction_t )function,  /* 任务入口函数 */
                        (const char*    )name,/* 任务名字 */
                        (uint16_t       )stacksize,  /* 任务栈大小 */
                        (void*          )arg,/* 任务入口函数参数 */
                        (UBaseType_t    )prio, /* 任务的优先级 */
                        (TaskHandle_t*  )&handle);/* 任务控制块指针 */ 
  if(xReturn != pdPASS)
  {
    printf("[sys_arch]:create task fail!err:%#lx\n",xReturn);
    return NULL;
  }
  return handle;
}
//================================================sys_thread_new=================================================


//================================================sys_mbox_new=================================================
//封装创建队列函数,消息的大小为sizeof(void *)  创建一个邮箱
err_t
sys_mbox_new(sys_mbox_t *mbox, int size)
{
    /* 创建Test_Queue */
  *mbox = xQueueCreate((UBaseType_t ) size,/* 消息队列的长度 */
                       (UBaseType_t ) sizeof(void *));/* 消息的大小 */
#if SYS_STATS
      ++lwip_stats.sys.mbox.used;
      if (lwip_stats.sys.mbox.max < lwip_stats.sys.mbox.used) {
         lwip_stats.sys.mbox.max = lwip_stats.sys.mbox.used;
	  }
#endif /* SYS_STATS */
	if(NULL == *mbox)
    return ERR_MEM;
  
  return ERR_OK;
}
//================================================sys_mbox_new=================================================

//================================================sys_mbox_free=================================================
//删除消息队列
void
sys_mbox_free(sys_mbox_t *mbox)
{
  if( uxQueueMessagesWaiting( *mbox ) )
	{
		/* Line for breakpoint.  Should never break here!断点行。永不中断 */
		portNOP();
#if SYS_STATS
	    lwip_stats.sys.mbox.err++;
#endif /* SYS_STATS */

		// TODO notify the user of failure.
	}
  vQueueDelete(*mbox);  //删除一个邮箱
  
#if SYS_STATS
     --lwip_stats.sys.mbox.used;
#endif /* SYS_STATS */
}
//================================================sys_mbox_free=================================================


//================================================sys_mbox_valid=================================================
//判断mbox(消息队列)有效返回1.无效返回0  
int sys_mbox_valid(sys_mbox_t *mbox)         
{      
  if (*mbox == SYS_MBOX_NULL) 
    return 0;
  else
    return 1;
}  
//================================================sys_mbox_valid=================================================

//================================================sys_mbox_set_invalid=================================================
//设置mbox(消息队列)无效
void
sys_mbox_set_invalid(sys_mbox_t *mbox) 
{
  *mbox = SYS_MBOX_NULL; 
}
//================================================sys_mbox_set_invalid=================================================


//================================================sys_mbox_post=================================================
//封装消息队列发送函数，死等
void
sys_mbox_post(sys_mbox_t *q, void *msg) 
{
  while(xQueueSend( *q, /* 消息队列的句柄 */
                    &msg,/* 发送的消息内容 */
                    portMAX_DELAY) != pdTRUE); /* 等待时间 */
}
//================================================sys_mbox_post=================================================

//================================================sys_mbox_trypost=================================================
//封装消息队列发送函数，不等
err_t
sys_mbox_trypost(sys_mbox_t *q, void *msg)
{
  if(xQueueSend(*q,&msg,0) == pdPASS)  //尝试发送一个消息，非阻塞发送
    return ERR_OK;
  else
    return ERR_MEM;
}
//================================================sys_mbox_trypost=================================================

//================================================sys_mbox_trypost_fromisr=================================================
//封装消息队列发送函数，不等
err_t
sys_mbox_trypost_fromisr(sys_mbox_t *q, void *msg)
{
  return sys_mbox_trypost(q, msg);
}
//================================================sys_mbox_trypost_fromisr=================================================



//================================================sys_arch_mbox_fetch=================================================
//封装消息队列读取函数，timeout=0时，死等；timeout != 0时，设置等待时间为timeout
u32_t
sys_arch_mbox_fetch(sys_mbox_t *q, void **msg, u32_t timeout)
{
  void *dummyptr;
  u32_t wait_tick = 0;
  u32_t start_tick = 0 ;
  
  if ( msg == NULL )  //看看存储消息的地方是否有效
		msg = &dummyptr;
  
  //首先获取开始等待信号量的时钟节拍
  start_tick = sys_now();
  
  //timeout != 0，需要将ms换成系统的时钟节拍
  if(timeout != 0)
  {
    //将ms转换成时钟节拍
    wait_tick = timeout / portTICK_PERIOD_MS;
    if (wait_tick == 0)
      wait_tick = 1;
  }
  //一直阻塞
  else
    wait_tick = portMAX_DELAY;
  
  //等待成功，计算等待的时间，否则就表示等待超时
  if(xQueueReceive(*q,&(*msg), wait_tick) == pdTRUE)
    return ((sys_now() - start_tick)*portTICK_PERIOD_MS);
  else
  {
    *msg = NULL;
    return SYS_ARCH_TIMEOUT;
  }
}
//================================================sys_arch_mbox_fetch=================================================




//================================================sys_arch_mbox_tryfetch=================================================
//封装消息队列读取函数，不等
u32_t
sys_arch_mbox_tryfetch(sys_mbox_t *q, void **msg)
{
	void *dummyptr;
	if ( msg == NULL )
		msg = &dummyptr;
  
  //等待成功，计算等待的时间
  if(xQueueReceive(*q,&(*msg), 0) == pdTRUE)
    return ERR_OK;
  else
    return SYS_MBOX_EMPTY;
}
//================================================sys_arch_mbox_tryfetch=================================================

#if LWIP_NETCONN_SEM_PER_THREAD
#error LWIP_NETCONN_SEM_PER_THREAD==1 not supported
#endif /* LWIP_NETCONN_SEM_PER_THREAD */

#endif /* !NO_SYS */


//================================================TCPIP_Init=================================================
/* Variables Initialization */
struct netif gnetif;
ip4_addr_t ipaddr;
ip4_addr_t netmask;
ip4_addr_t gw;
uint8_t IP_ADDRESS[4];
uint8_t NETMASK_ADDRESS[4];
uint8_t GATEWAY_ADDRESS[4];

void TCPIP_Init(void)
{
  tcpip_init(NULL, NULL);  //初始化tcpip
  
  /* IP addresses initialization IP地址初始化*/
  /* USER CODE BEGIN 0 */
#if LWIP_DHCP
  ip_addr_set_zero_ip4(&ipaddr);  //清空
  ip_addr_set_zero_ip4(&netmask);
  ip_addr_set_zero_ip4(&gw);
#else
  IP4_ADDR(&ipaddr,IP_ADDR0,IP_ADDR1,IP_ADDR2,IP_ADDR3);
  IP4_ADDR(&netmask,NETMASK_ADDR0,NETMASK_ADDR1,NETMASK_ADDR2,NETMASK_ADDR3);
  IP4_ADDR(&gw,GW_ADDR0,GW_ADDR1,GW_ADDR2,GW_ADDR3);
#endif /* USE_DHCP */
  /* USER CODE END 0 */
  /* Initilialize the LwIP stack without RTOS 
  在没有RTOS的情况下初始化LwIP堆栈 */
  /* add the network interface (IPv4/IPv6) without RTOS
  添加不带RTOS的网络接口（IPv4 / IPv6） */
  struct netif *jing;
  jing = netif_add(&gnetif, &ipaddr, &netmask, &gw, NULL, &ethernetif_init, &tcpip_input); //
  if(jing == NULL)
    printf("网络接口添加失败\n");

  /* Registers the default network interface
  注册默认网络接口 */
  netif_set_default(&gnetif);

  if (netif_is_link_up(&gnetif)) //询问链接是否建立
  {
    /* When the netif is fully configured this function must be called
    完全配置netif后，必须调用此函数 */
		printf("\n 链接建立成功\n");
    netif_set_up(&gnetif);
  }
  else
  {
    /* When the netif link is down this function must be called 
    当netif链接关闭时，必须调用此函数*/
		printf("\n 链接建立失败\n");
    netif_set_down(&gnetif);
  }
  
#if LWIP_DHCP	   			//若使用了DHCP
  int err;
  /*  Creates a new DHCP client for this interface on the first call.
  Note: you must call dhcp_fine_tmr() and dhcp_coarse_tmr() at
  the predefined regular intervals after starting the client.
  You can peek in the netif->dhcp struct for the actual DHCP status.
  在第一个调用时为此接口创建一个新的DHCP客户端。注意：启动客户端后，必须以预定义的定期间隔调用dhcp_fine_tmr（）和dhcp_coarse_tmr（）。
  您可以在netif-> dhcp结构中窥视实际的DHCP状态*/
  
  printf("本例程将使用DHCP动态分配IP地址,如果不需要则在lwipopts.h中将LWIP_DHCP定义为0\n\n");
  
  err = dhcp_start(&gnetif);      //开启dhcp
  if(err == ERR_OK)
    printf("lwip dhcp init success...\n\n");
  else
    printf("lwip dhcp init fail...\n\n");
  while(ip_addr_cmp(&(gnetif.ip_addr),&ipaddr))   //等待dhcp分配的ip有效
  {
    //printf("gnetif.ip_addr=%d\tipaddr=%d",gnetif.ip_addr.addr,ipaddr.addr);
    vTaskDelay(1);
  } 
#endif
  printf("本地IP地址是:%d.%d.%d.%d\n\n",  \
        ((gnetif.ip_addr.addr)&0x000000ff),       \
        (((gnetif.ip_addr.addr)&0x0000ff00)>>8),  \
        (((gnetif.ip_addr.addr)&0x00ff0000)>>16), \
        ((gnetif.ip_addr.addr)&0xff000000)>>24);
}


//================================================TCPIP_Init=================================================







