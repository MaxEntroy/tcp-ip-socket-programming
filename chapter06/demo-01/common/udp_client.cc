#include "udp_client.h"

#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>

namespace utils {

UdpClient::UdpClient() : sfd_(socket(PF_INET, SOCK_DGRAM, 0)) {
  if (sfd_ == -1) {
    err_handling("socket", "error");
  }

  memset(&serv_addr_, 0 , sizeof(serv_addr_));
}

void UdpClient::Init(const char* ip, int port) {
}

void UdpClient::Run() {

}

} // namespace utils
