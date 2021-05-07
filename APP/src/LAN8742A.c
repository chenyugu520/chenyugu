/**
  ******************************************************************************
  * @file    stm32f4x7_eth_bsp.c
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    31-July-2013 
  * @brief   STM32F4x7 Ethernet hardware configuration.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "lwip/opt.h"
#include "lwip/netif.h"
#include "lwip/ip_addr.h"
#include "lwip/dhcp.h"
#include "stm32f4x7_eth.h"
#include "LAN8742A.h"
#include "main.h"
//#include "netconf.h"
#if !NO_SYS
#include "sys_arch.h"
#endif
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
ETH_InitTypeDef ETH_InitStructure;
__IO uint32_t  EthStatus = 0;
__IO uint8_t EthLinkStatus = 0;
extern struct netif gnetif;
#ifdef USE_DHCP
extern __IO uint8_t DHCP_state;
#endif /* LWIP_DHCP */

/* Private function prototypes -----------------------------------------------*/
static void ETH_GPIO_Config(void);
static void ETH_NVIC_Config(void);
static void ETH_MACDMA_Config(void);

/* Private functions ---------------------------------------------------------*/
/* Bit 2 from Basic Status Register in PHY */
#define GET_PHY_LINK_STATUS()		(ETH_ReadPHYRegister(LAN8742A_PHY_ADDRESS, PHY_BSR) & 0x00000004)

/**
  * @brief  ETH_BSP_Config
  * @param  None
  * @retval None
  */
void ETH_BSP_Config(void)
{
  /* 以太网引脚配置GPIO端口 */
  ETH_GPIO_Config();

  /* 为以太网配置NVIC */
  ETH_NVIC_Config();

  /* 配置以太网MAC / DMA */
  ETH_MACDMA_Config();

  /* 获取以太网链接状态*/
  if(GET_PHY_LINK_STATUS())
  {
    EthStatus |= ETH_LINK_FLAG;
  }
}
////得到8720的速度模式
////返回值:
////001:10M半双工
////101:10M全双工
////010:100M半双工
////110:100M全双工
////其他:错误.
//u8 LAN8720_Get_Speed(void)
//{
//	u8 speed;
//	speed=((ETH_ReadPHYRegister(0x00,31)&0x1C)>>2); //从LAN8720的31号寄存器中读取网络速度和双工模式
//	return speed;
//}

/**
  * @brief  Configures the Ethernet Interface
  * 配置以太网接口
  * @param  None
  * @retval None
  */
static void ETH_MACDMA_Config(void)
{
 
  //使能以太网MAC以及MAC接收和发送时钟
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_ETH_MAC | RCC_AHB1Periph_ETH_MAC_Tx | RCC_AHB1Periph_ETH_MAC_Rx, ENABLE);
  ETH_DeInit(); //AHB总线重启以太网
  ETH_SoftwareReset();//软件重启网络
  while (ETH_GetSoftwareResetStatus() == SET);//等待软件重启网络完成 
  ETH_StructInit(&ETH_InitStructure);//初始化网络为默认值  
//#if 1
  /* Fill ETH_InitStructure parametrs */
  /*------------------------   MAC   -----------------------------------*/
  ETH_InitStructure.ETH_AutoNegotiation = ETH_AutoNegotiation_Enable;                   //开启PHY的自适应
 
