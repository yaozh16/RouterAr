//
// Created by yaozh16 on 18-12-26.
//

#include <arpa/inet.h>
#include "lookuproute.h"
#include <net/if.h>
#include <pthread.h>
//segmentTree
struct routeTableNode{
    short valid;
    struct routeTableNode* childs[2];
    struct in_addr iprefix;
    uint32_t prefixlen;


    //nexthop
    unsigned int ifindex;//获取出接口
    struct in_addr nexthopaddr;
    char ifname[IFNAMSIZ];

    //generated
    uint32_t mask;
};





int insert_route_s(struct routeTableNode *root,
                   uint32_t  ip4prefix,
                   unsigned int prefixlen,
                   char *ifname,
                   unsigned int ifindex,
                   uint32_t  nexthopaddr);

int find_route_s(struct routeTableNode *rt_table,in_addr_t dstaddr,struct nextaddr *nexthopinfo);
int delete_route_s(struct routeTableNode *rt_table,in_addr_t dstaddr,unsigned int prefixlen);
