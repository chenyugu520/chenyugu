#include "bsp_usart_dma.h"

uint8_t SendBuff[SENDBUFF_SIZE+1];
///重定向c库函数printf到USART1
int fputc(int ch, FILE *f)
{
		/* 发送一个字节数据到USART1 */
		USART_SendData(DEBUG_USART, (uint8_t) ch);
		/* 等待发送完毕 */
		while (USART_GetFlagStatus(DEBUG_USART, USART_FLAG_TXE) == RESET);		
		return (ch);
}

///重定向c库函数scanf到USART1
int fgetc(FILE *f)
{
		/* 等待串口1输入数据 */
		while (USART_GetFlagStatus(DEBUG_USART, USART_FLAG_RXNE) == RESET);
		return (int)USART_ReceiveData(DEBUG_USART);
}
//===================================================================================================

//=========================================USART1==========================================================
/**
  * @brief  USART1 TX DMA 配置，外设到内存(USART1->DR)
  * @param  无
  * @retval 无
  */
void USART_DMA_Config(void)
{
  DMA_InitTypeDef DMA_InitStructure; 
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);											/*开启DMA时钟*/   
  DMA_DeInit(DMA2_Stream2);																				/* 复位初始化DMA数据流 */ 
  while (DMA_GetCmdStatus(DMA2_Stream2) != DISABLE)  {}						/* 确保DMA数据流复位完成 */

  DMA_InitStructure.DMA_Channel = DMA_Channel_4;  								/*usart1 tx对应dma2，通道4，数据流7*/	//DMA 数据流通道选择
  DMA_InitStructure.DMA_PeripheralBaseAddr = (USART1_BASE+0x04);	 					/*设置DMA源：串口数据寄存器地址*/
  DMA_InitStructure.DMA_Memory0BaseAddr = (u32)SendBuff;										/*内存地址(要传输的变量的指针)*/	
  //DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;									/*方向：从内存到外设*/	
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;										/*方向：从外设到内存*/
  DMA_InitStructure.DMA_BufferSize = SENDBUFF_SIZE; 												/*传输大小DMA_BufferSize=SENDBUFF_SIZE*/	  
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 					/*外设地址不增*/	
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;										/*内存地址自增*/
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;		/*外设数据单位*/
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;						/*内存数据单位 8bit*/
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;	 													/*DMA模式：不断循环*/
  DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; 										/*优先级：中*/	     
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;   									/*禁用FIFO*/     
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;    
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;   						/*存储器突发传输 16个节拍*/  
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;    		/*外设突发传输 1个节拍*/	   
  DMA_Init(DMA2_Stream2, &DMA_InitStructure);	 										/*配置DMA2的数据流7*/
  DMA_Cmd(DMA2_Stream2, ENABLE);																	/*使能DMA*/
  while(DMA_GetCmdStatus(DMA2_Stream2) != ENABLE){}    						/* 等待DMA数据流有效*/
}
/**
  * @brief	DMA+串口空闲中断接受任意长数据
  * @param  无
  * @retval 无
  */

#if 1
void USART1_Init(uint32_t bound)//DMA2_Stream2
{
	GPIO_InitTypeDef GPIO_Initstructure;
	USART_InitTypeDef USART_Initstructure;
	NVIC_InitTypeDef NVIC_Initstrcuture;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE );
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA ,ENABLE );
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);
	
  GPIO_Initstructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_Initstructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Initstructure.GPIO_OType = GPIO_OType_PP;
	GPIO_Initstructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//浮空
	GPIO_Initstructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOA,&GPIO_Initstructure);
	GPIO_Initstructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_Initstructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Initstructure.GPIO_OType = GPIO_OType_PP;
	GPIO_Initstructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Initstructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOA,&GPIO_Initstructure);
	
	USART_Initstructure.USART_BaudRate = bound;
	USART_Initstructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Initstructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Initstructure.USART_Parity = USART_Parity_No;
	USART_Initstructure.USART_StopBits = USART_StopBits_1;
	USART_Initstructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1,&USART_Initstructure);
	
	NVIC_Initstrcuture.NVIC_IRQChannel = USART1_IRQn;
	NVIC_Initstrcuture.NVIC_IRQChannelPreemptionPriority=8;
	NVIC_Initstrcuture.NVIC_IRQChannelSubPriority =1;		
	NVIC_Initstrcuture.NVIC_IRQChannelCmd = ENABLE;		
	NVIC_Init(&NVIC_Initstrcuture);	
	
//	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启接收中断
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);//开启空闲中断
	USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);//开启DMA接收
	USART_Cmd(USART1, ENABLE);

	USART_DMA_Config();
}
//开启一次DMA传输
void DMA_Enable(DMA_Stream_TypeDef *DMA_Streamx,u16 ndtr)
{
	DMA_Cmd(DMA_Streamx, DISABLE);                      //先关闭DMA,才能设置它
	while (DMA_GetCmdStatus(DMA_Streamx) != DISABLE){}	//等待传输结束	
	DMA_SetCurrDataCounter(DMA_Streamx,ndtr);          //设置传输数据长度 
	DMA_Cmd(DMA_Streamx, ENABLE);                      //开启DMA
}	  