// //ETH_InitStructure.ETH_AutoNegotiation = ETH_AutoNegotiation_Disable;
//  //ETH_InitStructure.ETH_Speed = ETH_Speed_100M;
//  //ETH_InitStructure.ETH_Mode = ETH_Mode_FullDuplex;//全双工模式
//  ETH_InitStructure.ETH_LoopbackMode = ETH_LoopbackMode_Disable;                        //关闭反馈
//  ETH_InitStructure.ETH_RetryTransmission = ETH_RetryTransmission_Disable;              //关闭重传功能
//  ETH_InitStructure.ETH_AutomaticPadCRCStrip = ETH_AutomaticPadCRCStrip_Disable;        //关闭自动去除PDA/CRC功能     
//  ETH_InitStructure.ETH_ReceiveAll = ETH_ReceiveAll_Disable;                             //关闭接收所有的帧
//  ETH_InitStructure.ETH_BroadcastFramesReception = ETH_BroadcastFramesReception_Enable;   //允许接收所有广播帧
//  ETH_InitStructure.ETH_PromiscuousMode = ETH_PromiscuousMode_Disable;                    //关闭混合模式的地址过滤 
//  ETH_InitStructure.ETH_MulticastFramesFilter = ETH_MulticastFramesFilter_Perfect;        //对于组播地址使用完美地址过滤 
//  ETH_InitStructure.ETH_UnicastFramesFilter = ETH_UnicastFramesFilter_Perfect;             //对单播地址使用完美地址过滤 
//#ifdef CHECKSUM_BY_HARDWARE
//  ETH_InitStructure.ETH_ChecksumOffload = ETH_ChecksumOffload_Enable;//开启ipv4和TCP/UDP/ICMP的帧校验和卸载
//#endif

//  /*------------------------   DMA   -----------------------------------*/  
//  
//  //当我们使用帧校验和卸载功能的时候，一定要使能存储转发模式,存储转发模式中要保证整个帧存储在FIFO中,
//	//这样MAC能插入/识别出帧校验值,当真校验正确的时候DMA就可以处理帧,否则就丢弃掉该帧
//  ETH_InitStructure.ETH_DropTCPIPChecksumErrorFrame = ETH_DropTCPIPChecksumErrorFrame_Enable; //开启丢弃TCP/IP错误帧
//  ETH_InitStructure.ETH_ReceiveStoreForward = ETH_ReceiveStoreForward_Enable; //开启接收数据的存储转发模式  
//  ETH_InitStructure.ETH_TransmitStoreForward = ETH_TransmitStoreForward_Enable;//开启发送数据的存储转发模式

//  ETH_InitStructure.ETH_ForwardErrorFrames = ETH_ForwardErrorFrames_Disable;	//禁止转发错误帧  
//  ETH_InitStructure.ETH_ForwardUndersizedGoodFrames = ETH_ForwardUndersizedGoodFrames_Disable;//不转发过小的好帧 
//  ETH_InitStructure.ETH_SecondFrameOperate = ETH_SecondFrameOperate_Enable;//打开处理第二帧功能
//  ETH_InitStructure.ETH_AddressAlignedBeats = ETH_AddressAlignedBeats_Enable;//开启DMA传输的地址对齐功能
//  ETH_InitStructure.ETH_FixedBurst = ETH_FixedBurst_Enable;//开启固定突发功能    
//  ETH_InitStructure.ETH_RxDMABurstLength = ETH_RxDMABurstLength_32Beat;//DMA发送的最大突发长度为32个节拍 
//  ETH_InitStructure.ETH_TxDMABurstLength = ETH_TxDMABurstLength_32Beat;//DMA接收的最大突发长度为32个节拍
//  ETH_InitStructure.ETH_DMAArbitration = ETH_DMAArbitration_RoundRobin_RxTx_2_1;
//#else
///* Fill ETH_InitStructure parametrs */
//  /*------------------------ ETH_MACCR-----------------------------------*/
//    //参数是否自动配置，选择disable需要自动配置默认参数
//		ETH_InitStructure.ETH_AutoNegotiation = ETH_AutoNegotiation_Enable;
//    ETH_InitStructure.ETH_Watchdog = ETH_Watchdog_Disable;                            //关闭看门狗定时器，允许接收超长帧
//    ETH_InitStructure.ETH_Jabber = ETH_Jabber_Disable;                                //关闭jabber定时器，允许发送超长帧
//    ETH_InitStructure.ETH_InterFrameGap = ETH_InterFrameGap_40Bit;										//发送帧间间隙
//    ETH_InitStructure.ETH_Speed = ETH_Speed_10M;                                     //快速以太网速度
//    ETH_InitStructure.ETH_LoopbackMode = ETH_LoopbackMode_Disable;                    //不启用自循环模式
//    ETH_InitStructure.ETH_Mode = ETH_Mode_FullDuplex;                                 //全双工模式
//  
//    /*自动填充/CRC剥离处理不执行，转发所有帧*/
//    ETH_InitStructure.ETH_AutomaticPadCRCStrip = ETH_AutomaticPadCRCStrip_Disable;   
//#ifdef CHECKSUM_BY_HARDWARE
//		//ETH_InitStructure.ETH_ChecksumOffload = ETH_ChecksumOffload_Enable;								//IPV4头文件硬件校验
//#endif

