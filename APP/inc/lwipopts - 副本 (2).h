#ifndef LWIPOPTS_H
#define LWIPOPTS_H

//*****************************************************************************
//
// ---------- SNMP options ----------
//
//*****************************************************************************
#define LWIP_SNMP                       0      　　//SNMP协议启用
#if 0//LWIP_SNMP
#define SNMP_CONCURRENT_REQUESTS     1            //SNMP当前允许请求数量
#define SNMP_TRAP_DESTINATIONS       1            //SNMP trap目的地址数目
#define SNMP_PRIVATE_MIB             1      　　　 //SNMP 私有节点设置允许
#define SNMP_SAFE_REQUESTS           1            //仅回复SNMP安全请求
#endif

//*****************************************************************************
//
// ---------- Platform specific locking ----------
//
//*****************************************************************************
#define SYS_LIGHTWEIGHT_PROT              1          //临界中断保护开关(多任务模式下开启)
#define NO_SYS                            0          //LWIP独立运行或者基于操作系统，为0则基于操作系统

//*****************************************************************************
//
// ---------- Memory options ----------
//
//*****************************************************************************
#define MEM_LIBC_MALLOC                 0                //采用LWIP自带函数分配动态内存
#define MEM_LIBC_MALLOC                 0                //内存池不通过内存堆来分配
#define MEM_ALIGNMENT                   4                //字节对齐(和CPU的位数有关,32位设置为4)
#define MEM_SIZE                       (25*1024)         //堆内存大小，用于发送数据
#define MEMP_SANITY_CHECK               0                //mem_free调用后检查链表是否有循环 by zc
#define MEMP_OVERFLOW_CHECK             0                //lwip堆内存溢出检查 
#define MEM_USE_POOLS                   0                //内存堆不通过内存池分配
#define MEM_USE_POOLS_TRY_BIGGER_POOL   0             　 //申请内存失败不选择大内存池
#define MEMP_USE_CUSTOM_POOLS           0                //同上

//*****************************************************************************
//
// ---------- Internal Memory Pool Sizes ----------
//
//*****************************************************************************
#define MEMP_NUM_PBUF												60				//来自memp的PBUF_ROM(ROM内存池)和PBUF_REF(RAM内存池)数目最大总和
#define MEMP_NUM_RAW_PCB										4					//RAW连接的PCB数目(需要LWIP RAW有效)
#define MEMP_NUM_UDP_PCB										4					//能够同时连接的UDP的PCB数目
#define MEMP_NUM_TCP_PCB										(TCP_WND + TCP_SND_BUF)/TCP_MSS  //能够同时连接的TCP的PCB数目 12
#define MEMP_NUM_TCP_PCB_LISTEN              1				//(TCP_WND + TCP_SND_BUF)/TCP_MSS  //能够同时监听的TCP的PCB数目
#define MEMP_NUM_TCP_SEG										40					//80 能够同时在队列里的TCP的PCB数目
#define MEMP_NUM_REASSDATA                   8       //最大同时等待重装的IP包数目，是整个IP包，不是IP分片
#define MEMP_NUM_ARP_QUEUE                  30       //最大等待回复ARP请求的数目(ARP_QUEUEING有效)
#define MEMP_NUM_IGMP_GROUP                  8       //多播组同时接收数据的最大成员数目(LWIP_IGMP有效)
#define MEMP_NUM_SYS_TIMEOUT                20       //能够同时激活的超时连接数目(NO_SYS==0有戏)
#define MEMP_NUM_NETBUF                     10       //netbufs结构的数目
#define MEMP_NUM_NETCONN                    16       //netconns结构的数目
#define MEMP_NUM_TCPIP_MSG_API               40       //tcpip_msg结构的最大数目，用于callback和API的通讯 by zc
#define MEMP_NUM_TCPIP_MSG_INPKT            40       //tcpip_msg接受数据包结构的最大数目 by zc
#define PBUF_POOL_SIZE                       48    //内存池数量(小内存减小该选项可大大减小内存占用)

//*****************************************************************************
//
// ---------- ARP options ----------
//
//*****************************************************************************
#define LWIP_ARP                            1        //ARP协议允许 
#define ARP_TABLE_SIZE                      10       //ARP维护的表项大小
#define ARP_QUEUEING                         1           //硬件地址解析时，将发送数据包计入队列
#define ETHARP_TRUST_IP_MAC                              //所有IP数据包都会直接引起ARP table的更新，
                                                     //为0则非表项内IP-MAC关系会引起ARP请求,可以避免IP欺骗，不过会造成延时
