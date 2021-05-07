#include "main.h"
#include "LAN8742A.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "list.h"
#include "bsp_usart_dma.h"
#include "bsp_led.h"
#include "bsp_dhcp.h"
#include "client.h"
#include "core_delay.h" 
#include "adc.h"
#include "bsp_dht11.h"
#include "lwip/apps/mqtt.h"
#include "bsp_exti.h"
#include "bsp_TIM6.h"
//#include "server.h"
#include "mqttclient.h"
#if !NO_SYS
#include "sys_arch.h"
#endif
#define  MQTT_QUEUE_LEN    4   /* 队列的长度，最大可包含多少个消息 */
#define  MQTT_QUEUE_SIZE   4   /* 队列中每个消息大小（字节） */
QueueHandle_t MQTT_Data_Queue =NULL;

static void led_task(void *pvParameters);
static void TCPIP_task(void *pvParameters);
static void start_task(void *pvParameters);
//static void DHT11_Task(void* parameter);
static TaskHandle_t StartTask_Handler;
static TaskHandle_t LEDTask_Handler;
static TaskHandle_t TCPIPTask_Handler;
//static TaskHandle_t DHT11Task_Handler;

/*获取MCU的唯一ID*/
void GetSTM32MCUID(void)
{
	u32 id[3] = {0};	
	id[0]=*(uint32_t*)(0x1FFF7A10);
	id[1]=*(uint32_t*)(0x1FFF7A10+4);
	id[2]=*(uint32_t*)(0x1FFF7A10+8);
	printf("id = %d %d %d",id[0],id[1],id[2]);
}
//得到8720的速度模式
//返回值:
//001:10M半双工
//101:10M全双工
//010:100M半双工
//110:100M全双工
//其他:错误.
u8 LAN8720_Get_Speed(void)
{
	u8 speed;
	speed=((ETH_ReadPHYRegister(0x00,31)&0x1C)>>2); //从LAN8720的31号寄存器中读取网络速度和双工模式
	return speed;
}

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	USART1_Init(115200);
	//GetSTM32MCUID();
	//EXTI_SR501_Config();
	//Adc_Init();
	LED_GPIO_Config();
	CPU_TS_TmrInit();
	//DHT11_GPIO_Config();
	ETH_BSP_Config();
	TCPIP_Init();
	//TIMx_Configuration();
	vTraceEnable(TRC_START);
	/* 创建Test_Queue */
//  MQTT_Data_Queue = xQueueCreate((UBaseType_t ) MQTT_QUEUE_LEN,/* 消息队列的长度 */
//                                 (UBaseType_t ) MQTT_QUEUE_SIZE);/* 消息的大小 */
	//mqtt_thread_init();
	//client_init();
	xTaskCreate(start_task,            						
							"start_task",          			
							configMINIMAL_STACK_SIZE * 2,   
							NULL,                  			
							1,       						
							&StartTask_Handler);   			
	vTaskStartScheduler();          						
} 
//===================================================================================================

//===================================================================================================
void start_task(void *pvParameters)
{
	taskENTER_CRITICAL();           							
	xTaskCreate(led_task,     									
							"led_task",   						
							configMINIMAL_STACK_SIZE * 2,		
							NULL,								
							2,									
							&LEDTask_Handler); 					
	xTaskCreate(TCPIP_task,     								
							"TCPIP_task",   					
							configMINIMAL_STACK_SIZE * 2,		
							NULL,								
							2,									
							&TCPIPTask_Handler);
//xTaskCreate(DHT11_Task,     								
//							"DHT11_Task",   					
//							configMINIMAL_STACK_SIZE * 2,		
//							NULL,								
//							1,									
//							&DHT11Task_Handler); 		
	vTaskDelete(StartTask_Handler); 							
	taskEXIT_CRITICAL();            							
	vTaskDelete(NULL);
}
//===================================================================================================

//===================================================================================================
void led_task(void *pvParameters)
{
	while(1)
	{
		GPIO_ResetBits(GPIOC, GPIO_Pin_3);
		vTaskDelay(500);
		GPIO_SetBits(GPIOC, GPIO_Pin_3);
		vTaskDelay(500);
		//int adc=Get_Adc_Average(ADC_Channel_5,20);
		//printf("adc=%d\n",adc);
	}
} 
static void TCPIP_task(void *pvParameters)
{
	ETH_BSP_Config();
	TCPIP_Init();
	while(1)
	{
		vTaskDelay(1000);
	}
}
/**********************************************************************
  * @ 函数名  ： Test1_Task
  * @ 功能说明： Test1_Task任务主体
  * @ 参数    ：   
  * @ 返回值  ： 无
  ********************************************************************/
//static void DHT11_Task(void* parameter)
//{	
//  uint8_t res;
//  BaseType_t xReturn = pdPASS;
//  DHT11_Data_TypeDef DHT11_Data;
//  while (1)
//  {
//    //taskENTER_CRITICAL();           //进入临界区
//    res = Read_DHT11(&DHT11_Data);
//    //taskEXIT_CRITICAL();            //退出临界区
//    
//    if(SUCCESS == res)
//    {
//      xReturn = xQueueSend( MQTT_Data_Queue, /* 消息队列的句柄 */
//                            &DHT11_Data,/* 发送的消息内容 */
//                            0 );        /* 等待时间 0 */
//      if(xReturn == pdTRUE)
//        printf("发送消息数据成功");
//    }
//    vTaskDelay(1000);/* 延时1000个tick */
//  }
//}
//===================================================================================================
//===================================================================================================
