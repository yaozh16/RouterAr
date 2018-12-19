//
// Created by yaozh16 on 18-11-14.
//
#include <stdio.h>
#include "checksum.h"
int check_sum(unsigned short *iphd,int len,unsigned short checksum){
    unsigned int data=0;
    for(int i=0;i<len*2;i++){
        data+=iphd[i];
    }
    data=((data>>16)+data);
    data=(~data)&65535;
    /*printf("checksum:");
    int tem=data;
    for(int i=15;i>=0;i--){
        printf("%d",(tem>>i)&1);
    }
    printf("\n");*/
    return (data==0)?1:0;
}
unsigned short count_check_sum(unsigned short *iphd){
    struct _iphdr* header=(struct _iphdr*)iphd;
    header->ttl-=1;
    header->checksum+=1;
    return header->ttl;
}