//    /*------------------------ ETH_MACFFR----------------------------------*/
//    ETH_InitStructure.ETH_ReceiveAll = ETH_ReceiveAll_Disable;                                   //MAC过滤只接受通过源目的地址的数据
//    ETH_InitStructure.ETH_SourceAddrFilter = ETH_SourceAddrFilter_Normal_Enable;                 //MAC过滤源地址错误帧？
//    ETH_InitStructure.ETH_PassControlFrames = ETH_PassControlFrames_BlockAll;                    //MAC不转发任何控制帧
//    ETH_InitStructure.ETH_BroadcastFramesReception = ETH_BroadcastFramesReception_Enable;        //接收广播帧
//    ETH_InitStructure.ETH_DestinationAddrFilter = ETH_DestinationAddrFilter_Normal;              //目的地址过滤结果正常
//    ETH_InitStructure.ETH_PromiscuousMode = ETH_PromiscuousMode_Disable;                         //混杂模式，启用帧过滤
//    ETH_InitStructure.ETH_MulticastFramesFilter = ETH_MulticastFramesFilter_Perfect;             //过滤器正常工作，不传送控制帧
//    ETH_InitStructure.ETH_UnicastFramesFilter = ETH_UnicastFramesFilter_Perfect;                 //单播帧目的地址完美过滤

//    /*------------------------   DMA  ETH_DMAOMR -----------------------------------*/  
//    /* When we use the Checksum offload feature, we need to enable the Store and Forward mode: 
//    he store and forward guarantee that a whole frame is stored in the FIFO, so the MAC can insert/verify the checksum, 
//    f the checksum is OK the DMA can handle the frame otherwise the frame is dropped */
//    /*丢弃校验错误帧不执行(因为未进行硬件校验)*/
//    ETH_InitStructure.ETH_DropTCPIPChecksumErrorFrame = ETH_DropTCPIPChecksumErrorFrame_Disable;
//    ETH_InitStructure.ETH_ReceiveStoreForward = ETH_ReceiveStoreForward_Disable;              //接收数据超过阈值转发
//    ETH_InitStructure.ETH_FlushReceivedFrame = ETH_FlushReceivedFrame_Enable;                 //描述符被占用和接收FIFO不可用时清空FIFO(解决堵塞)
//    ETH_InitStructure.ETH_TransmitStoreForward = ETH_TransmitStoreForward_Disable;            //发送数据完整帧转发
//    ETH_InitStructure.ETH_TransmitThresholdControl = ETH_TransmitThresholdControl_64Bytes;    //发送阈值为64Bytes
//    ETH_InitStructure.ETH_ForwardErrorFrames = ETH_ForwardErrorFrames_Disable;                //接收FIFO丢弃所有错误帧

//    /*接收FIFO上传长度不够的好帧*/
//		ETH_InitStructure.ETH_ForwardUndersizedGoodFrames = ETH_ForwardUndersizedGoodFrames_Enable;
//		ETH_InitStructure.ETH_ReceiveThresholdControl = ETH_ReceiveThresholdControl_128Bytes;       //接收阈值为64Bytes    
//		ETH_InitStructure.ETH_SecondFrameOperate = ETH_SecondFrameOperate_Enable;                  //DMA直接发送第二个帧，不需要之前帧回复

