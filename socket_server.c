/*************************************************************************
	> File Name: socket_server.c
	> Author: 
	> Mail: 
	> Created Time: 2018年09月24日 星期一 11时29分59秒
 ************************************************************************/

#include <arpa/inet.h>
#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/file.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <stdarg.h>

#define PORT 1028  //端口号
#define LISTNUM 100   //最大监听数
#define BUFFLEN 1024 //消息最大字节数

int main() {
    
    int sockfd, newfd;
    struct sockaddr_in s_addr, c_addr;
    char buf[BUFFLEN];
    socklen_t len;
    unsigned int port, listnum;
    fd_set rfds;  //文字个数
    //struct timeavl tv;
    int retval;
    int maxfd;

    //建立socket
    if((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(errno);
    } else {
        printf("socket create success!\n");
    }

    memset(&s_addr, 0, sizeof(s_addr));
    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons(PORT);
    s_addr.sin_addr.s_addr = htons(INADDR_ANY);

    //把地址和端口绑定到套接字上
    if((bind(sockfd, (struct sockaddr*) &s_addr, sizeof(struct sockaddr))) == -1) {
        perror("bind");
        exit(errno);
    } else {
        printf("bind success!\n");
    }

    //监听
    if(listen(sockfd, listnum) == -1) {
        perror("listen");
        exit(errno);
    } else {
        printf("listening now!\n");
    }

    while(1) {
        printf("----chat list----\n");
        len = sizeof(struct sockaddr);
        if((newfd = accept(sockfd,(struct sockaddr*) &c_addr, &len)) == -1) {
            perror("accept");
            exit(errno);
        } else {
            printf("已连接IP地址 ：%s : %d\n", inet_ntoa(c_addr.sin_addr), ntohs(c_addr.sin_port));
        }

        while(1) {
            FD_ZERO(&rfds);
            FD_SET(0, &rfds);
            maxfd = 0;
            FD_SET(newfd, &rfds);
            
            if(maxfd < newfd) {
                maxfd = newfd;
            }

            retval = select(maxfd + 1, &rfds, NULL, NULL, 0);
            if(retval == -1) {
                perror("select");
                exit(errno);
            } else {

            //用户发送信息
            if(FD_ISSET(0, &rfds)) {
                memset(buf, 0, sizeof(buf));
                //fget函数: 从流中读取BUFFLEN-1个字符
                fgets(buf, BUFFLEN, stdin);
                //打印消息
                fputs(buf, stdout);

                if(!strncasecmp(buf, "quit", 1)) {
                    printf("server 请求终止聊天\n");
                    break;
                }
                    len = send(newfd, buf, strlen(buf), 0);
                if(len > 0) {
                    printf("\t发送成功 ：%s\n", buf);
                } else {
                    printf("发送失败\n");
                    break;
                }
            }

            //客户端发来消息
            if(FD_ISSET(newfd, &rfds)) {
                memset(buf, 0, sizeof(buf));
                len = recv(newfd, buf, BUFFLEN, 0);
                if(len > 0) {
                    printf("客户端发来的消息 ： %s\n", buf);
                } else if(len < 0) {
                    printf("接收失败\n");
                } else {
                    printf("客户端退出，结束聊天\n");
                }
                break;
            }

        }
}

        //关闭聊天套接字
        close(newfd);
        printf("是否退出(y/n)");
        bzero(buf, BUFFLEN);
        fgets(buf, BUFFLEN, stdin);
        if(!strncasecmp(buf, "y", 1)) {
            printf("server 退出\n");
            break;
        }
    }

    close(sockfd);
    return 0;
}

