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
    pthread_mutex_lock(&mutex);
    struct route* new_route;
    new_route=(struct route*)malloc(sizeof(struct route));
    if(new_route==NULL){
            printf("malloc error!!\n");
            return -1;
    }
    new_route->iprefix.s_addr=ip4prefix;
    new_route->prefixlen=prefixlen;
    new_route->nexthop=(struct nexthop*)malloc(sizeof(struct nexthop));
    new_route->nexthop->ifindex=ifindex;
    new_route->nexthop->ifname=(char*)malloc(sizeof(char)*strlen(ifname)+1);
    strcpy(new_route->nexthop->ifname,ifname);
    new_route->nexthop->nexthopaddr.s_addr=nexthopaddr;

    while (rt_table->next){
        if(rt_table->next->prefixlen>prefixlen){
            rt_table=rt_table->next;
        } else{
            break;
        }
    }
    new_route->next=rt_table->next;
    if(rt_table->next)
        rt_table->next->prev=new_route;
    rt_table->next=new_route;
    new_route->prev=rt_table;

    pthread_mutex_unlock(&mutex);
}
int find_route(struct route *rt_table,struct in_addr dstaddr,struct nextaddr *nexthopinfo){

    pthread_mutex_lock(&mutex);
    //dst addr :32 bit
    while (rt_table->next){
        unsigned  int mask=htonl(0xffffffff<<(32-rt_table->next->prefixlen));
        if((rt_table->next->iprefix.s_addr&mask)==(dstaddr.s_addr&mask)){
            strcpy(nexthopinfo->ifname,rt_table->next->nexthop->ifname);
            nexthopinfo->ipv4addr.s_addr=rt_table->nexthop->nexthopaddr.s_addr;
            pthread_mutex_unlock(&mutex);
            return 0;
        } else{
            rt_table=rt_table->next;
        }

    }

    pthread_mutex_unlock(&mutex);
    return -1;

}
int delete_route(struct route *rt_table,struct in_addr dstaddr,unsigned int prefixlen){

    pthread_mutex_lock(&mutex);
    unsigned  int mask=htonl(0xffffffff<<(32-prefixlen));
    while (rt_table->next){
        if(rt_table->next->prefixlen<prefixlen){
            rt_table=rt_table->next;
        } else if((rt_table->next->prefixlen==prefixlen) && ((rt_table->iprefix.s_addr&mask)==(dstaddr.s_addr&mask))){
            struct route* tmp=rt_table->next;
            rt_table->next=rt_table->next->next;
            if(rt_table->next){
                rt_table->next->prev=rt_table;
            }
            free(tmp->nexthop->ifname);
            free(tmp);
            pthread_mutex_unlock(&mutex);
            return 0;
        } else{
            pthread_mutex_unlock(&mutex);
            return -1;
        }
    }
}