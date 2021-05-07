#ifndef LWIPOPTS_H
#define LWIPOPTS_H

//*****************************************************************************
//
// ---------- SNMP options ----------
//
//*****************************************************************************
#define LWIP_SNMP                       0      ����//SNMPЭ������
#if 0//LWIP_SNMP
#define SNMP_CONCURRENT_REQUESTS     1            //SNMP��ǰ������������
#define SNMP_TRAP_DESTINATIONS       1            //SNMP trapĿ�ĵ�ַ��Ŀ
#define SNMP_PRIVATE_MIB             1      ������ //SNMP ˽�нڵ���������
#define SNMP_SAFE_REQUESTS           1            //���ظ�SNMP��ȫ����
#endif

//*****************************************************************************
//
// ---------- Platform specific locking ----------
//
//*****************************************************************************
#define SYS_LIGHTWEIGHT_PROT              1          //�ٽ��жϱ�������(������ģʽ�¿���)
#define NO_SYS                            0          //LWIP�������л��߻��ڲ���ϵͳ��Ϊ0����ڲ���ϵͳ

//*****************************************************************************
//
// ---------- Memory options ----------
//
//*****************************************************************************
#define MEM_LIBC_MALLOC                 0                //����LWIP�Դ��������䶯̬�ڴ�
#define MEM_LIBC_MALLOC                 0                //�ڴ�ز�ͨ���ڴ��������
#define MEM_ALIGNMENT                   4                //�ֽڶ���(��CPU��λ���й�,32λ����Ϊ4)
#define MEM_SIZE                       (25*1024)         //���ڴ��С�����ڷ�������
#define MEMP_SANITY_CHECK               0                //mem_free���ú��������Ƿ���ѭ�� by zc
#define MEMP_OVERFLOW_CHECK             0                //lwip���ڴ������� 
#define MEM_USE_POOLS                   0                //�ڴ�Ѳ�ͨ���ڴ�ط���
#define MEM_USE_POOLS_TRY_BIGGER_POOL   0             �� //�����ڴ�ʧ�ܲ�ѡ����ڴ��
#define MEMP_USE_CUSTOM_POOLS           0                //ͬ��

//*****************************************************************************
//
// ---------- Internal Memory Pool Sizes ----------
//
//*****************************************************************************
#define MEMP_NUM_PBUF												60				//����memp��PBUF_ROM(ROM�ڴ��)��PBUF_REF(RAM�ڴ��)��Ŀ����ܺ�
#define MEMP_NUM_RAW_PCB										4					//RAW���ӵ�PCB��Ŀ(��ҪLWIP RAW��Ч)
#define MEMP_NUM_UDP_PCB										4					//�ܹ�ͬʱ���ӵ�UDP��PCB��Ŀ
#define MEMP_NUM_TCP_PCB										(TCP_WND + TCP_SND_BUF)/TCP_MSS  //�ܹ�ͬʱ���ӵ�TCP��PCB��Ŀ 12
#define MEMP_NUM_TCP_PCB_LISTEN              1				//(TCP_WND + TCP_SND_BUF)/TCP_MSS  //�ܹ�ͬʱ������TCP��PCB��Ŀ
#define MEMP_NUM_TCP_SEG										40					//80 �ܹ�ͬʱ�ڶ������TCP��PCB��Ŀ
#define MEMP_NUM_REASSDATA                   8       //���ͬʱ�ȴ���װ��IP����Ŀ��������IP��������IP��Ƭ
#define MEMP_NUM_ARP_QUEUE                  30       //���ȴ��ظ�ARP�������Ŀ(ARP_QUEUEING��Ч)
#define MEMP_NUM_IGMP_GROUP                  8       //�ಥ��ͬʱ�������ݵ�����Ա��Ŀ(LWIP_IGMP��Ч)
#define MEMP_NUM_SYS_TIMEOUT                20       //�ܹ�ͬʱ����ĳ�ʱ������Ŀ(NO_SYS==0��Ϸ)
#define MEMP_NUM_NETBUF                     10       //netbufs�ṹ����Ŀ
#define MEMP_NUM_NETCONN                    16       //netconns�ṹ����Ŀ
#define MEMP_NUM_TCPIP_MSG_API               40       //tcpip_msg�ṹ�������Ŀ������callback��API��ͨѶ by zc
#define MEMP_NUM_TCPIP_MSG_INPKT            40       //tcpip_msg�������ݰ��ṹ�������Ŀ by zc
#define PBUF_POOL_SIZE                       48    //�ڴ������(С�ڴ��С��ѡ��ɴ���С�ڴ�ռ��)

//*****************************************************************************
//
// ---------- ARP options ----------
//
//*****************************************************************************
#define LWIP_ARP                            1        //ARPЭ������ 
#define ARP_TABLE_SIZE                      10       //ARPά���ı����С
#define ARP_QUEUEING                         1           //Ӳ����ַ����ʱ�����������ݰ��������
#define ETHARP_TRUST_IP_MAC                              //����IP���ݰ�����ֱ������ARP table�ĸ��£�
                                                     //Ϊ0��Ǳ�����IP-MAC��ϵ������ARP����,���Ա���IP��ƭ�������������ʱ
