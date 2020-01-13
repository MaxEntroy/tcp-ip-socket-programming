#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "common/err.h"
#include "common/log.h"

#define TRUE 1
#define BACKLOG 15
#define BUF_SZ 128

/* echo server */
static void echo_server(uint16_t port, pzlog log_handler);

/* event handler */
static void do_io_event(int clnt_sfd);

int main(int argc, char* argv[]) {
  if(argc != 4) {
    err_handling(argv[0], "error\nUsage:\t./echo_server port log_path log_cat");
  }

  pzlog log_handler = LOG_INIT(argv[2], argv[3]);

  echo_server(atoi(argv[1]), log_handler);

  LOG_FINI();
  return 0;
}

void echo_server(uint16_t port, pzlog log_handler) {
  // create a socket
  int serv_sfd = socket(PF_INET, SOCK_STREAM, 0);
  if(serv_sfd == -1) {
    perr_handling("socket", "error");
  }

  // bind
  struct sockaddr_in serv_addr;
  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(port);

  int ret = bind(serv_sfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr));
  if(ret == -1) {
    perr_handling("bind", "error");
  }

  // listen
  ret = listen(serv_sfd, BACKLOG);
  if(ret == -1) {
    perr_handling("listen", "error");
  }
  // serving-loop
  struct sockaddr_in clnt_addr;
  memset(&clnt_addr, 0, sizeof(clnt_addr));
  socklen_t clnt_addr_len = 0;
  while(TRUE) {
    LOG(log_handler, INFO, "Echo Server[localhost:%u] waiting...", port);

    // accept
    int clnt_sfd = accept(serv_sfd, (struct sockaddr*) &clnt_addr, &clnt_addr_len);
    LOG(log_handler, INFO, "[%s:%u] connected.", inet_ntoa(clnt_addr.sin_addr), ntohs(clnt_addr.sin_port));

    // IO
    do_io_event(clnt_sfd);

    close(clnt_sfd);
    LOG(log_handler, INFO, "[%s:%u] disconnected.", inet_ntoa(clnt_addr.sin_addr), ntohs(clnt_addr.sin_port));
  }

  close(serv_sfd);
}

void do_io_event(int clnt_sfd) {
  char buf[BUF_SZ];
  int nread = 0;

  while((nread = read(clnt_sfd, buf, BUF_SZ)) > 0) {
    write(clnt_sfd, buf, nread);
  }
}
