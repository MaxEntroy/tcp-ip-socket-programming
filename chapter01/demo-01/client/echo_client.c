#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "common/err.h"

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

  return 0;
}
