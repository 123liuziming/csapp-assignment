#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "csapp.h"

/* Recommended max cache and object sizes */
#define MAX_CACHE_SIZE 1049000
#define MAX_OBJECT_SIZE 102400

/* You won't lose style points for including this long line in your code */
static const char *user_agent_hdr = "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:10.0.3) Gecko/20120305 Firefox/10.0.3\r\n";

int modify_headers(rio_t* rp, char* host, int* port);
void proxy(int fd);
void parse_uri(char* uri, char* port, char* path, char* host);
int proxy_request(char* port, char* path, char* host, int line_count);


char headers[MAXLINE][MAXLINE];
//解析uri
//uri格式类似于 http://www.cmu.edu:8080/hub/index.html
int modify_headers(rio_t* rp, char* host, int* port) {
    int i = 4;
    strcpy(headers[1], user_agent_hdr);
    strcpy(headers[2], "Connection: close\r\n");
    strcpy(headers[3], "Proxy-Connection: close\r\n");
    while (1) {
        Rio_readlineb(rp, headers[i], MAXLINE);
        if (!strcmp(headers[i], "\r\n")) {
            break;
        }
        char* split = strstr(headers[i], ":");
        const int n = split - headers[i] - 1;
        if ((!strncmp(headers[i], "Host", n)) || (!strncmp(headers[i], "User-Agent", n), n) || (!strncmp(headers[i], "Connection", n)) || (!strncmp(headers[i], "Proxy-Connection", n))) {
            //记录Host和端口号
            if (!strncmp(headers[i], "Host", n)) {
                char* uri = (char*) malloc(MAXLINE);
                strcpy(uri, headers[i] + strlen("Host: "));
                host = (char*) malloc(MAXLINE);
                strcpy(host, strtok(uri, ":"));
                char* port_str = strtok(NULL, ":");
                *port = port_str ? atoi(port_str) : 80;
                sprintf(headers[0], "Host: %s:%d\r\n", host, *port);
            }
            continue;
        }
        ++i;
    }
    return i;
}

int proxy_request(char* port, char* path, char* host, int line_count) {
    int clientfd = Open_clientfd(host, port);
    rio_t rio;
    char req[100];
    sprintf(req, "GET %s HTTP/1.0\r\n", path);
    Rio_writen(clientfd, req, strlen(req));
    int i;
    for (i = 0; i < line_count; ++i) {
        Rio_writen(clientfd, headers[i], strlen(headers[i]));
    }
    Rio_writen(clientfd, "\r\n", 2);
    return clientfd;
}

//读和解析请求行，并进行转发
void proxy(int fd) {
    rio_t rio;
    char buf[MAXLINE], method[MAXLINE], uri[MAXLINE], version[MAXLINE];
    Rio_readinitb(&rio, fd);
    //读一行
    Rio_readlineb(&rio, buf, MAXLINE);
    //格式类似GET /url HTTP/1.1
    sscanf(buf, "%s %s %s", method, uri, version);
    char* host;
    char port_str[10];
    int port;
    int line_count = modify_headers(&rio, host, &port);
    sprintf(port_str, "%d", port + 1);
    int response_fd = proxy_request(port_str, uri, host, line_count);
    Rio_readinitb(&rio, response_fd);
    int n;
    while ((n = Rio_readlineb(&rio, buf, MAXLINE))) {
        Rio_writen(fd, buf, n);
    }
    Close(response_fd);
}

int main(int argc, char** argv)
{
    char* port = argv[1];
    printf("The port of this proxy server is %s\n", port);
    //循环监听端口
    int listenfd, connfd, clientlen;
    struct sockaddr_in clientaddr;
    //DNS
    struct hostent* hp;
    listenfd = Open_listenfd(port);
    while (1) {
        clientlen = sizeof(clientaddr);
        connfd = Accept(listenfd, (struct sockaddr*) &clientaddr, &clientlen);
        //具体逻辑处理
        //TODO: 多线程
        proxy(connfd);
        Close(connfd);
    }
    return 0;
}