#define ETHARP_SUPPORT_VLAN                  0        //�������������Ϊ1������������ͨѶ��Ч

//*****************************************************************************
//
// ---------- IP options ----------
//
//*****************************************************************************
#define IP_FORWARD                          0    //������ƥ�����ݰ�ת��,��ӿ�ʱΪ1
#define IP_OPTIONS_ALLOWED                  1    //��IPѡ�����ݰ����� Ϊ0��������IP���ݰ�
#define IP_REASSEMBLY                       1    //�������IP����Ƭ��(Ϊ0���������ܹ����մ���MTU�İ�)
#define IP_FRAG                             1    //������IP����Ƭ��
#define IP_REASS_MAXAGE                     3    //������յ����ֶ���
#define IP_REASS_MAX_PBUFS                  10   //���������ڵ�IP��Ƭ��ռ�õ��ڴ�ظ���
#define IP_FRAG_USES_STATIC_BUF             1    //IP��Ƭʹ�þ�̬������
#define IP_FRAG_MAX_MTU                     1500 //IP��Ƭ��󻺳�����
#define IP_DEFAULT_TTL                      255  //IP���ݰ�������豸��Ŀ
#define IP_SOF_BROADCAST                    0    //IP���͹㲥������
#define IP_SOF_BROADCAST_RECV               0    //IP���չ㲥������

//*****************************************************************************
//
// ---------- ICMP options ----------
//
//*****************************************************************************
#define LWIP_ICMP                           1    //����ping������/����
#define ICMP_TTL													(IP_DEFAULT_TTL) //ping��������豸��Ŀ
#define LWIP_BROADCAST_PING									0				//���ظ��㲥ping��
#define LWIP_MULTICAST_PING									0			//���ظ��ಥping��

//*****************************************************************************
//
// ---------- RAW options ----------
//
//*****************************************************************************
#define LWIP_RAW														0										//�޲���ϵͳ���ڻص���������
#define RAW_TTL															(IP_DEFAULT_TTL)		//Ӧ�ò����ݴ������(����IP���TTL)

//*****************************************************************************
//
// ---------- DHCP options ----------
//
//*****************************************************************************
#define LWIP_DHCP														0			//��̬����Э������(Ϊ1ʱ)
#define LWIP_AUTOIP													0		//��̬����IP��ַ����(Ϊ1ʱ)
#define LWIP_DHCP_AUTOIP_COOP								0		//����������������ͬʱ������1���ӿ�(Ϊ1ʱ)
#define LWIP_DHCP_AUTOIP_COOP_TRIES					9			//DHCP��������ʧ�ܴ�����ʧ����ʹ��AUTOUP

//*****************************************************************************
//
// ---------- IGMP options ----------
//
//*****************************************************************************
#define LWIP_IGMP														0					//LWIP�����Э��

//*****************************************************************************
//
// ---------- DNS options -----------
//
//*****************************************************************************
#if LWIP_DNS /* don't build if not configured for use in lwipopts.h */
#define LWIP_DNS														0			//����������ģ��(����UDPЭ��)
#define DNS_TABLE_SIZE											4			//����������ά������������Ŀ
#define DNS_MAX_NAME_LENGTH									256		//����������������ַ��󳤶�
#define DNS_MAX_SERVERS											2			//������������������Ŀ
#define DNS_DOES_NAME_CHECK									1			//��ѯ����������ʱ����ַ��
#define DNS_USES_STATIC_BUF									1			//����������ʹ�þ�̬��ַ
#define DNS_MSG_SIZE												512		//�����������������ͨѶ���ݳ���
#define DNS_LOCAL_HOSTLIST									0			//�ڱ���ά����������������-��ַ��(Ϊ1ʱ)
#define DNS_LOCAL_HOSTLIST_IS_DYNAMIC				0			//����-��ַ��ʵʱ����(Ϊ1ʱ)
#endif
/* ---------- Statistics options ---------- */
#define LWIP_STATS 0
#define LWIP_PROVIDE_ERRNO 1
//*****************************************************************************
//
// ---------- UDP options ----------
//
//*****************************************************************************
#define LWIP_UDP															1						//����UDPЭ��(snmpЭ����ڴ�)
#define LWIP_UDPLITE                          1						//UDPЭ�����þ����
#define UDP_TTL																(IP_DEFAULT_TTL)//UDP���ݰ��������
#define LWIP_NETBUF_RECVINFO									0						//���յ������ݰ����׸�������������Ŀ�ĵ�ַ�Ͷ˿�

