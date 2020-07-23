#include "udp_server.h"

#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "err.h"

namespace utils {

UdpServer::UdpServer() : sfd_(socket(PF_INET, SOCK_DGRAM, 0)) {
  if (sfd_ == -1) {
    err_handling("socket", "error");
  }

  memset(&serv_addr_, 0 , sizeof(serv_addr_));
}

void UdpServer::Init(int port) {
  // bind
  serv_addr_.sin_family = AF_INET;
  serv_addr_.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr_.sin_port = htons(port);

  int ret = bind(sfd_, (struct sockaddr*) &serv_addr_, sizeof(serv_addr_));
  if(ret == -1) {
    err_handling("bind", "error");
  }
}

void UdpServer::EventLoop() {
  while(1) {
    HandleIoEvent(sfd_);
  }
}


} // namespace utils
