#include "bsp_usart_dma.h"

uint8_t SendBuff[SENDBUFF_SIZE+1];
///�ض���c�⺯��printf��USART1
int fputc(int ch, FILE *f)
{
		/* ����һ���ֽ����ݵ�USART1 */
		USART_SendData(DEBUG_USART, (uint8_t) ch);
		/* �ȴ�������� */
		while (USART_GetFlagStatus(DEBUG_USART, USART_FLAG_TXE) == RESET);		
		return (ch);
}

///�ض���c�⺯��scanf��USART1
int fgetc(FILE *f)
{
		/* �ȴ�����1�������� */
		while (USART_GetFlagStatus(DEBUG_USART, USART_FLAG_RXNE) == RESET);
		return (int)USART_ReceiveData(DEBUG_USART);
}
//===================================================================================================

//=========================================USART1==========================================================
/**
  * @brief  USART1 TX DMA ���ã����赽�ڴ�(USART1->DR)
  * @param  ��
  * @retval ��
  */
void USART_DMA_Config(void)
{
  DMA_InitTypeDef DMA_InitStructure; 
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);											/*����DMAʱ��*/   
  DMA_DeInit(DMA2_Stream2);																				/* ��λ��ʼ��DMA������ */ 
  while (DMA_GetCmdStatus(DMA2_Stream2) != DISABLE)  {}						/* ȷ��DMA��������λ��� */

  DMA_InitStructure.DMA_Channel = DMA_Channel_4;  								/*usart1 tx��Ӧdma2��ͨ��4��������7*/	//DMA ������ͨ��ѡ��
  DMA_InitStructure.DMA_PeripheralBaseAddr = (USART1_BASE+0x04);	 					/*����DMAԴ���������ݼĴ�����ַ*/
  DMA_InitStructure.DMA_Memory0BaseAddr = (u32)SendBuff;										/*�ڴ��ַ(Ҫ����ı�����ָ��)*/	
  //DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;									/*���򣺴��ڴ浽����*/	
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;										/*���򣺴����赽�ڴ�*/
  DMA_InitStructure.DMA_BufferSize = SENDBUFF_SIZE; 												/*�����СDMA_BufferSize=SENDBUFF_SIZE*/	  
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 					/*�����ַ����*/	
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;										/*�ڴ��ַ����*/
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;		/*�������ݵ�λ*/
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;						/*�ڴ����ݵ�λ 8bit*/
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;	 													/*DMAģʽ������ѭ��*/
  DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; 										/*���ȼ�����*/	     
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;   									/*����FIFO*/     
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;    
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;   						/*�洢��ͻ������ 16������*/  
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;    		/*����ͻ������ 1������*/	   
  DMA_Init(DMA2_Stream2, &DMA_InitStructure);	 										/*����DMA2��������7*/
  DMA_Cmd(DMA2_Stream2, ENABLE);																	/*ʹ��DMA*/
  while(DMA_GetCmdStatus(DMA2_Stream2) != ENABLE){}    						/* �ȴ�DMA��������Ч*/
}
/**
  * @brief	DMA+���ڿ����жϽ������ⳤ����
  * @param  ��
  * @retval ��
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
	GPIO_Initstructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//����
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
	
//	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//���������ж�
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);//���������ж�
	USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);//����DMA����
	USART_Cmd(USART1, ENABLE);

	USART_DMA_Config();
}
//����һ��DMA����
void DMA_Enable(DMA_Stream_TypeDef *DMA_Streamx,u16 ndtr)
{
	DMA_Cmd(DMA_Streamx, DISABLE);                      //�ȹر�DMA,����������
	while (DMA_GetCmdStatus(DMA_Streamx) != DISABLE){}	//�ȴ��������	
	DMA_SetCurrDataCounter(DMA_Streamx,ndtr);          //���ô������ݳ��� 
	DMA_Cmd(DMA_Streamx, ENABLE);                      //����DMA
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
		rc_tmp=USART1->DR;//����������IDLE��־λ
		DMA_Cmd(DMA2_Stream2, DISABLE);//�ر�DMA��׼����������
		DMA_ClearITPendingBit(DMA2_Stream2, DMA_IT_TCIF2);	// Clear Transfer Complete flag
		DMA_ClearITPendingBit(DMA2_Stream2, DMA_IT_TEIF2);	// Clear Transfer error flag	
		rc_len = SENDBUFF_SIZE - DMA_GetCurrDataCounter(DMA2_Stream2);//����������ݳ���
		while(rc_len)
		{
			printf("%X",SendBuff[i]);
			i++;
			rc_len--;
		}
		printf("\n");
      
    //Data_Decode(USART1_Rx_Buffer);//�����յ�������
	}
	DMA_Enable(DMA2_Stream2,SENDBUFF_SIZE);//������һ��DMA����

}

#endif
//=====================================USART1==============================================================

//========================================memcpy===========================================================
#if 1 //memcpy
/**
  * @brief DMA ���ã��ڴ浽�ڴ�(USART1->DR)
  * @param  ��
  * @retval ��
  */
