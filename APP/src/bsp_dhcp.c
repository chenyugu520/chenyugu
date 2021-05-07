#include "lwip/opt.h"
#include "lwip/arch.h"
#include "lwip/api.h"
#include "lwip/dhcp.h"
#include "lwip/inet.h"

#include "bsp_dhcp.h"

#if LWIP_NETCONN

#define MAX_BUFFER_LEN 256

char sendbuf[MAX_BUFFER_LEN];

extern struct netif gnetif;

 void dhcp_thread(void *arg)
{
	struct netconn *conn;
	ip4_addr_t ipaddr;
  int ret;
	int strlen = 0;
  
  printf("目地IP地址:%d.%d.%d.%d \t 端口号:%d\n\n",      \
          DEST_IP_ADDR0,DEST_IP_ADDR1,DEST_IP_ADDR2,DEST_IP_ADDR3,DEST_PORT);
  
  printf("请将电脑上位机设置为TCP Server.在User/arch/sys_arch.h文件中将目标IP地址修改为您电脑上的IP地址\n\n");
  
  printf("修改对应的宏定义:DEST_IP_ADDR0,DEST_IP_ADDR1,DEST_IP_ADDR2,DEST_IP_ADDR3,DEST_PORT\n\n");
  
	while(1)
	{
    conn = netconn_new(NETCONN_TCP);
    if (conn == NULL)
    {
      printf("create conn failed!\n");
      vTaskDelay(10);
      continue;
    }

    printf("create conn success...\n");
    
    //构造服务器IP地址
    IP4_ADDR(&ipaddr,DEST_IP_ADDR0,DEST_IP_ADDR1,DEST_IP_ADDR2,DEST_IP_ADDR3); 			
    
    ret = netconn_connect(conn,&ipaddr,DEST_PORT);	        //连接服务器，端口号5001
    if (ret == -1)
    {
      printf("Connect failed!\n");
      netconn_close(conn);
      vTaskDelay(10);
      continue;
    }
    
    printf("Connect to server successful!\n");
    
    strlen = sprintf(sendbuf,"A LwIP client Using DHCP Address: %s\n", \
    ipaddr_ntoa((ip_addr_t *)&(gnetif.ip_addr)));
    
    while(1)
    {
      printf("%s",sendbuf);
      ret=netconn_write(conn,sendbuf, strlen, NETCONN_NOCOPY);
      if (ret == ERR_OK)
      {
         printf("write success...\n");
      }
      vTaskDelay(1000); 
    }
    
//    printf("Connection failed \n");
//    netconn_close(conn); 						//关闭连接
//    netconn_delete(conn);						//删除连接结构
  }
}


void dhcp_netconn_init()
{
  //sys_thread_new("dhcp_thread", dhcp_thread, NULL, 128, 1);
	sys_thread_t handle = NULL;
  BaseType_t xReturn = pdPASS;
  /* 创建任务 */
  xReturn = xTaskCreate((TaskFunction_t )dhcp_thread,  /* 任务入口函数 */
                        (const char*    )"DHCP",/* 任务名字 */
                        (uint16_t       )256,  /* 任务栈大小 */
                        (void*          )NULL,/* 任务入口函数参数 */
                        (UBaseType_t    )2, /* 任务的优先级 */
                        (TaskHandle_t*  )&handle);/* 任务控制块指针 */ 
  if(xReturn != pdPASS)
  {
    printf("[sys_arch]:create task fail!err:%#lx\n",xReturn);
   
  }
}

#endif /* LWIP_NETCONN*/