void USART1_IRQHandler(void) 
{
	uint8_t rc_tmp =0;
	rc_tmp=rc_tmp;
	uint16_t rc_len=0;
	rc_len=rc_len;
	int i=0;
	if(USART_GetITStatus(USART1,USART_IT_IDLE)!=RESET)
	{ 
		rc_tmp=USART1->SR;
		rc_tmp=USART1->DR;//软件序列清除IDLE标志位
		DMA_Cmd(DMA2_Stream2, DISABLE);//关闭DMA，准备重新配置
		DMA_ClearITPendingBit(DMA2_Stream2, DMA_IT_TCIF2);	// Clear Transfer Complete flag
		DMA_ClearITPendingBit(DMA2_Stream2, DMA_IT_TEIF2);	// Clear Transfer error flag	
		rc_len = SENDBUFF_SIZE - DMA_GetCurrDataCounter(DMA2_Stream2);//计算接收数据长度
		while(rc_len)
		{
			printf("%X",SendBuff[i]);
			i++;
			rc_len--;
		}
		printf("\n");
      
    //Data_Decode(USART1_Rx_Buffer);//解码收到的数据
	}
	DMA_Enable(DMA2_Stream2,SENDBUFF_SIZE);//开启下一次DMA接收

}

#endif
//=====================================USART1==============================================================

//========================================memcpy===========================================================
#if 1 //memcpy
/**
  * @brief DMA 配置，内存到内存(USART1->DR)
  * @param  无
  * @retval 无
  */
void memcpy_DMA_Config(char *Peripheralbuf,char *Memory0BaseAddr,int size)
{
	DMA_InitTypeDef  DMA_InitStructure;
  RCC_AHB1PeriphClockCmd(DEBUG_MEMCPY_DMA_CLK, ENABLE);										/* 使能DMA时钟 */
  DMA_DeInit(DEBUG_MEMCPY_DMA_STREAM);																		/* 复位初始化DMA数据流 */
  while (DMA_GetCmdStatus(DEBUG_MEMCPY_DMA_STREAM) != DISABLE){} 					/* 确保DMA数据流复位完成 */
		
  DMA_InitStructure.DMA_Channel = DEBUG_MEMCPY_DMA_CHANNEL; 							/* DMA数据流通道选择 */ 
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)Peripheralbuf; 		/* 源数据地址 */
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)Memory0BaseAddr;			/* 目标地址 */
  DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToMemory;											/* 存储器到存储器模式 */
  DMA_InitStructure.DMA_BufferSize = (uint32_t)size;											/* 数据数目 */
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;  				/* 使能自动递增功能 */
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;									/* 使能自动递增功能 */
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;	/* 源数据是字大小(8位) */
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;					/* 目标数据也是字大小(8位) */ 
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal; 													/* 一次传输模式，存储器到存储器模式不能使用循环传输 */  
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;											/* DMA数据流优先级为高 */  
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;									/* 禁用FIFO模式 */     
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full; 
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single; 						/* 单次模式 */
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;  		/* 单次模式 */
  DMA_Init(DEBUG_MEMCPY_DMA_STREAM, &DMA_InitStructure); 									/* 完成DMA数据流参数配置 */
  DMA_ClearFlag(DEBUG_MEMCPY_DMA_STREAM,DMA_FLAG_TCIF7); 									/* 清除DMA数据流传输完成标志位 */
  DMA_Cmd(DEBUG_MEMCPY_DMA_STREAM, ENABLE); 															/* 使能DMA数据流，开始DMA数据传输 */
  while ((DMA_GetCmdStatus(DEBUG_MEMCPY_DMA_STREAM) != ENABLE)){}					/* 等待DMA数据流有效*/
}


int jiang_memcpy(char *Peripheralbuf,char *Memory0BaseAddr,int size)
{
	int i=0;
	//printf("Peripheralbuf=%s\n",Peripheralbuf);
	//printf("Memory0BaseAddr=%s\n",Memory0BaseAddr);
	//printf("size=%d\n",size);
	memcpy_DMA_Config(Peripheralbuf,Memory0BaseAddr,size);
	/* 等待 DMA 传输完成 */
	while (DMA_GetFlagStatus(DEBUG_MEMCPY_DMA_STREAM,DMA_FLAG_TCIF7)==DISABLE) {}
		
	while(size)
	{
		size--;
		if(Peripheralbuf[i] != Memory0BaseAddr[i])
		{
			printf("复制失败%c\n",Memory0BaseAddr[i]);
		}
		i++;
	}
	//Memory0BaseAddr[i] = '\0';
	//printf("Peripheralbuf=%s\n",Peripheralbuf);
	//printf("Memory0BaseAddr=%s\n",Memory0BaseAddr);
	//printf("i=%d\n",i);
	return 0;
}

#endif
//==========================================memcpy=========================================================
//===================================================================================================
/*********************************************END OF FILE**********************/
