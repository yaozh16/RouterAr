//
// Created by yaozh16 on 18-11-14.
//

#include "stdlib.h"
#include "arpfind.h"
int arpGet(char *ifname, unsigned int IP,struct arpmac *srcmac,int arp_fd){
    static unsigned char flag=0;
    static struct arpreq arpreq;
    if(flag==0){
        arpreq.arp_pa.sa_family=AF_INET;
        struct sockaddr_in *addr = (struct sockaddr_in*)&arpreq.arp_pa;
        addr->sin_family = AF_INET;
        flag=1;
    }
    ((struct sockaddr_in*)&arpreq.arp_pa)->sin_addr.s_addr = IP;
    strcpy(arpreq.arp_dev, ifname);
    int err = ioctl(arp_fd, SIOCGARP, &arpreq);
    if(err<0){
        printf("arp error:%d\n",IP);
        return err;
    } else{
        srcmac->mac=(unsigned char*)malloc(sizeof(unsigned char)*14);
        memcpy(srcmac->mac,arpreq.arp_ha.sa_data,14);
        unsigned char* hw = (unsigned char*)&arpreq.arp_ha.sa_data; /*硬件地址*/
        //printf("[next hop mac]:");
        //printf("%02x:%02x:%02x:%02x:%02x:%02x\n",hw[0],hw[1],hw[2],hw[3],hw[4],hw[5]);

        return 0;
    }
}