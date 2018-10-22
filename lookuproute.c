//
// Created by yaozh16 on 18-10-22.
//

#include "lookuproute.h"
int insert_route(struct route *rt_table,
                 unsigned long  ip4prefix,
                 unsigned int prefixlen,
                 char *ifname,
                 unsigned int ifindex,
                 unsigned long  nexthopaddr){
    struct route* new_route;
    new_route=(struct route*)malloc(sizeof(struct route));
}