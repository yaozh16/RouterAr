#ifndef __ROUTE__
#define __ROUTE__
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<netinet/ip.h>
#include<netinet/ip_icmp.h>
#include<sys/time.h>
#include<linux/if_ether.h>
#include <arpa/inet.h>

void analyseIP(struct ip *ip, struct sockaddr_in srcsock);
void err_exit(const char *err_msg);

#endif
