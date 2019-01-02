//
// Created by yaozh16 on 18-11-14.
//
#include <net/if.h>
#include "sendetherip.h"
#include "analyseip.h"
void ip_transmit(char* skbuf,
                 unsigned int recvlen,
                 unsigned char *ifname,
                 unsigned char *nexthopmac,
                 unsigned int mac_fd,
                 unsigned int send_fd
                ){
    //get interface mac
    struct ifreq ifr;
    strncpy(ifr.ifr_ifrn.ifrn_name, (char*)ifname, IFNAMSIZ - 1);
    if (ioctl(mac_fd, SIOCGIFHWADDR, &ifr) == -1){
        printf("[error]device mac not found:%s\n",ifname);
        return;
    }
    //unsigned char* mac = (unsigned char*)(ifr.ifr_ifru.ifru_hwaddr.sa_data);
    //printf("[%10s mac]: %2.2x:%2.2x:%2.2x:%2.2x:%2.2x:%2.2x\n",ifname, mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);
    //mac=nexthopmac;
    //printf("[%10s mac]: %2.2x:%2.2x:%2.2x:%2.2x:%2.2x:%2.2x\n","dst", mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);

    //修改以太网包头
    struct ether_header* etherHeader=(struct ether_header*)skbuf;

    //etherHeader->ether_type=0x0800;
    memcpy(etherHeader->ether_shost,ifr.ifr_ifru.ifru_hwaddr.sa_data,6);
    memcpy(etherHeader->ether_dhost,nexthopmac,6);


    //send packet
    struct sockaddr_ll addr;
    memset(&addr,0,sizeof(struct sockaddr_ll));
    addr.sll_family=AF_PACKET;
    addr.sll_protocol=htons(ETH_P_IP);
    addr.sll_ifindex=if_nametoindex(ifname);
    memcpy(addr.sll_addr,nexthopmac,6);
    ssize_t ret=sendto(send_fd,
                       skbuf,
                       recvlen,
                       0,
                       (struct sockaddr*)&addr,
                       sizeof(struct sockaddr_ll));
    printf("[finish]%ld,%d\n",ret,recvlen);
}