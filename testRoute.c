//
// Created by yaozh16 on 18-12-26.
//
#include <stdio.h>
#include "routeTableSTree.h"
int printTable(struct routeTableNode* root){
    int curCount=0;
    int nextCount=0;
    struct routeTableNode* curs[100];
    struct routeTableNode* nexts[100];
    curCount=1;
    curs[0]=root;

    printf("Tree:\n");
    while(curCount>0) {
        nextCount=0;
        printf("\t>>>\n");
        for (int i = 0; i < curCount; i++) {
            printf("\t%s[%s](%d:%d)",inet_ntoa(curs[i]->iprefix),inet_ntoa(curs[i]->nexthopaddr),curs[i]->valid,curs[i]->prefixlen);
            if(i%6==5){
                printf("\n");
            }
            if(curs[i]->childs[0]!=NULL){
                nexts[nextCount++]=curs[i]->childs[0];
            }
            if(curs[i]->childs[1]!=NULL){
                nexts[nextCount++]=curs[i]->childs[1];
            }
        }
        curCount=nextCount;
        for(int i=0;i<curCount;i++){
            curs[i]=nexts[i];
        }

        printf("\n");
    }
    printf("\n");

}

int main(){
    struct routeTableNode* root=(struct routeTableNode*)malloc(sizeof(struct routeTableNode));
    root->childs[0]=NULL;
    root->childs[1]=NULL;
    printTable(root);
    insert_route_s(root,inet_addr("192.168.1.0"),24,"interface1",0,inet_addr("192.168.5.1"));
    printTable(root);
    insert_route_s(root,inet_addr("192.168.2.0"),24,"interface1",0,inet_addr("192.168.5.1"));
    printTable(root);
    insert_route_s(root,inet_addr("192.168.0.0"),22,"interface1",0,inet_addr("192.168.5.1"));
    printTable(root);
    insert_route_s(root,inet_addr("192.168.3.0"),24,"interface1",0,inet_addr("192.168.14.1"));
    printTable(root);
    insert_route_s(root,inet_addr("192.168.4.0"),24,"interface1",0,inet_addr("192.168.8.1"));
    printTable(root);
    insert_route_s(root,inet_addr("192.168.6.0"),24,"interface1",0,inet_addr("192.168.5.1"));
    printTable(root);
    struct nextaddr* nextaddr1=(struct nextaddr*)malloc(sizeof(struct nextaddr));

    //delete
    delete_route_s(root,inet_addr("192.168.2.0"),22);
    printTable(root);

    delete_route_s(root,inet_addr("192.168.1.1"),24);
    printTable(root);
    delete_route_s(root,inet_addr("192.168.2.1"),24);
    printTable(root);
    delete_route_s(root,inet_addr("192.168.3.1"),24);
    printTable(root);

}