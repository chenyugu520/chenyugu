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
#define  MQTT_QUEUE_LEN    4   /* ���еĳ��ȣ����ɰ������ٸ���Ϣ */
#define  MQTT_QUEUE_SIZE   4   /* ������ÿ����Ϣ��С���ֽڣ� */
QueueHandle_t MQTT_Data_Queue =NULL;

static void led_task(void *pvParameters);
static void TCPIP_task(void *pvParameters);
static void start_task(void *pvParameters);
//static void DHT11_Task(void* parameter);
static TaskHandle_t StartTask_Handler;
static TaskHandle_t LEDTask_Handler;
static TaskHandle_t TCPIPTask_Handler;
//static TaskHandle_t DHT11Task_Handler;

/*��ȡMCU��ΨһID*/
void GetSTM32MCUID(void)
{
	u32 id[3] = {0};	
	id[0]=*(uint32_t*)(0x1FFF7A10);
	id[1]=*(uint32_t*)(0x1FFF7A10+4);
	id[2]=*(uint32_t*)(0x1FFF7A10+8);
	printf("id = %d %d %d",id[0],id[1],id[2]);
}
//�õ�8720���ٶ�ģʽ
//����ֵ:
//001:10M��˫��
//101:10Mȫ˫��
//010:100M��˫��
//110:100Mȫ˫��
//����:����.
u8 LAN8720_Get_Speed(void)
{
	u8 speed;
	speed=((ETH_ReadPHYRegister(0x00,31)&0x1C)>>2); //��LAN8720��31�żĴ����ж�ȡ�����ٶȺ�˫��ģʽ
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
	/* ����Test_Queue */
//  MQTT_Data_Queue = xQueueCreate((UBaseType_t ) MQTT_QUEUE_LEN,/* ��Ϣ���еĳ��� */
//                                 (UBaseType_t ) MQTT_QUEUE_SIZE);/* ��Ϣ�Ĵ�С */
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
  * @ ������  �� Test1_Task
  * @ ����˵���� Test1_Task��������
  * @ ����    ��   
  * @ ����ֵ  �� ��
  ********************************************************************/
//static void DHT11_Task(void* parameter)
//{	
//  uint8_t res;
//  BaseType_t xReturn = pdPASS;
//  DHT11_Data_TypeDef DHT11_Data;
//  while (1)
//  {
//    //taskENTER_CRITICAL();           //�����ٽ���
//    res = Read_DHT11(&DHT11_Data);
//    //taskEXIT_CRITICAL();            //�˳��ٽ���
//    
//    if(SUCCESS == res)
//    {
//      xReturn = xQueueSend( MQTT_Data_Queue, /* ��Ϣ���еľ�� */
//                            &DHT11_Data,/* ���͵���Ϣ���� */
//                            0 );        /* �ȴ�ʱ�� 0 */
//      if(xReturn == pdTRUE)
//        printf("������Ϣ���ݳɹ�");
//    }
//    vTaskDelay(1000);/* ��ʱ1000��tick */
//  }
//}
//===================================================================================================
//===================================================================================================
