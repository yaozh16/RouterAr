//
// Created by yaozh16 on 18-12-26.
//
#include "routeTableSTree.h"



struct routeTableNode* createNode(uint32_t  ip4prefix,
                                  unsigned int prefixlen,
                                  char *ifname,
                                  unsigned int ifindex,
                                  uint32_t  nexthopaddr){

    struct routeTableNode* tem=(struct routeTableNode*)malloc(sizeof(struct routeTableNode));
    tem->childs[0]=NULL;
    tem->childs[1]=NULL;
    tem->valid=1;
    tem->iprefix.s_addr=ip4prefix;
    tem->prefixlen=prefixlen;
    tem->nexthopaddr.s_addr=nexthopaddr;
    tem->ifindex=ifindex;
    strcpy(tem->ifname,ifname);
    tem->mask=htonl(0xffffffff<<(32-prefixlen));

    return tem;
}

uint32_t commonPrefixLength(struct routeTableNode* node1,struct routeTableNode* node2){
    //common parent
    uint32_t prefixlen=node1->prefixlen;
    uint32_t mask=htonl(0xffffffff<<(32-prefixlen));
    while((node1->iprefix.s_addr&mask)!=(node2->iprefix.s_addr&mask)){
        prefixlen=prefixlen-1;
        mask=htonl(0xffffffff<<(32-prefixlen));
    }
    return prefixlen;
}
struct routeTableNode* mergeNodes(struct routeTableNode* node1,struct routeTableNode* node2){
    //common parent


    struct routeTableNode* tem=(struct routeTableNode*)malloc(sizeof(struct routeTableNode));
    tem->childs[0]=NULL;
    tem->childs[1]=NULL;
    tem->valid=0;
    tem->prefixlen=commonPrefixLength(node1,node2);
    tem->mask=htonl(0xffffffff<<(32-tem->prefixlen));
    tem->iprefix.s_addr=node1->iprefix.s_addr&tem->mask;

    tem->childs[0]=node1;
    tem->childs[1]=node2;

    return tem;
}


