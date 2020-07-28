#include "echo_server.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>

#define BUF_SZ 128

namespace echo {

void EchoServer::HandleIoEvent(int sfd) {
  char buf[BUF_SZ];

  struct sockaddr_in clnt_addr;
  socklen_t clnt_len = sizeof(clnt_addr);
  memset(&clnt_addr, 0, clnt_len);

  int nrecv= recvfrom(sfd, buf, BUF_SZ, 0, (struct sockaddr*)&clnt_addr, &clnt_len);

  sendto(sfd, buf, nrecv, 0, (struct sockaddr*)&clnt_addr, clnt_len);
}

} // namespace echo
