#include "echo_client.h"

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>

#define BUF_SZ 128

namespace echo {

void EchoClient::HandleIoEvent(int sfd, const sockaddr_in& serv_addr) {
  const char* msg = "hello, echo server, i am echo client.";
  char buf[BUF_SZ];

  struct sockaddr_in from_addr;
  socklen_t from_len = sizeof(from_addr);
  memset(&from_addr, 0, from_len);

  sendto(sfd, msg, strlen(msg), 0, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
  int nrecv = recvfrom(sfd, buf, BUF_SZ, 0, (struct sockaddr*)&from_addr, &from_len);

  buf[nrecv] = '\0';
  printf("%s\n", buf);
}

} // namespace echo
