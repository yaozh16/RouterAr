//
// Created by yaozh16 on 18-10-22.
//
#include "recvroute.h"
#define MAXLINE 200
void logColor(const char* msg){
    printf("\033[1;34m%s\033[0m\n",msg);
}
int listenfd;
int static_route_get(struct selfroute* route){
    struct sockaddr_in cliaddr;
    socklen_t cliaddr_len;
    int connfd;
    cliaddr_len = sizeof(cliaddr);
    connfd = accept(listenfd,(struct sockaddr *)&cliaddr,&cliaddr_len);
    //如果得不到客户端发来的消息，将会被阻塞，一直等到消息到来
    ssize_t n = read(connfd,route,sizeof(struct selfroute));
    close(connfd);
    return n>0?1:0;
}
int startListen(){
    struct sockaddr_in servaddr;
    listenfd = socket(AF_INET,SOCK_STREAM,0);
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);   //因为本机地址可以有很多种表示，htonl只是为了保险起见，可以去掉
    servaddr.sin_port = htons(800);

    bind(listenfd,(struct sockaddr *)&servaddr,sizeof(servaddr));  //端口绑定
    logColor("start listen\n");
    listen(listenfd,20);//监听端口，20表示最大阻塞
}