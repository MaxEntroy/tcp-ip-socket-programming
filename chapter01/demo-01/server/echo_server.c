#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "common/err.h"

#define BACKLOG 5

int main(int argc, char* argv[]) {
  if(argc != 3) {
    err_handling(argv[0], "Usage:<ip><port>");
  }

  // socket
  int serv_sfd = socket(PF_INET, SOCK_STREAM, 0);
  if(serv_sfd == -1) {
    perr_handling("socket", "error");
  }

  // bind
  struct sockaddr_in serv_addr;
  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
  serv_addr.sin_port = htons(atoi(argv[2]));

  int ret = bind(serv_sfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr));
  if(ret == -1) {
    perr_handling("bind", "error");
  }

  // listen
  ret = listen(serv_sfd, BACKLOG);
  if(ret == -1) {
    perr_handling("listen", "error");
  }

  // accept
  struct sockaddr_in clnt_addr;
  memset(&clnt_addr, 0, sizeof(clnt_addr));
  socklen_t clnt_addr_sz = sizeof(clnt_addr);

  while(1) {
    printf("[%s:%s] listening...\n", argv[1], argv[2]);

    int clnt_sfd = accept(serv_sfd, (struct sockaddr*) &clnt_addr, &clnt_addr_sz);
    if(clnt_sfd == -1) {
      perr_handling("accept", "error");
    }
    printf("[%s:%u] connected!\n", inet_ntoa(clnt_addr.sin_addr), clnt_addr.sin_port);

    // send message to client
    const char message[] = "hello,world!";
    write(clnt_sfd, &message, sizeof(message));

    close(clnt_sfd);
    printf("[%s:%u] disonnected!\n", inet_ntoa(clnt_addr.sin_addr), clnt_addr.sin_port);
  }

  close(serv_sfd);
  return 0;
}