//    /*------------------------   DMA  ETH_DMABMR -----------------------------------*/      
//    ETH_InitStructure.ETH_AddressAlignedBeats = ETH_AddressAlignedBeats_Enable;                //传输地址对齐
//    ETH_InitStructure.ETH_FixedBurst = ETH_FixedBurst_Enable;                                  //固定的突发
//    ETH_InitStructure.ETH_RxDMABurstLength = ETH_RxDMABurstLength_32Beat;                              
//    ETH_InitStructure.ETH_TxDMABurstLength = ETH_TxDMABurstLength_32Beat;
//    ETH_InitStructure.ETH_DMAArbitration = ETH_DMAArbitration_RoundRobin_RxTx_2_1;             //发送和接收比例 2:1
//#endif

//  /* Configure Ethernet */
  EthStatus = ETH_Init(&ETH_InitStructure, LAN8742A_PHY_ADDRESS);//配置ETH
  if(EthStatus==ETH_SUCCESS)//配置成功
    {
      ETH_DMAITConfig(ETH_DMA_IT_NIS|ETH_DMA_IT_R,ENABLE);  	//使能以太网接收中断	
    }
}

/**
  * @brief  Configures the different GPIO ports.
  * 配置不同的GPIO端口。
  * @param  None
  * @retval None
  */
void ETH_GPIO_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
  /* Enable GPIOs clocks */
  RCC_AHB1PeriphClockCmd(ETH_MDIO_GPIO_CLK 			  | ETH_MDC_GPIO_CLK 				  |
                        ETH_RMII_REF_CLK_GPIO_CLK	| ETH_RMII_CRS_DV_GPIO_CLK	|
                        ETH_RMII_RXD0_GPIO_CLK		| ETH_RMII_RXD1_GPIO_CLK		|
                        ETH_RMII_TX_EN_GPIO_CLK	  | ETH_RMII_TXD0_GPIO_CLK		|
                        ETH_RMII_TXD1_GPIO_CLK		| ETH_NRST_GPIO_CLK  , ENABLE);

  /* Enable SYSCFG clock 
  启用S??YSCFG时钟*/
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);  
  SYSCFG_ETH_MediaInterfaceConfig(SYSCFG_ETH_MediaInterface_RMII);


/* Ethernet pins configuration ************************************************/
   /*
        ETH_MDIO -------------------------> PA2
        ETH_MDC --------------------------> PC1
        ETH_MII_RX_CLK/ETH_RMII_REF_CLK---> PA1
        ETH_MII_RX_DV/ETH_RMII_CRS_DV ----> PA7
        ETH_MII_RXD0/ETH_RMII_RXD0 -------> PC4
        ETH_MII_RXD1/ETH_RMII_RXD1 -------> PC5
        ETH_MII_TX_EN/ETH_RMII_TX_EN -----> PG11
        ETH_MII_TXD0/ETH_RMII_TXD0 -------> PG13
        ETH_MII_TXD1/ETH_RMII_TXD1 -------> PG14
				ETH_NRST -------------------------> PI1
   */
