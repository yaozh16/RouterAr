#include "analyseip.h"
#include "checksum.h"
#include "lookuproute.h"
#include "arpfind.h"
#include "sendetherip.h"
#include "recvroute.h"
#include <pthread.h>

#define ICMP_PACKET_LEN sizeof(struct icmp)

#define IP_HEADER_LEN sizeof(struct ip)
#define IP_ICMP_PACKET_LEN  ICMP_PACKET_LEN + IP_HEADER_LEN
#define ETHER_HEADER_LEN sizeof(struct ether_header)

int sta=0;

struct route *route_table;

//接收路由信息的线程
void *thr_fn(void *arg)
{
	int st=0;
	struct selfroute *selfrt;
	selfrt = (struct selfroute*)malloc(sizeof(struct selfroute));
	memset(selfrt,0,sizeof(struct selfroute));

	//get if.name
	struct if_nameindex *head, *ifni;
	ifni = if_nameindex();
	head = ifni;
	char *ifname;

	startListen();
	// add-24 del-25
	while(1)
	{
		st=static_route_get(selfrt);		//和quagga通信获取路由表：第四步
        printf("%d ",selfrt->cmdnum);
		if(st == 1)
		{
			if(selfrt->cmdnum == 24)
			{
				while(ifni->if_index != 0) {
					if(ifni->if_index==selfrt->ifindex)
					{
						printf("if_name is %s\n",ifni->if_name);
						ifname= ifni->if_name;
						break;
					}
					ifni++;
				}

				{
					//插入到路由表里
					//对比insert_route(route_table,inet_addr("192.168.5.0"),24,"enp0s9",if_nametoindex("enp0s9"),inet_addr("192.168.3.1"));
                    printf("[insert]:\n");
                    printf("[inet_addr]:%s\n",inet_ntoa(selfrt->prefix));
                    printf("[prefix]:%s\n",inet_ntoa(selfrt->prefix));
                    printf("[ifname]:%s\n",ifname);
                    printf("[ifindex]:%d\n",selfrt->ifindex);
                    printf("[nexthop]:%s\n",inet_ntoa(selfrt->nexthop));
					insert_route(route_table,
								 selfrt->prefix.s_addr,
								 selfrt->prefixlen,
								 ifname,
								 selfrt->ifindex,
								 selfrt->nexthop.s_addr);
				}
			}
			else if(selfrt->cmdnum == 25)
			{
				//从路由表里删除路由
				printf("[delete]:\n");
				printf("[ifname]:%s\n",ifname);
                printf("[prefix]:%u,%u\n",inet_addr("192.168.5.0"),selfrt->prefix.s_addr);
				delete_route(route_table,
							 selfrt->prefix,
							 selfrt->prefixlen);
			}
		}
	}

}

int main()
{
	char skbuf[1514];
	char data[1480];
	int recv_fd,send_fd,mac_fd;
	ssize_t recvlen;
	struct ip *ip_recvpkt;
	pthread_t tid;

	//创建raw socket套接字
	if((recv_fd=socket(AF_PACKET,SOCK_RAW,htons(ETH_P_IP)))==-1)
	{
		err_exit("recvfd()\n");
	}
	if((send_fd = socket(AF_PACKET, SOCK_RAW, IPPROTO_RAW))==-1)
	{
		err_exit("sendfd()\n");
	}
	if((mac_fd = socket(AF_INET, SOCK_DGRAM, 0))==-1)
	{
		err_exit("arpfd()\n");
	}
    if(pthread_mutex_init(&mutex, NULL) != 0){
        err_exit("mutex err\n");
    }

	//路由表初始化
	route_table=(struct route*)malloc(sizeof(struct route));

	if(route_table==NULL)
	{
		printf("malloc error!!\n");
		return -1;
	}
	memset(route_table,0,sizeof(struct route));


	{
		//调用添加函数insert_route往路由表里添加直连路由
        //insert_route(route_table,inet_addr("192.168.5.0"),24,"enp0s9",if_nametoindex("enp0s9"),inet_addr("192.168.3.1"));
	}

	//创建线程去接收路由信息
	int pd;
	pd = pthread_create(&tid,NULL,thr_fn,NULL);

	while(1)
	{

		//接收ip数据包模块
		recvlen=recv(recv_fd,skbuf,sizeof(skbuf),0);//读入skbuf中
		if(recvlen>0)
		{

			ip_recvpkt = (struct ip *)(skbuf+ETHER_HEADER_LEN);//实际IP包内容


			//分析打印ip数据包的源和目的ip地址
            //发送与接收
			if(ip_recvpkt->ip_src.s_addr == inet_addr("192.168.1.1"))
			{


                //analyseIP(ip_recvpkt);
				int s;
				memset(data,0,1480);
				for(s=0;s<1480;s++)
				{
					data[s]=skbuf[s+34];
				}


				// 校验计算模块
				struct _iphdr *iphead;
				int c=0;

				iphead=(struct _iphdr *)ip_recvpkt;
				{
					//调用校验函数check_sum，成功返回1
                    c=check_sum((unsigned short*)iphead,iphead->h_verlen&15,iphead->checksum);
				}
				if(c ==1)
				{
					//printf("checksum is ok!!\n");
				}else
				{
					printf("checksum is error !!\n");
					return -1;
				}

				{
					//调用计算校验和函数count_check_sum
                    if(0==count_check_sum((unsigned short*)iphead)) {
                        //TTL==0
                        printf("TTL error:0\n");
                        continue;
                    }
				}


				//查找路由表，获取下一跳ip地址和出接口模块

				struct nextaddr *nexthopinfo;
				nexthopinfo = (struct nextaddr *)malloc(sizeof(struct nextaddr));
				memset(nexthopinfo,0,sizeof(struct nextaddr));

				{

                    //调用查找路由函数lookup_route，获取下一跳ip地址和出接口
                    int ret=0;
                    ret=find_route(route_table,ip_recvpkt->ip_dst,nexthopinfo);
                    if(ret==-1) {
                        printf("nexthopinfo not found !!\n");
						analyseIP(ip_recvpkt);
						free(nexthopinfo);
                        continue;
                    }
                    if(ret==0){
                        printf("[nexthop interface]:%s\n",nexthopinfo->ifname);
                    }
				}


				//arp find
				struct arpmac *srcmac;
				srcmac = (struct arpmac*)malloc(sizeof(struct arpmac));
				memset(srcmac,0,sizeof(struct arpmac));
				{
					//调用arpGet获取下一跳的mac地址
					arpGet(nexthopinfo->ifname,nexthopinfo->ipv4addr.s_addr,srcmac,mac_fd);
				}

				//send ether icmp
				{

					//调用ip_transmit函数   填充数据包，通过原始套接字从查表得到的出接口(比如网卡2)
					// 将数据包发送出去
					//将获取到的下一跳接口信息存储到存储接口信息的结构体ifreq里，
					// 通过ioctl获取出接口的mac地址作为数据包的源mac地址
					//封装数据包：
					//<1>.根据获取到的信息填充以太网数据包头，
					// 以太网包头主要需要源mac地址、目的mac地址、以太网类型
					// eth_header->ether_type = htons(ETHERTYPE_IP);
					//<2>.再填充ip数据包头，对其进行校验处理；
					//<3>.然后再填充接收到的ip数据包剩余数据部分，然后通过raw socket发送出去

					ip_transmit(skbuf,recvlen,nexthopinfo->ifname,srcmac->mac,mac_fd,send_fd);
				}
				//clear
				{
					free(nexthopinfo);
					free(srcmac->mac);
					free(srcmac);
				}
			}



		}
	}

	close(recv_fd);
	return 0;
}

