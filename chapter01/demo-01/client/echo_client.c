#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "common/err.h"

#define BUF_SZ 128

int main(int argc, char* argv[]) {
  if(argc != 3) {
    err_handling(argv[0], "Usage:<ip><port>");
  }

  // socket
  int clnt_sfd = socket(PF_INET, SOCK_STREAM, 0);
  if(clnt_sfd == -1 ) {
    perr_handling("socket", "error");
  }

  // server address
  struct sockaddr_in serv_addr;
  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
  serv_addr.sin_port = htons(atoi(argv[2]));

  // connect
  int ret = connect(clnt_sfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr));
  if(ret == -1) {
    perr_handling("connect", "error");
  }

  // recv message from server
  char buf[BUF_SZ];
  read(clnt_sfd, buf, sizeof(buf));
  printf("%s\n", buf);

  close(clnt_sfd);
  return 0;
}
