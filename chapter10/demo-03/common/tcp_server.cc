#include "tcp_server.h"

#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include "err.h"

namespace utils {

TcpServer::TcpServer() : listen_sfd_(socket(PF_INET, SOCK_STREAM, 0)) {
  if(listen_sfd_ == -1) {
    perr_handling("socket", "error");
  }

  memset(&serv_addr_, 0, sizeof(serv_addr_));
}

static void child_handler(int) {
  int status = 0;
  waitpid(-1, &status, WNOHANG);
}

static void register_child_async_event() {
  struct sigaction child_event;
  child_event.sa_handler = &child_handler;
  child_event.sa_flags = 0;
  sigemptyset(&child_event.sa_mask);

  sigaction(SIGCHLD, &child_event, NULL);
}

void TcpServer::Init(int port, int backlog) {
  // set reuse addr
  int option = 1;
  socklen_t optlen = sizeof(option);
  int ret = setsockopt(listen_sfd_, SOL_SOCKET, SO_REUSEADDR, &option, optlen);
  if (ret == -1) {
    perr_handling("setsockopt", "error");
  }

  // bind
  serv_addr_.sin_family = AF_INET;
  serv_addr_.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr_.sin_port = htons(port);

  ret = bind(listen_sfd_, (struct sockaddr*) &serv_addr_, sizeof(serv_addr_));
  if(ret == -1) {
    perr_handling("bind", "error");
  }

  // listen
  ret = listen(listen_sfd_, backlog);
  if(ret == -1) {
    perr_handling("listen", "error");
  }

  // register child event
  register_child_async_event();
}

void TcpServer::EventLoop() {
  // serving-loop
  struct sockaddr_in clnt_addr;
  memset(&clnt_addr, 0, sizeof(clnt_addr));
  socklen_t clnt_addr_len = 0;

  while(1) {
    printf("Tcp server[localhost:%d] waiting...\n", ntohs(serv_addr_.sin_port));

    // accept
    int clnt_sfd = accept(listen_sfd_, (struct sockaddr*) &clnt_addr, &clnt_addr_len);
    if (clnt_sfd == -1) {
      continue;
    }
    printf("[%s:%d] connected.\n", inet_ntoa(clnt_addr.sin_addr), ntohs(clnt_addr.sin_port));

    pid_t pid = fork();
    if (pid == -1) {
      close(clnt_sfd);
      continue;
    }

    if (!pid) {
      // IO
      HandleIoEvent(clnt_sfd);

      close(clnt_sfd);
      printf("[%s:%d] disconnected.\n", inet_ntoa(clnt_addr.sin_addr), ntohs(clnt_addr.sin_port));
      return;
    } else {
      close(clnt_sfd);
    }
  }
}

} // namespace utils
