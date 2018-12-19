//
// Created by yaozh16 on 18-10-22.
//

#include <stdio.h>
#include <netinet/ip.h>

void err_exit(const char *err_msg){
    printf("\033[1;32merror\033[0m");
}

void analyseIP(struct ip *ip){
    printf("[analyze]");
    printf("src:%d.%d.%d.%d ",(ip->ip_src.s_addr>>0)&255,(ip->ip_src.s_addr>>8)&255,(ip->ip_src.s_addr>>16)&255,(ip->ip_src.s_addr>>24)&255);
    printf("dst:%d.%d.%d.%d\n",(ip->ip_dst.s_addr>>0)&255,(ip->ip_dst.s_addr>>8)&255,(ip->ip_dst.s_addr>>16)&255,(ip->ip_dst.s_addr>>24)&255);
}