void memcpy_DMA_Config(char *Peripheralbuf,char *Memory0BaseAddr,int size)
{
	DMA_InitTypeDef  DMA_InitStructure;
  RCC_AHB1PeriphClockCmd(DEBUG_MEMCPY_DMA_CLK, ENABLE);										/* ʹ��DMAʱ�� */
  DMA_DeInit(DEBUG_MEMCPY_DMA_STREAM);																		/* ��λ��ʼ��DMA������ */
  while (DMA_GetCmdStatus(DEBUG_MEMCPY_DMA_STREAM) != DISABLE){} 					/* ȷ��DMA��������λ��� */
		
  DMA_InitStructure.DMA_Channel = DEBUG_MEMCPY_DMA_CHANNEL; 							/* DMA������ͨ��ѡ�� */ 
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)Peripheralbuf; 		/* Դ���ݵ�ַ */
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)Memory0BaseAddr;			/* Ŀ���ַ */
  DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToMemory;											/* �洢�����洢��ģʽ */
  DMA_InitStructure.DMA_BufferSize = (uint32_t)size;											/* ������Ŀ */
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;  				/* ʹ���Զ��������� */
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;									/* ʹ���Զ��������� */
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;	/* Դ�������ִ�С(8λ) */
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;					/* Ŀ������Ҳ���ִ�С(8λ) */ 
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal; 													/* һ�δ���ģʽ���洢�����洢��ģʽ����ʹ��ѭ������ */  
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;											/* DMA���������ȼ�Ϊ�� */  
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;									/* ����FIFOģʽ */     
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full; 
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single; 						/* ����ģʽ */
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;  		/* ����ģʽ */
  DMA_Init(DEBUG_MEMCPY_DMA_STREAM, &DMA_InitStructure); 									/* ���DMA�������������� */
  DMA_ClearFlag(DEBUG_MEMCPY_DMA_STREAM,DMA_FLAG_TCIF7); 									/* ���DMA������������ɱ�־λ */
  DMA_Cmd(DEBUG_MEMCPY_DMA_STREAM, ENABLE); 															/* ʹ��DMA����������ʼDMA���ݴ��� */
  while ((DMA_GetCmdStatus(DEBUG_MEMCPY_DMA_STREAM) != ENABLE)){}					/* �ȴ�DMA��������Ч*/
}


int jiang_memcpy(char *Peripheralbuf,char *Memory0BaseAddr,int size)
{
	int i=0;
	//printf("Peripheralbuf=%s\n",Peripheralbuf);
	//printf("Memory0BaseAddr=%s\n",Memory0BaseAddr);
	//printf("size=%d\n",size);
	memcpy_DMA_Config(Peripheralbuf,Memory0BaseAddr,size);
	/* �ȴ� DMA ������� */
	while (DMA_GetFlagStatus(DEBUG_MEMCPY_DMA_STREAM,DMA_FLAG_TCIF7)==DISABLE) {}
		
	while(size)
	{
		size--;
		if(Peripheralbuf[i] != Memory0BaseAddr[i])
		{
			printf("����ʧ��%c\n",Memory0BaseAddr[i]);
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