#define ETHARP_SUPPORT_VLAN                  0        //非虚拟局域网，为1则仅虚拟局域网通讯有效

//*****************************************************************************
//
// ---------- IP options ----------
//
//*****************************************************************************
#define IP_FORWARD                          0    //不允许不匹配数据包转发,多接口时为1
#define IP_OPTIONS_ALLOWED                  1    //带IP选项数据包允许 为0则丢弃所有IP数据包
#define IP_REASSEMBLY                       1    //允许接收IP包分片包(为0不允许，不能够接收大于MTU的包)
#define IP_FRAG                             1    //允许发送IP包分片包
#define IP_REASS_MAXAGE                     3    //允许接收的最大分段数
#define IP_REASS_MAX_PBUFS                  10   //最大允许存在的IP分片包占用的内存池个数
#define IP_FRAG_USES_STATIC_BUF             1    //IP分片使用静态缓冲区
#define IP_FRAG_MAX_MTU                     1500 //IP分片最大缓冲数量
#define IP_DEFAULT_TTL                      255  //IP数据包最大经历设备数目
#define IP_SOF_BROADCAST                    0    //IP发送广播包过滤
#define IP_SOF_BROADCAST_RECV               0    //IP接收广播包过滤

//*****************************************************************************
//
// ---------- ICMP options ----------
//
//*****************************************************************************
#define LWIP_ICMP                           1    //开启ping包接收/发送
#define ICMP_TTL													(IP_DEFAULT_TTL) //ping包最大经历设备数目
#define LWIP_BROADCAST_PING									0				//不回复广播ping包
#define LWIP_MULTICAST_PING									0			//不回复多播ping包

//*****************************************************************************
//
// ---------- RAW options ----------
//
//*****************************************************************************
#define LWIP_RAW														0										//无操作系统基于回调函数驱动
#define RAW_TTL															(IP_DEFAULT_TTL)		//应用层数据传输次数(基于IP层的TTL)

//*****************************************************************************
//
// ---------- DHCP options ----------
//
//*****************************************************************************
#define LWIP_DHCP														0			//动态主机协议配置(为1时)
#define LWIP_AUTOIP													0		//动态主机IP地址配置(为1时)
#define LWIP_DHCP_AUTOIP_COOP								0		//允许上述两种配置同时存在于1个接口(为1时)
#define LWIP_DHCP_AUTOIP_COOP_TRIES					9			//DHCP分配允许失败次数，失败则使用AUTOUP

//*****************************************************************************
//
// ---------- IGMP options ----------
//
//*****************************************************************************
#define LWIP_IGMP														0					//LWIP组管理协议

//*****************************************************************************
//
// ---------- DNS options -----------
//
//*****************************************************************************
#if LWIP_DNS /* don't build if not configured for use in lwipopts.h */
#define LWIP_DNS														0			//域名服务器模块(依托UDP协议)
#define DNS_TABLE_SIZE											4			//域名服务器维护的最大入口数目
#define DNS_MAX_NAME_LENGTH									256		//域名服务器主机地址最大长度
#define DNS_MAX_SERVERS											2			//域名服务器最大服务数目
#define DNS_DOES_NAME_CHECK									1			//查询域名服务器时检测地址名
#define DNS_USES_STATIC_BUF									1			//域名服务器使用静态地址
#define DNS_MSG_SIZE												512		//域名服务器接收最大通讯数据长度
#define DNS_LOCAL_HOSTLIST									0			//在本地维护域名服务器主机-地址表(为1时)
#define DNS_LOCAL_HOSTLIST_IS_DYNAMIC				0			//主机-地址表实时更新(为1时)
#endif
/* ---------- Statistics options ---------- */
#define LWIP_STATS 0
#define LWIP_PROVIDE_ERRNO 1
//*****************************************************************************
//
// ---------- UDP options ----------
//
//*****************************************************************************
#define LWIP_UDP															1						//启用UDP协议(snmp协议基于此)
#define LWIP_UDPLITE                          1						//UDP协议启用精简版
#define UDP_TTL																(IP_DEFAULT_TTL)//UDP数据包传输次数
#define LWIP_NETBUF_RECVINFO									0						//接收到的数据包除首个外其它不附加目的地址和端口

