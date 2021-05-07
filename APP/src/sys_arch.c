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
//��ȡϵͳ������
u32_t
sys_jiffies(void)
{
  lwip_sys_now = xTaskGetTickCount();
  return lwip_sys_now;
}
//================================================sys_jiffies==================================================

//================================================sys_now==================================================
//��ȡϵͳ������
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
  //��ʼ��ÿ���̵߳�sys_timeouts�ṹ,ȷ���б���û����Ч��pid
	for(i = 0; i < SYS_THREAD_MAX; i++)
	{
		s_timeoutlist[i].pid = 0;
		s_timeoutlist[i].timeouts.next = NULL;
	}
	// keep track of how many threads have been created�����Ѵ������߳���
	s_nextthread = 0;
}
//================================================sys_init==================================================\

//================================================sys_arch_timeouts==================================================
struct sys_timeouts *sys_arch_timeouts(void)
{
	int i;
	xTaskHandle pid;
	struct timeoutlist *tl;
	pid = xTaskGetCurrentTaskHandle( );//xTask��ȡ��ǰ������
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
 //��װ ��ʼ��ʱ��
sys_prot_t sys_arch_protect(void)
{
	vPortEnterCritical();
	return 1;
}
//================================================sys_arch_protect==================================================

//================================================sys_arch_unprotect==================================================
//��װ �˳���ʱ��
void sys_arch_unprotect(sys_prot_t pval)
{
	( void ) pval;
	vPortExitCritical();
}
//================================================sys_arch_unprotect==================================================

#if !NO_SYS
//================================================sys_sem_new==================================================
//�����ź���sem��countС�ڵ���1�������������ź��������ͣ����򴴽������ź���������ʼֵ�����ֵ��Ϊcount
err_t sys_sem_new(sys_sem_t *sem, u8_t count)
{
  /* ���� sem */
  if(count <= 1)
  {    
    *sem = xSemaphoreCreateBinary();  //�����������ź���
    if(count == 1)
    {
      sys_sem_signal(sem);//�´������ź�������Ч�ģ���Ҫ�ͷ�һ���ź���
    }
  }
  else
    *sem = xSemaphoreCreateCounting(count,count); //���������ź���
  
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
//ɾ���ź���sem,����sem����Ϊ��Ч��
void
sys_sem_free(sys_sem_t *sem)
{
#if SYS_STATS
   --lwip_stats.sys.sem.used;
#endif /* SYS_STATS */
  /* ɾ�� sem */
  vSemaphoreDelete(*sem);
  *sem = SYS_SEM_NULL;//ɾ��֮���ÿ�
}
//================================================sys_sem_free==================================================

//================================================sys_sem_valid=================================================
////�����ź����Ƿ���Ч 
int sys_sem_valid(sys_sem_t *sem)                                               
{
  return (*sem != SYS_SEM_NULL);                                    
}
//================================================sys_sem_valid=================================================

//================================================sys_sem_set_invalid=================================================
 //��sem����Ϊ��Ч��,������
void
sys_sem_set_invalid(sys_sem_t *sem)
{
  *sem = SYS_SEM_NULL;
}
//================================================sys_sem_set_invalid=================================================


//================================================sys_arch_sem_wait=================================================
/*  ���timeout������Ϊ�㣬�򷵻�ֵΪ�ȴ��ź��������ѵĺ�����������ź���δ��ָ��ʱ���ڷ����źţ�����ֵΪ
    SYS_ARCH_TIMEOUT������̲߳��صȴ��ź����ú��������㡣 */
u32_t
sys_arch_sem_wait(sys_sem_t *sem, u32_t timeout)
{
  u32_t wait_tick = 0;
  u32_t start_tick = 0 ;
  
  //�����ź����Ƿ���Ч
  if(*sem == SYS_SEM_NULL)
    return SYS_ARCH_TIMEOUT;
  
  //���Ȼ�ȡ��ʼ�ȴ��ź�����ʱ�ӽ���
  start_tick = xTaskGetTickCount();
  
  //timeout != 0����Ҫ��ms����ϵͳ��ʱ�ӽ���
  if(timeout != 0)
  {
    //��msת����ʱ�ӽ���
    wait_tick = timeout / portTICK_PERIOD_MS;
    if (wait_tick == 0)
      wait_tick = 1;
  }
  else
    wait_tick = portMAX_DELAY;  //һֱ����
  
  //�ȴ��ɹ�������ȴ���ʱ�䣬����ͱ�ʾ�ȴ���ʱ
  if(xSemaphoreTake(*sem, wait_tick) == pdTRUE) //�ȴ���ȡ�ź���
    return ((xTaskGetTickCount()-start_tick)*portTICK_RATE_MS);
  else
    return SYS_ARCH_TIMEOUT;
}
//================================================sys_arch_sem_wait=================================================

//================================================sys_sem_signal=================================================
 //�����ź������ͷ��ź���
void
sys_sem_signal(sys_sem_t *sem)
{
  if(xSemaphoreGive( *sem ) != pdTRUE)
    printf("[sys_arch]:sem signal fail!\n");
}
//���������ź���
err_t
sys_mutex_new(sys_mutex_t *mutex)
{
  /* ���� sem */   
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
//ɾ�������ź���
void
sys_mutex_free(sys_mutex_t *mutex)
{
  vSemaphoreDelete(*mutex); 
}
//================================================sys_mutex_free=================================================

//================================================sys_mutex_set_invalid=================================================
//���û����ź�����Ч��
void
sys_mutex_set_invalid(sys_mutex_t *mutex)
{
  *mutex = SYS_MRTEX_NULL;  
}
//================================================sys_mutex_set_invalid=================================================

//================================================sys_mutex_lock=================================================
//��ȡ�����ź���������
void
sys_mutex_lock(sys_mutex_t *mutex)  
{
  xSemaphoreTake(*mutex,/* ��������� */
                 portMAX_DELAY); /* �ȴ�ʱ�� */
}
//================================================sys_mutex_lock=================================================


//================================================sys_mutex_unlock=================================================
//���������ź������ͷŻ����ź���
void
sys_mutex_unlock(sys_mutex_t *mutex)
{
  xSemaphoreGive( *mutex );
}
//================================================sys_mutex_unlock=================================================

//================================================sys_thread_new=================================================
//��װ������������������������
sys_thread_t
sys_thread_new(const char *name, lwip_thread_fn function, void *arg, int stacksize, int prio) 
{
  sys_thread_t handle = NULL;
  BaseType_t xReturn = pdPASS;
  /* �������� */
  xReturn = xTaskCreate((TaskFunction_t )function,  /* ������ں��� */
                        (const char*    )name,/* �������� */
                        (uint16_t       )stacksize,  /* ����ջ��С */
                        (void*          )arg,/* ������ں������� */
                        (UBaseType_t    )prio, /* ��������ȼ� */
                        (TaskHandle_t*  )&handle);/* ������ƿ�ָ�� */ 
  if(xReturn != pdPASS)
  {
    printf("[sys_arch]:create task fail!err:%#lx\n",xReturn);
    return NULL;
  }
  return handle;
}
//================================================sys_thread_new=================================================


//================================================sys_mbox_new=================================================
//��װ�������к���,��Ϣ�Ĵ�СΪsizeof(void *)  ����һ������
err_t
sys_mbox_new(sys_mbox_t *mbox, int size)
{
    /* ����Test_Queue */
  *mbox = xQueueCreate((UBaseType_t ) size,/* ��Ϣ���еĳ��� */
                       (UBaseType_t ) sizeof(void *));/* ��Ϣ�Ĵ�С */
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
//ɾ����Ϣ����
void
sys_mbox_free(sys_mbox_t *mbox)
{
  if( uxQueueMessagesWaiting( *mbox ) )
	{
		/* Line for breakpoint.  Should never break here!�ϵ��С������ж� */
		portNOP();
#if SYS_STATS
	    lwip_stats.sys.mbox.err++;
#endif /* SYS_STATS */

		// TODO notify the user of failure.
	}
  vQueueDelete(*mbox);  //ɾ��һ������
  
#if SYS_STATS
     --lwip_stats.sys.mbox.used;
#endif /* SYS_STATS */
}
//================================================sys_mbox_free=================================================


//================================================sys_mbox_valid=================================================
//�ж�mbox(��Ϣ����)��Ч����1.��Ч����0  
int sys_mbox_valid(sys_mbox_t *mbox)         
{      
  if (*mbox == SYS_MBOX_NULL) 
    return 0;
  else
    return 1;
}  
//================================================sys_mbox_valid=================================================

//================================================sys_mbox_set_invalid=================================================
//����mbox(��Ϣ����)��Ч
void
sys_mbox_set_invalid(sys_mbox_t *mbox) 
{
  *mbox = SYS_MBOX_NULL; 
}
//================================================sys_mbox_set_invalid=================================================


//================================================sys_mbox_post=================================================
//��װ��Ϣ���з��ͺ���������
void
sys_mbox_post(sys_mbox_t *q, void *msg) 
{
  while(xQueueSend( *q, /* ��Ϣ���еľ�� */
                    &msg,/* ���͵���Ϣ���� */
                    portMAX_DELAY) != pdTRUE); /* �ȴ�ʱ�� */
}
//================================================sys_mbox_post=================================================

//================================================sys_mbox_trypost=================================================
//��װ��Ϣ���з��ͺ���������
err_t
sys_mbox_trypost(sys_mbox_t *q, void *msg)
{
  if(xQueueSend(*q,&msg,0) == pdPASS)  //���Է���һ����Ϣ������������
    return ERR_OK;
  else
    return ERR_MEM;
}
//================================================sys_mbox_trypost=================================================

//================================================sys_mbox_trypost_fromisr=================================================
//��װ��Ϣ���з��ͺ���������
err_t
sys_mbox_trypost_fromisr(sys_mbox_t *q, void *msg)
{
  return sys_mbox_trypost(q, msg);
}
//================================================sys_mbox_trypost_fromisr=================================================



//================================================sys_arch_mbox_fetch=================================================
//��װ��Ϣ���ж�ȡ������timeout=0ʱ�����ȣ�timeout != 0ʱ�����õȴ�ʱ��Ϊtimeout
u32_t
sys_arch_mbox_fetch(sys_mbox_t *q, void **msg, u32_t timeout)
{
  void *dummyptr;
  u32_t wait_tick = 0;
  u32_t start_tick = 0 ;
  
  if ( msg == NULL )  //�����洢��Ϣ�ĵط��Ƿ���Ч
		msg = &dummyptr;
  
  //���Ȼ�ȡ��ʼ�ȴ��ź�����ʱ�ӽ���
  start_tick = sys_now();
  
  //timeout != 0����Ҫ��ms����ϵͳ��ʱ�ӽ���
  if(timeout != 0)
  {
    //��msת����ʱ�ӽ���
    wait_tick = timeout / portTICK_PERIOD_MS;
    if (wait_tick == 0)
      wait_tick = 1;
  }
  //һֱ����
  else
    wait_tick = portMAX_DELAY;
  
  //�ȴ��ɹ�������ȴ���ʱ�䣬����ͱ�ʾ�ȴ���ʱ
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
//��װ��Ϣ���ж�ȡ����������
u32_t
sys_arch_mbox_tryfetch(sys_mbox_t *q, void **msg)
{
	void *dummyptr;
	if ( msg == NULL )
		msg = &dummyptr;
  
  //�ȴ��ɹ�������ȴ���ʱ��
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
  tcpip_init(NULL, NULL);  //��ʼ��tcpip
  
  /* IP addresses initialization IP��ַ��ʼ��*/
  /* USER CODE BEGIN 0 */
#if LWIP_DHCP
  ip_addr_set_zero_ip4(&ipaddr);  //���
  ip_addr_set_zero_ip4(&netmask);
  ip_addr_set_zero_ip4(&gw);
#else
  IP4_ADDR(&ipaddr,IP_ADDR0,IP_ADDR1,IP_ADDR2,IP_ADDR3);
  IP4_ADDR(&netmask,NETMASK_ADDR0,NETMASK_ADDR1,NETMASK_ADDR2,NETMASK_ADDR3);
  IP4_ADDR(&gw,GW_ADDR0,GW_ADDR1,GW_ADDR2,GW_ADDR3);
#endif /* USE_DHCP */
  /* USER CODE END 0 */
  /* Initilialize the LwIP stack without RTOS 
  ��û��RTOS������³�ʼ��LwIP��ջ */
  /* add the network interface (IPv4/IPv6) without RTOS
  ��Ӳ���RTOS������ӿڣ�IPv4 / IPv6�� */
  struct netif *jing;
  jing = netif_add(&gnetif, &ipaddr, &netmask, &gw, NULL, &ethernetif_init, &tcpip_input); //
  if(jing == NULL)
    printf("����ӿ����ʧ��\n");

  /* Registers the default network interface
  ע��Ĭ������ӿ� */
  netif_set_default(&gnetif);

  if (netif_is_link_up(&gnetif)) //ѯ�������Ƿ���
  {
    /* When the netif is fully configured this function must be called
    ��ȫ����netif�󣬱�����ô˺��� */
		printf("\n ���ӽ����ɹ�\n");
    netif_set_up(&gnetif);
  }
  else
  {
    /* When the netif link is down this function must be called 
    ��netif���ӹر�ʱ��������ô˺���*/
		printf("\n ���ӽ���ʧ��\n");
    netif_set_down(&gnetif);
  }
  
#if LWIP_DHCP	   			//��ʹ����DHCP
  int err;
  /*  Creates a new DHCP client for this interface on the first call.
  Note: you must call dhcp_fine_tmr() and dhcp_coarse_tmr() at
  the predefined regular intervals after starting the client.
  You can peek in the netif->dhcp struct for the actual DHCP status.
  �ڵ�һ������ʱΪ�˽ӿڴ���һ���µ�DHCP�ͻ��ˡ�ע�⣺�����ͻ��˺󣬱�����Ԥ����Ķ��ڼ������dhcp_fine_tmr������dhcp_coarse_tmr������
  ��������netif-> dhcp�ṹ�п���ʵ�ʵ�DHCP״̬*/
  
  printf("�����̽�ʹ��DHCP��̬����IP��ַ,�������Ҫ����lwipopts.h�н�LWIP_DHCP����Ϊ0\n\n");
  
  err = dhcp_start(&gnetif);      //����dhcp
  if(err == ERR_OK)
    printf("lwip dhcp init success...\n\n");
  else
    printf("lwip dhcp init fail...\n\n");
  while(ip_addr_cmp(&(gnetif.ip_addr),&ipaddr))   //�ȴ�dhcp�����ip��Ч
  {
    //printf("gnetif.ip_addr=%d\tipaddr=%d",gnetif.ip_addr.addr,ipaddr.addr);
    vTaskDelay(1);
  } 
#endif
  printf("����IP��ַ��:%d.%d.%d.%d\n\n",  \
        ((gnetif.ip_addr.addr)&0x000000ff),       \
        (((gnetif.ip_addr.addr)&0x0000ff00)>>8),  \
        (((gnetif.ip_addr.addr)&0x00ff0000)>>16), \
        ((gnetif.ip_addr.addr)&0xff000000)>>24);
}


//================================================TCPIP_Init=================================================