// 配置ETH_NRST
	GPIO_InitStructure.GPIO_Pin = ETH_NRST_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(ETH_NRST_PORT, &GPIO_InitStructure);

	ETH_NRST_PIN_LOW();
	_eth_delay_(PHY_CONFIG_DELAY);
	ETH_NRST_PIN_HIGH();
	_eth_delay_(PHY_CONFIG_DELAY);

   /* Configure ETH_MDIO */
	GPIO_InitStructure.GPIO_Pin = ETH_MDIO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(ETH_MDIO_PORT, &GPIO_InitStructure);
	GPIO_PinAFConfig(ETH_MDIO_PORT, ETH_MDIO_SOURCE, ETH_MDIO_AF);

	/* Configure ETH_MDC */
	GPIO_InitStructure.GPIO_Pin = ETH_MDC_PIN;
	GPIO_Init(ETH_MDC_PORT, &GPIO_InitStructure);
	GPIO_PinAFConfig(ETH_MDC_PORT, ETH_MDC_SOURCE, ETH_MDC_AF);

	/* Configure ETH_RMII_REF_CLK */
	GPIO_InitStructure.GPIO_Pin = ETH_RMII_REF_CLK_PIN;
	GPIO_Init(ETH_RMII_REF_CLK_PORT, &GPIO_InitStructure);
	GPIO_PinAFConfig(ETH_RMII_REF_CLK_PORT, ETH_RMII_REF_CLK_SOURCE, ETH_RMII_REF_CLK_AF);

	/* Configure ETH_RMII_CRS_DV */
	GPIO_InitStructure.GPIO_Pin = ETH_RMII_CRS_DV_PIN;
	GPIO_Init(ETH_RMII_CRS_DV_PORT, &GPIO_InitStructure);
	GPIO_PinAFConfig(ETH_RMII_CRS_DV_PORT, ETH_RMII_CRS_DV_SOURCE, ETH_RMII_CRS_DV_AF);

	/* Configure ETH_RMII_RXD0 */
	GPIO_InitStructure.GPIO_Pin = ETH_RMII_RXD0_PIN;
	GPIO_Init(ETH_RMII_RXD0_PORT, &GPIO_InitStructure);
	GPIO_PinAFConfig(ETH_RMII_RXD0_PORT, ETH_RMII_RXD0_SOURCE, ETH_RMII_RXD0_AF);

	/* Configure ETH_RMII_RXD1 */
	GPIO_InitStructure.GPIO_Pin = ETH_RMII_RXD1_PIN;
	GPIO_Init(ETH_RMII_RXD1_PORT, &GPIO_InitStructure);
	GPIO_PinAFConfig(ETH_RMII_RXD1_PORT, ETH_RMII_RXD1_SOURCE, ETH_RMII_RXD1_AF);

	/* Configure ETH_RMII_TX_EN */
	GPIO_InitStructure.GPIO_Pin = ETH_RMII_TX_EN_PIN;
	GPIO_Init(ETH_RMII_TX_EN_PORT, &GPIO_InitStructure);
	GPIO_PinAFConfig(ETH_RMII_TX_EN_PORT, ETH_RMII_TX_EN_SOURCE, ETH_RMII_TX_EN_AF);

	/* Configure ETH_RMII_TXD0 */
	GPIO_InitStructure.GPIO_Pin = ETH_RMII_TXD0_PIN;
	GPIO_Init(ETH_RMII_TXD0_PORT, &GPIO_InitStructure);
	GPIO_PinAFConfig(ETH_RMII_TXD0_PORT, ETH_RMII_TXD0_SOURCE, ETH_RMII_TXD0_AF);

	/* Configure ETH_RMII_TXD1 */
	GPIO_InitStructure.GPIO_Pin = ETH_RMII_TXD1_PIN;
	GPIO_Init(ETH_RMII_TXD1_PORT, &GPIO_InitStructure);
	GPIO_PinAFConfig(ETH_RMII_TXD1_PORT, ETH_RMII_TXD1_SOURCE, ETH_RMII_TXD1_AF);	  
}

/**
  * @brief  Configures and enable the Ethernet global interrupt.
  * 配置并启用以太网全局中断。
  * @param  None
  * @retval None
  */
