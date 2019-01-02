//
// Created by yaozh16 on 18-10-22.
//

#include <arpa/inet.h>
#include <stdio.h>
#include <netinet/ip.h>

void err_exit(const char *err_msg){
    printf("\033[1;32m error:%s\033[0m\n",err_msg);
}

void analyseIP(struct ip *ip, struct sockaddr_in srcsock){
    printf("[analyze]\n");
    printf("\tsrc:%d.%d.%d.%d:%d\n",
            (ip->ip_src.s_addr>>0)&255,(ip->ip_src.s_addr>>8)&255,
            (ip->ip_src.s_addr>>16)&255,(ip->ip_src.s_addr>>24)&255,
            ip->ip_tos);
    printf("\tdst:%d.%d.%d.%d\n",(ip->ip_dst.s_addr>>0)&255,(ip->ip_dst.s_addr>>8)&255,(ip->ip_dst.s_addr>>16)&255,(ip->ip_dst.s_addr>>24)&255);
}