//*****************************************************************************
//
// ---------- TCP options ----------
//
//*****************************************************************************
#define LWIP_TCP															1																	//����TCPЭ��(httpЭ����ڴ�)
#define TCP_TTL                              (IP_DEFAULT_TTL)                    //TCP���ݰ��������
#define TCP_WND                              4*TCP_MSS                           //tcp���ڴ�С
#define TCP_MAXRTX                           12                                  //��������ش�TCP���ݰ���Ŀ
#define TCP_SYNMAXRTX                        6                                   //��������ش�SYN���Ӱ���Ŀ
#define TCP_QUEUE_OOSEQ                      (LWIP_TCP)                          //TCP���ն��������ݿ�����
#define TCP_MSS                              1460                                //tcp�������ֶγ���
#define TCP_CALCULATE_EFF_SEND_MSS           1                                   //tcpԤ�Ʒ��͵ķֶγ��ȣ�Ϊ1����ݴ��ڴ�С����
#define TCP_SND_BUF                          (8*TCP_MSS)                         //TCP���ͻ�����  zc 7.1                                     
#define TCP_SND_QUEUELEN                     (4*(TCP_SND_BUF/TCP_MSS))           //TCP���Ͷ��г���
#define TCP_SNDLOWAT                         (TCP_SND_BUF/4)                     //TCP�ɷ������ݳ���
#define TCP_LISTEN_BACKLOG                   1                                   //TCP����������
#define TCP_DEFAULT_LISTEN_BACKLOG           0xff                                //TCP���ӺͰ����ӵ�����
#define LWIP_TCP_TIMESTAMPS                  0                                   //TCPʱ���ѡ��
#define TCP_WND_UPDATE_THRESHOLD             (TCP_WND / 4)                       //TCP���ڸ�����ֵ
//TCP��ʱ������ѡ��
#define  TCP_MSL                             10000                            //TCP���Ӵ���ʱ�� ��λms
#define  TCP_FIN_WAIT_TIMEOUT                20000                             //FIN�ȵ�ACKӦ��ʱ�� ��λms
#define  TCP_TMR_INTERVA                     20                                 //TCP��ʱ��������� 20ms

//*****************************************************************************
//         
// ---------- Internal Memory Pool Sizes ----------
//
//*****************************************************************************
 #define  PBUF_LINK_HLEN                14            //Ϊ������̫��ͷ����İ�����(����MAC��ַ+Զ��MAC��ַ+Э������) 6+6+2                          #define  PBUF_POOL_BUFSIZE             256           //�����ڴ�س��ȣ�Ҫ���ǵ�4�ֽڶ������Ѵ�С
 #define  ETH_PAD_SIZE                  0             //��̫����䳤�ȣ�stm32����Ϊ0 ���ݷ��Ͱ��ж�
 
//��ϵ����.. Ĭ�ϼ���

//*****************************************************************************
//
// ---------- Sequential layer options ----------
//
//*****************************************************************************
#define LWIP_TCPIP_CORE_LOCKING           1              //?
#define LWIP_NETCONN                      1              //Ӧ�ò�ʹ��NETCONN��غ���

//*****************************************************************************
//
// ---------- Socket Options ----------
//
//*****************************************************************************
#define LWIP_SOCKET                       1              //�ر�SOCKETͨѶ
//#define LWIP_COMPAT_SOCKETS             1              
//#define LWIP_POSIX_SOCKETS_IO_NAMES     1              
#define LWIP_SO_RCVTIMEO                  0              //conn->acceptmbox/recvmbox�����г�ʱ����,��ʱ���Զ��Ͽ�����
//#define LWIP_SO_RCVBUF                  0              
//#define SO_REUSE                        0              

//*****************************************************************************
//
// ---------- Statistics options ----------
//
//*****************************************************************************
 #define LWIP_STATS                          0                    //LWIPͳ��ѡ��

//*****************************************************************************
//
// ---------- checksum options ----------
//
//*****************************************************************************
#define CHECKSUM_GEN_IP                      1                 //IPУ�������
#define CHECKSUM_GEN_UDP                     1                 //UDPУ�������
#define CHECKSUM_GEN_TCP                     1                 //TCPУ�������
#define CHECKSUM_CHECK_IP                    1                 //IPУ���У��
#define CHECKSUM_CHECK_UDP                   1                 //UDPУ���У��
#define CHECKSUM_CHECK_TCP                   1                 //TCPУ���У��

//*****************************************************************************
//
// ---------- TCP KEEPLIVE ----------
//
//*****************************************************************************
#define LWIP_TCP_KEEPALIVE                   1                   //tcp���ʱ��
#define TCP_KEEPIDLE_DEFAULT                 60000             //����ʱ�� 60s
#define TCP_KEEPINTVL_DEFAULT                10000             //����̽�� 10s
#define TCP_KEEPCNT_DEFAULT                  9U    

//*****************************************************************************
//
// ---------- DEBUG_OPTIONS  ----------
//
//*****************************************************************************
#define DEBUG_OPTIONS                        0                 //���Դ���
#endif