//*****************************************************************************
//
// ---------- TCP options ----------
//
//*****************************************************************************
#define LWIP_TCP															1																	//启用TCP协议(http协议基于此)
#define TCP_TTL                              (IP_DEFAULT_TTL)                    //TCP数据包传输次数
#define TCP_WND                              4*TCP_MSS                           //tcp窗口大小
#define TCP_MAXRTX                           12                                  //最大允许重传TCP数据包数目
#define TCP_SYNMAXRTX                        6                                   //最大允许重传SYN连接包数目
#define TCP_QUEUE_OOSEQ                      (LWIP_TCP)                          //TCP接收队列外数据块排序
#define TCP_MSS                              1460                                //tcp报文最大分段长度
#define TCP_CALCULATE_EFF_SEND_MSS           1                                   //tcp预计发送的分段长度，为1则根据窗口大小分配
#define TCP_SND_BUF                          (8*TCP_MSS)                         //TCP发送缓冲区  zc 7.1                                     
#define TCP_SND_QUEUELEN                     (4*(TCP_SND_BUF/TCP_MSS))           //TCP发送队列长度
#define TCP_SNDLOWAT                         (TCP_SND_BUF/4)                     //TCP可发送数据长度
#define TCP_LISTEN_BACKLOG                   1                                   //TCP多连接允许
#define TCP_DEFAULT_LISTEN_BACKLOG           0xff                                //TCP连接和半连接的总数
#define LWIP_TCP_TIMESTAMPS                  0                                   //TCP时间戳选项
#define TCP_WND_UPDATE_THRESHOLD             (TCP_WND / 4)                       //TCP窗口更新阈值
//TCP定时器设置选项
#define  TCP_MSL                             10000                            //TCP连接存在时间 单位ms
#define  TCP_FIN_WAIT_TIMEOUT                20000                             //FIN等到ACK应答时间 单位ms
#define  TCP_TMR_INTERVA                     20                                 //TCP定时器计数间隔 20ms

//*****************************************************************************
//         
// ---------- Internal Memory Pool Sizes ----------
//
//*****************************************************************************
 #define  PBUF_LINK_HLEN                14            //为处理以太网头申请的包长度(本地MAC地址+远端MAC地址+协议类型) 6+6+2                          #define  PBUF_POOL_BUFSIZE             256           //单个内存池长度，要考虑到4字节对齐和最佳大小
 #define  ETH_PAD_SIZE                  0             //以太网填充长度，stm32设置为0 根据发送包判断
 
//关系不大.. 默认即可

//*****************************************************************************
//
// ---------- Sequential layer options ----------
//
//*****************************************************************************
#define LWIP_TCPIP_CORE_LOCKING           1              //?
#define LWIP_NETCONN                      1              //应用层使用NETCONN相关函数

//*****************************************************************************
//
// ---------- Socket Options ----------
//
//*****************************************************************************
#define LWIP_SOCKET                       1              //关闭SOCKET通讯
//#define LWIP_COMPAT_SOCKETS             1              
//#define LWIP_POSIX_SOCKETS_IO_NAMES     1              
#define LWIP_SO_RCVTIMEO                  0              //conn->acceptmbox/recvmbox接收有超时限制,超时后自动断开连接
//#define LWIP_SO_RCVBUF                  0              
//#define SO_REUSE                        0              

//*****************************************************************************
//
// ---------- Statistics options ----------
//
//*****************************************************************************
 #define LWIP_STATS                          0                    //LWIP统计选项

//*****************************************************************************
//
// ---------- checksum options ----------
//
//*****************************************************************************
#define CHECKSUM_GEN_IP                      1                 //IP校验和生成
#define CHECKSUM_GEN_UDP                     1                 //UDP校验和生成
#define CHECKSUM_GEN_TCP                     1                 //TCP校验和生成
#define CHECKSUM_CHECK_IP                    1                 //IP校验和校验
#define CHECKSUM_CHECK_UDP                   1                 //UDP校验和校验
#define CHECKSUM_CHECK_TCP                   1                 //TCP校验和校验

//*****************************************************************************
//
// ---------- TCP KEEPLIVE ----------
//
//*****************************************************************************
#define LWIP_TCP_KEEPALIVE                   1                   //tcp保活定时器
#define TCP_KEEPIDLE_DEFAULT                 60000             //保活时间 60s
#define TCP_KEEPINTVL_DEFAULT                10000             //保活探测 10s
#define TCP_KEEPCNT_DEFAULT                  9U    

//*****************************************************************************
//
// ---------- DEBUG_OPTIONS  ----------
//
//*****************************************************************************
#define DEBUG_OPTIONS                        0                 //调试代码
#endif
