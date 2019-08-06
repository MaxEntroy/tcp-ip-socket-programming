#include "inc/webserver.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

#include <iostream>
#include <string>

void webserver(const std::string& ip, int port, int backlog) {
    int serv_sock = -1;
    struct sockaddr_in serv_addr;
    int ret = 0;

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(ip.c_str());
    serv_addr.sin_port = htons(port);

    ret = bind(serv_sock, reinterpret_cast<struct sockaddr*>(&serv_addr), sizeof(serv_addr));
    if(ret == -1)
        error_handling("bind() error");

    ret = listen(serv_sock, backlog);
    if(ret == -1)
        error_handling("listen() error");

    int clnt_sock = -1;
    struct sockaddr_in clnt_addr;
    socklen_t clnt_adr_size = sizeof(clnt_addr);
    while(true) {
        std::cout << "Server waiting[" << ip << ":" << port << "]..." << std::endl;
        clnt_sock = accept(serv_sock, reinterpret_cast<struct sockaddr*>(&clnt_addr), &clnt_adr_size);
        std::cout << "Connection Request : ["
            << inet_ntoa(clnt_addr.sin_addr) << ":" << ntohs(clnt_addr.sin_port) << "]"
            << std::endl;
        close(clnt_sock);
    }

    close(serv_sock);
}

void error_handling(const std::string& msg) {
    std::cerr << msg << std::endl;
    exit(EXIT_FAILURE);
}
