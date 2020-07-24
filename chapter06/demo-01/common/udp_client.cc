#include "udp_client.h"

#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "err.h"

namespace utils {

UdpClient::UdpClient() : sfd_(socket(PF_INET, SOCK_DGRAM, 0)) {
  if (sfd_ == -1) {
    err_handling("socket", "error");
  }

  memset(&serv_addr_, 0 , sizeof(serv_addr_));
}

void UdpClient::Init(const char* ip, int port) {
  serv_addr_.sin_family = AF_INET;
  inet_aton(ip, &(serv_addr_.sin_addr));
  serv_addr_.sin_port = htons(port);
}

void UdpClient::Run() {
  HandleIoEvent(sfd_);
}

} // namespace utils
