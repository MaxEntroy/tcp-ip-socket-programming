#include "tcp_client.h"

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "err.h"

namespace utils {

TcpClient::TcpClient() : clnt_sfd_(socket(PF_INET, SOCK_STREAM, 0)) {
  if (clnt_sfd_ == -1) {
    err_handling("socket", "error");
  }

  memset(&serv_addr_, 0, sizeof(serv_addr_));
}

void TcpClient::Init(const char* ip, int port) {
  serv_addr_.sin_family = AF_INET;
  inet_aton(ip, &(serv_addr_.sin_addr));
  serv_addr_.sin_port = htons(port);

  // Connect to server
  int ret = connect(clnt_sfd_, (struct sockaddr*) &serv_addr_, sizeof(serv_addr_));
  if(ret == -1) {
    err_handling("connect", "error");
  }
  printf("Connected to [%s:%d].\n", ip, port);
}

void TcpClient::Run() {
  HandleIoEvent();
}

} // namespace utils
