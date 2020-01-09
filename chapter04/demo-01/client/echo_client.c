#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "common/err.h"

static void echo_client(const char* ip, uint16_t port);

int main(int argc, char* argv[]) {
  if(argc != 3) {
    err_handling(argv[0], "error\nUsage:\t./echo_server ip port");
  }

  echo_client(argv[1], atoi(argv[2]));

  return 0;
}

void echo_client(const char* ip, uint16_t port) {
  // create a socket
  int clnt_sfd = socket(PF_INET, SOCK_STREAM, 0);
  if(clnt_sfd == -1) {
    perr_handling("socket", "error");
  }

  struct sockaddr_in serv_addr;
  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  inet_aton(ip, &(serv_addr.sin_addr));
  serv_addr.sin_port = htons(port);

  // connect to server
  int ret = connect(clnt_sfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr));
  if(ret == -1) {
    perr_handling("connect", "error");
  }
  printf("[INFO]:Connected to [%s:%u]\n", ip, port);

  // IO
  close(clnt_sfd);
}
