/*************************************************************************
> File Name: socket_server.c
> Author: 
> Mail: 
> Created Time: 2018年09月25日 星期二 19时13分13秒
************************************************************************/

#include<stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <errno.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <sys/types.h> 
#include <unistd.h> 
#include <sys/time.h> 

#define BUFLEN 1000
#define PORT 1028
#define LISTNUM 20
//#define host "192.168.1.156"
int main() {
    int a = 0;
    int pid;
    int sockfd, newfd; 
    struct sockaddr_in s_addr, c_addr;  
    socklen_t len; 
    int port, listnum; 
    
    /*建立socket*/ 
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) { 
        perror("socket"); 
        return -1;
    } 

    s_addr.sin_family = AF_INET; 
    s_addr.sin_port = htons(PORT); 
    s_addr.sin_addr.s_addr = htons(INADDR_ANY); 
    //s_addr.sin_addr.s_addr = inet_addr(host);
    
    /*把地址和端口帮定到套接字上*/ 
    if((bind(sockfd, (struct sockaddr*) &s_addr,sizeof(struct sockaddr))) < 0){ 
        perror("bind"); 
        return -1;
    }

    /*侦听本地端口*/ 
    if(listen(sockfd,listnum) < 0){ 
        perror("listen"); 
        exit(1);
    }

    while(1){ 
        printf("*****************聊天开始***************\n"); 
        len = sizeof(struct sockaddr); 
        if((newfd = accept(sockfd,(struct sockaddr*) &c_addr, &len)) < 0) { 
            perror("accept"); 
            exit(errno);
        }else 
        printf("正在与您聊天的客户端是：%s: %d\n",inet_ntoa(c_addr.sin_addr), ntohs(c_addr.sin_port));

            if((pid = fork()) < 0)
                printf("Error forking child process");
            if(pid == 0) {
                char buffer[BUFLEN];
                while((a = recv(newfd, buffer, BUFLEN, 0)) > 0) {
                    buffer[a] = '\0';   
                    printf("recv %d 字节 %s\n", a, buffer);
                    fflush(stdout);
                    memset(buffer, 0, sizeof(buffer));
                }
                printf("\n");
                close(newfd);
                exit(0);
            }
    }

        close(sockfd);
    return 0;
}
