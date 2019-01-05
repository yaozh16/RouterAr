//
// Created by yaozh16 on 18-11-14.
//
#include <unistd.h>
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
        perror("[ARPGET Error]");
        return -1;
    } else{
        memcpy(srcmac->mac,arpreq.arp_ha.sa_data,14);
        return 0;
    }

}