void ETH_NVIC_Config(void)
{
  NVIC_InitTypeDef   NVIC_InitStructure; 
  
  /* Enable the Ethernet global Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = ETH_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 6 ;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  Link callback function, this function is called on change of link status.
  * 链接回调函数，该函数在链接状态更改时被调用。
  * @param  The network interface
  * @retval None
  */
void ETH_link_callback(struct netif *netif)
{
  __IO uint32_t timeout = 0;
 uint32_t tmpreg;
 uint16_t RegValue;
  ip4_addr_t ipaddr;
  ip4_addr_t netmask;
  ip4_addr_t gw;

  if(netif_is_link_up(netif))
  {
    /* Restart the auto-negotiation */
    if(ETH_InitStructure.ETH_AutoNegotiation != ETH_AutoNegotiation_Disable)
    {
      /* Reset Timeout counter */
      timeout = 0;

      /* Enable auto-negotiation */
      ETH_WritePHYRegister(LAN8742A_PHY_ADDRESS, PHY_BCR, PHY_AutoNegotiation);

      /* Wait until the auto-negotiation will be completed */
      do
      {
        timeout++;
      } while (!(ETH_ReadPHYRegister(LAN8742A_PHY_ADDRESS, PHY_BSR) & PHY_AutoNego_Complete) && (timeout < (uint32_t)PHY_READ_TO));  

      /* Reset Timeout counter */
      timeout = 0;

      /* Read the result of the auto-negotiation */
      RegValue = ETH_ReadPHYRegister(LAN8742A_PHY_ADDRESS, PHY_SR);

      /* Configure the MAC with the Duplex Mode fixed by the auto-negotiation process */
      if((RegValue & PHY_DUPLEX_STATUS) != (uint16_t)RESET)
      {
        /* Set Ethernet duplex mode to Full-duplex following the auto-negotiation */
        ETH_InitStructure.ETH_Mode = ETH_Mode_FullDuplex;  
      }
      else
      {
        /* Set Ethernet duplex mode to Half-duplex following the auto-negotiation */
        ETH_InitStructure.ETH_Mode = ETH_Mode_HalfDuplex;
      }
      /* Configure the MAC with the speed fixed by the auto-negotiation process */
      if(RegValue & PHY_SPEED_STATUS)
      {
        /* Set Ethernet speed to 10M following the auto-negotiation */
        ETH_InitStructure.ETH_Speed = ETH_Speed_10M; 
      }
      else
      {
        /* Set Ethernet speed to 100M following the auto-negotiation */
        ETH_InitStructure.ETH_Speed = ETH_Speed_100M;
      }

      /*------------------------ ETHERNET MACCR Re-Configuration --------------------*/
      /* Get the ETHERNET MACCR value */  
      tmpreg = ETH->MACCR;

      /* Set the FES bit according to ETH_Speed value */ 
      /* Set the DM bit according to ETH_Mode value */ 
      tmpreg |= (uint32_t)(ETH_InitStructure.ETH_Speed | ETH_InitStructure.ETH_Mode);

      /* Write to ETHERNET MACCR */
      ETH->MACCR = (uint32_t)tmpreg;

      _eth_delay_(ETH_REG_WRITE_DELAY);
      tmpreg = ETH->MACCR;
      ETH->MACCR = tmpreg;
    }

    /* Restart MAC interface */
    ETH_Start();

#ifdef USE_DHCP
	ip4_addr_set_zero(&gw);
	ip4_addr_set_zero(&ipaddr);
	ip4_addr_set_zero(&netmask);
  DHCP_state = DHCP_START;
#else
    IP4_ADDR(&ipaddr, IP_ADDR0, IP_ADDR1, IP_ADDR2, IP_ADDR3);
    IP4_ADDR(&netmask, NETMASK_ADDR0, NETMASK_ADDR1 , NETMASK_ADDR2, NETMASK_ADDR3);
    IP4_ADDR(&gw, GW_ADDR0, GW_ADDR1, GW_ADDR2, GW_ADDR3);
#endif /* USE_DHCP */

    netif_set_addr(&gnetif, &ipaddr , &netmask, &gw);
    
    /* When the netif is fully configured this function must be called.*/
    netif_set_up(&gnetif);    

	printf("\r\n  Network Cable is  \r\n");
	printf("    now connected   \r\n");

    EthLinkStatus = 0;
  }
  else
  {
    ETH_Stop();
#ifdef USE_DHCP
    DHCP_state = DHCP_LINK_DOWN;
    dhcp_stop(netif);
#endif /* USE_DHCP */

    /*  When the netif link is down this function must be called.*/
    netif_set_down(&gnetif);

	printf("\r\n	Network Cable is  \r\n");
	printf("     unplugged   \r\n");
  }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
