
#include "client.h"

#include "lwip/opt.h"

#include "lwip/sys.h"
#include "lwip/api.h"


static void client(void *thread_param)
{
  struct netconn *conn;
  int ret;
  ip4_addr_t ipaddr;
  
  uint8_t send_buf[]= "This is a TCP Client test...\n";
  
  printf("目地IP地址:%d.%d.%d.%d \t 端口号:%d\n\n",      \
          DEST_IP_ADDR0,DEST_IP_ADDR1,DEST_IP_ADDR2,DEST_IP_ADDR3,DEST_PORT);
 
  while(1)
  {
    conn = netconn_new(NETCONN_TCP); //创建TCP连接结构
    if (conn == NULL)
    {
      printf("create conn failed!\n");
      vTaskDelay(10);
      continue;
    }
    
    IP4_ADDR(&ipaddr,DEST_IP_ADDR0,DEST_IP_ADDR1,DEST_IP_ADDR2,DEST_IP_ADDR3); 

    ret = netconn_connect(conn,&ipaddr,DEST_PORT); //连接
    if (ret == -1)
    {
        printf("Connect failed!\n");
        netconn_close(conn);
        vTaskDelay(10);
        continue;
    }

    printf("Connect to server successful!\n");
     
    while (1)
    {
      ret = netconn_write(conn,send_buf,sizeof(send_buf),0);
   
      vTaskDelay(1000);
    }
  }

}

void
client_init(void)
{
  sys_thread_new("client", client, NULL, 512, 4);
}
