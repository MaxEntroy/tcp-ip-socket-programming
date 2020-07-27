#include "echo_server.h"

#define BUF_SZ 128

namespace echo {

void EchoServer::HandleIoEvent(int sfd) {
  char buf[BUF_SZ];
  int nread = 0;
  struct sockaddr_in clnt_addr;
  socklen_t clnt_len = 0;

  while((nread = recvfrom(sfd, buf, BUF_SZ, 0, (struct sockaddr*)&clnt_addr, &clnt_len))) {
    sendto(sfd, buf, nread, 0, (struct sockaddr*)&clnt_addr, clnt_len);
  }

}

} // namespace echo