int insert_route_t(struct routeTableNode *root,
                   uint32_t  ip4prefix,
                   unsigned int prefixlen,
                   char *ifname,
                   unsigned int ifindex,
                   uint32_t  nexthopaddr){
    struct routeTableNode* newNode=createNode(ip4prefix,prefixlen,ifname,ifindex,nexthopaddr);

    struct routeTableNode* curnode=root;
    struct routeTableNode* tem;
    while(curnode!=NULL){
        if(curnode->childs[0]==NULL){
            //no child:insert
            curnode->childs[0]=newNode;
            return 0;
        } else{
            tem=curnode->childs[0];
            if(tem->prefixlen < newNode->prefixlen){
                if((tem->iprefix.s_addr&tem->mask)==(newNode->iprefix.s_addr&tem->mask)){
                    curnode=tem;
                    continue;
                } else{
                    //search child[1]
                }
            } else if(tem->prefixlen > newNode->prefixlen){
                if((tem->iprefix.s_addr&newNode->mask)==(newNode->iprefix.s_addr&newNode->mask)){
                    //original node
                    curnode->childs[0]=newNode;
                    newNode->childs[0]=tem;
                    if(curnode->childs[1]!=NULL && commonPrefixLength(newNode,curnode->childs[1])==newNode->prefixlen){
                        newNode->childs[1]=curnode->childs[1];
                        curnode->childs[1]=NULL;
                    }
                    return 0;
                } else{
                    //search child[1]
                }
            } else{//tem->prefixlen==prefixlen
                if((tem->iprefix.s_addr&tem->mask)==(newNode->iprefix.s_addr&tem->mask)){
                    //update
                    tem->valid=1;
                    tem->iprefix.s_addr=newNode->iprefix.s_addr;
                    tem->nexthopaddr.s_addr=newNode->nexthopaddr.s_addr;
                    strcpy(tem->ifname,newNode->ifname);
                    tem->ifindex=newNode->ifindex;
                    tem->mask=newNode->mask;
                    free(newNode);
                    return 0;
                } else{
                    //search child[1]
                }
            }
        }

        //search child[1]
        if(curnode->childs[1]==NULL){
            //no child:insert
            curnode->childs[1]=newNode;
            return 0;
        } else{
            tem=curnode->childs[1];
            if(tem->prefixlen < newNode->prefixlen){
                if((tem->iprefix.s_addr&tem->mask)==(newNode->iprefix.s_addr&tem->mask)){
                    curnode=tem;
                    continue;
                } else{
                    //merge 3 childs to 2
                }
            } else if(tem->prefixlen > newNode->prefixlen){
                if((tem->iprefix.s_addr&newNode->mask)==(newNode->iprefix.s_addr&newNode->mask)){
                    //original node
                    curnode->childs[1]=newNode;
                    newNode->childs[1]=tem;
                    return 0;
                } else{
                    //merge 3 childs to 2
                }
            }else {//tem->prefixlen==prefixlen
                if ((tem->iprefix.s_addr & tem->mask) == (newNode->iprefix.s_addr & tem->mask)) {
                    //update
                    tem->valid = 1;
                    tem->iprefix.s_addr = newNode->iprefix.s_addr;
                    tem->nexthopaddr.s_addr = newNode->nexthopaddr.s_addr;
                    strcpy(tem->ifname, newNode->ifname);
                    tem->ifindex= newNode->ifindex;
                    tem->mask= newNode->mask;
                    free(newNode);
                    return 0;
                } else {
                    //merge 3 childs to 2
                }
            }
        }
        //merge 3 childs to 2
        //printf("\033[1;32mmerge\033[0m\n");
        if(curnode->childs[0]->prefixlen==curnode->prefixlen-1){
            //keep child[0]
            curnode->childs[1]=mergeNodes(newNode,curnode->childs[1]);
        } else if(curnode->childs[1]->prefixlen==curnode->prefixlen-1){
            //keep child[1]
            curnode->childs[0]=mergeNodes(newNode,curnode->childs[0]);
        } else{
            uint32_t M1=commonPrefixLength(curnode->childs[0],curnode->childs[1]);
            uint32_t M2=commonPrefixLength(curnode->childs[0],newNode);
            uint32_t M3=commonPrefixLength(curnode->childs[1],newNode);

            //printf("\033[1;32mmerge %d %d %d \033[0m\n",M1,M2,M3);
            if(M1>=M2&&M1>=M3){
                curnode->childs[0]=mergeNodes(curnode->childs[0],curnode->childs[1]);
                curnode->childs[1]=newNode;
            } else if(M2>=M1 && M2>=M3){
                curnode->childs[0]=mergeNodes(curnode->childs[0],newNode);
            } else{
                curnode->childs[1]=mergeNodes(curnode->childs[1],newNode);
            }
        }
        //merge done
        return 0;
    }
}
int find_route_t(struct routeTableNode *root,in_addr_t dstaddr,struct nextaddr *nexthopinfo){
    struct routeTableNode *curNode=root;
    struct routeTableNode *hitNode=NULL;
    while(curNode!=NULL){
        if(curNode->childs[0]!=NULL){
            if((curNode->childs[0]->iprefix.s_addr&curNode->childs[0]->mask)== (dstaddr&curNode->childs[0]->mask)){
                //possible hit!
                curNode=curNode->childs[0];
                if(curNode->valid==1)
                    hitNode=curNode;
                continue;
            } else{
                //test child[1]
            }
        }
        if(curNode->childs[1]!=NULL){
            if((curNode->childs[1]->iprefix.s_addr&curNode->childs[1]->mask)== (dstaddr&curNode->childs[1]->mask)){
                //possible hit!
                curNode=curNode->childs[1];
                if(curNode->valid==1)
                    hitNode=curNode;
                continue;
            } else{
                //miss!
            }
        }
        //miss!
        break;
    }
    if(hitNode!=NULL){
        //hit!
        nexthopinfo->ipv4addr.s_addr=hitNode->nexthopaddr.s_addr;
        nexthopinfo->prefixl=hitNode->prefixlen;
        strcpy(nexthopinfo->ifname,hitNode->ifname);
        return 0;
    } else{
        return -1;
    }
}
int delete_route_t(struct routeTableNode *root,in_addr_t dstaddr,unsigned int prefixlen){
    struct routeTableNode *curNode=root;
    struct routeTableNode *hitNode=NULL;
    struct routeTableNode* stack[33];
    int stackCount=0;
    int hitStackCount=0;
    while(curNode!=NULL&&prefixlen>curNode->prefixlen){
        if(curNode->childs[0]!=NULL){
            /*struct in_addr tem;
            tem.s_addr=dstaddr;
            printf("\033[1;32m [node]%s",inet_ntoa(curNode->iprefix));
            printf(" [cur]%s\033[0m\n",inet_ntoa(tem));
            */
            if((curNode->childs[0]->iprefix.s_addr&curNode->childs[0]->mask)== (dstaddr&curNode->childs[0]->mask)){
                //hit!
                //printf("\033[1;32m%s(%d)\033[0m\n",inet_ntoa(curNode->childs[0]->iprefix),curNode->prefixlen);
                stack[stackCount]=curNode;
                curNode=curNode->childs[0];
                if(curNode->valid==1) {
                    hitNode = curNode;
                    hitStackCount=stackCount;
                }
                stackCount++;
                continue;
            } else{
                //test child[1]
            }
        }
        if(curNode->childs[1]!=NULL){
            if((curNode->childs[1]->iprefix.s_addr&curNode->childs[1]->mask)== (dstaddr&curNode->childs[1]->mask)){
                //hit!
                //printf("\033[1;32m%s(%d)\033[0m\n",inet_ntoa(curNode->childs[1]->iprefix),curNode->prefixlen);
                stack[stackCount]=curNode;
                curNode=curNode->childs[1];
                if(curNode->valid==1) {
                    hitNode = curNode;
                    hitStackCount=stackCount;
                }
                stackCount++;
                continue;
            } else{
                //miss!
            }
        }

        //printf("\033[1;32m miss !\033[0m\n");
        //miss!
        break;
    }
    //exit(0);
    if(hitNode!=NULL&&hitNode->prefixlen==prefixlen){
        //delete route

        if(hitNode->childs[0]!=NULL || hitNode->childs[1]!=NULL){//not leaf
            hitNode->valid=0;
        } else {
            for (int i = hitStackCount; i >= 0; i--) {//leaf
                //printf("\033[1;32m[parent]%s\033[0m\n",inet_ntoa(stack[i]->iprefix));
                if (stack[i]->childs[0] == hitNode) {
                    //printf("\033[1;32m[childL]%s\033[0m\n",inet_ntoa(stack[i]->iprefix));
                    free(hitNode);
                    stack[i]->childs[0] =NULL;
                    if(stack[i]->childs[1]!=NULL||stack[i]->valid==1){
                        //keep it
                        return 0;
                    } else{
                        //delete recrusive
                        hitNode=stack[i];
                    }
                } else if (stack[i]->childs[1] == hitNode) {
                    //printf("\033[1;32m[childR]%s\033[0m\n",inet_ntoa(stack[i]->iprefix));
                    free(hitNode);
                    stack[i]->childs[1] =NULL;
                    if(stack[i]->childs[0]!=NULL||stack[i]->valid==1){
                        //keep it
                        return 0;
                    } else{
                        //delete recrusive
                        hitNode=stack[i];
                    }
                } else {
                    printf("route not found!\n");
                }
            }
        }
    }
}



int insert_route_s(struct routeTableNode *root,
                   uint32_t  ip4prefix,
                   unsigned int prefixlen,
                   char *ifname,
                   unsigned int ifindex,
                   uint32_t  nexthopaddr){
    pthread_mutex_lock(&mutex);
    int ret=insert_route_t(root,ip4prefix,prefixlen,ifname,ifindex,nexthopaddr);
    pthread_mutex_unlock(&mutex);
    return ret;
}

int find_route_s(struct routeTableNode *rt_table,in_addr_t dstaddr,struct nextaddr *nexthopinfo){
    pthread_mutex_lock(&mutex);
    int ret=find_route_t(rt_table,dstaddr,nexthopinfo);
    pthread_mutex_unlock(&mutex);
    return ret;
}
int delete_route_s(struct routeTableNode *rt_table,in_addr_t dstaddr,unsigned int prefixlen){
    pthread_mutex_lock(&mutex);
    int ret=delete_route_t(rt_table,dstaddr,prefixlen);
    pthread_mutex_unlock(&mutex);
    return ret;
}