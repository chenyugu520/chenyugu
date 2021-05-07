
#include "server.h"

#include "lwip/opt.h"

#include "lwip/sys.h"
#include "lwip/api.h"


static void server(void *thread_param)
{
  struct netconn *conn, *accept_conn;
  int ret;
  ip4_addr_t ipaddr;
  
  uint8_t send_buf[]= "This is a TCP server test...\n";
  
  printf("������IP��ַ:%d.%d.%d.%d \t �˿ں�:%d\n\n",      \
          IP_ADDR0,IP_ADDR1,IP_ADDR2,IP_ADDR3,LOCAL_PORT);
 
  while(1)
  {
    conn = netconn_new(NETCONN_TCP); //����TCP���ӽṹ
    if (conn == NULL)
    {
      printf("create conn failed!\n");
      vTaskDelay(10);
      continue;
    }
    
    IP4_ADDR(&ipaddr,IP_ADDR0,IP_ADDR1,IP_ADDR2,IP_ADDR3); 

    ret = netconn_bind(conn,&ipaddr,LOCAL_PORT); //����
    if (ret == -1)
    {
        printf("bind failed!\n");
        netconn_close(conn);
        vTaskDelay(10);
        continue;
    }
    else
    {
        printf("Connect to server successful!\n");
    }
    ret = netconn_listen(conn);
     if (ret == -1)
    {
        printf("listen failed!\n");
        netconn_close(conn);
        vTaskDelay(10);
        continue;
    }
    else
    {
        printf("listen to server successful!\n");
    }
    while(1)
    {
        printf("�ȴ�����\n");
        ret = netconn_accept(conn,&accept_conn);
        if(ret != -1)
        {
            printf("���ӳɹ�\n");
        }
         while (ret != -1)
        {
        ret = netconn_write(accept_conn,send_buf,sizeof(send_buf),0);
    
        vTaskDelay(10);
        }
    }
   
  }

}

void
server_init(void)
{
  sys_thread_new("server", server, NULL, 512, 4);
}
