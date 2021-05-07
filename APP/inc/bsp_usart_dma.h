#ifndef __USART_DMA_H
#define	__USART_DMA_H

#include "stm32f4xx.h"
#include <stdio.h>
#define ECHO_EVENT      			(0x01 << 2)				// 设置事件掩码的位1
#define SENDBUFF_SIZE          1024				//发送的数据量
//================================USART1=======================================================
//USART1_RX
#define DEBUG_USART                       USART1
#define DEBUG_USART_CLK                   RCC_APB2Periph_USART1
#define DEBUG_USART_RX_GPIO_PORT          GPIOA
#define DEBUG_USART_RX_GPIO_CLK           RCC_AHB1Periph_GPIOA
#define DEBUG_USART_RX_PIN                GPIO_Pin_10
#define DEBUG_USART_RX_AF                 GPIO_AF_USART1
#define DEBUG_USART_RX_SOURCE             GPIO_PinSource10
//USART1_TX
#define DEBUG_USART_TX_GPIO_PORT          GPIOA
#define DEBUG_USART_TX_GPIO_CLK           RCC_AHB1Periph_GPIOA
#define DEBUG_USART_TX_PIN                GPIO_Pin_9
#define DEBUG_USART_TX_AF                 GPIO_AF_USART1
#define DEBUG_USART_TX_SOURCE             GPIO_PinSource9
#define DEBUG_USART_BAUDRATE              115200
//DMA_USART1
#define DEBUG_USART_DR_BASE               (USART1_BASE+0x04)		
#define DEBUG_USART_DMA_CLK               RCC_AHB1Periph_DMA2	
#define DEBUG_USART_DMA_CHANNEL           DMA_Channel_4 //DMA数据通道
//#define DEBUG_USART_DMA_STREAM            DMA2_Stream7   //DMA数据流(DMA写USART1)
#define DEBUG_USART_DMA_STREAM            DMA2_Stream2   //DMA数据流(DMA读USART1)
//================================USART1=======================================================

//DMA_MEMCPY
#define DEBUG_MEMCPY_DMA_CLK               RCC_AHB1Periph_DMA2	
#define DEBUG_MEMCPY_DMA_CHANNEL           DMA_Channel_7 //DMA数据通道
#define DEBUG_MEMCPY_DMA_STREAM            DMA2_Stream7  //DMA数据流


void USART1_Init(uint32_t bound);
void memcpy_DMA_Config(char *Peripheralbuf,char *Memory0BaseAddr,int size);
int jiang_memcpy(char *Peripheralbuf,char *Memory0BaseAddr,int size);
#endif /* __USART1_H */
