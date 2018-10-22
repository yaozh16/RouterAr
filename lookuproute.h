#ifndef __FIND__
#define __FIND__
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include <arpa/inet.h>

struct route
{
    struct route *prev;
    struct route *next;

    unsigned int num;
    struct in_addr iprefix;
	unsigned int prefixlen;
    struct nexthop *nexthop;
};
 
struct nexthop
{
   struct nexthop *next;
   struct nexthop *prev;
 
   //Interface index. 
   char *ifname;
   unsigned int ifindex;//zlw ifindex2ifname()获取出接口
 
   // Nexthop address 
   struct in_addr nexthopaddr;
};

struct nextaddr
{
   char *ifname;
   struct in_addr ipv4addr;
   unsigned int prefixl;
};

int insert_route(struct route *rt_table,unsigned long  ip4prefix,unsigned int prefixlen,char *ifname,unsigned int ifindex,unsigned long  nexthopaddr);
int find_route(struct route *rt_table,struct in_addr dstaddr,struct nextaddr *nexthopinfo);
int delete_route(struct route *rt_table,struct in_addr dstaddr,unsigned int prefixlen);

#endif
