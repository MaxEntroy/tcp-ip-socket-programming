#include "tcp_server.h"

#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/wait.h>

#include <algorithm>

#include "err.h"

namespace utils {

TcpServer::TcpServer() : listen_sfd_(socket(PF_INET, SOCK_STREAM, 0)) {
  if(listen_sfd_ == -1) {
    perr_handling("socket", "error");
  }

  memset(&serv_addr_, 0, sizeof(serv_addr_));
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
}

void TcpServer::EventLoop() {
  // serving-loop
  struct sockaddr_in clnt_addr;
  memset(&clnt_addr, 0, sizeof(clnt_addr));
  socklen_t clnt_addr_len = 0;

  fd_set reads, cpy_reads;
  FD_ZERO(&reads);
  FD_SET(listen_sfd_, &reads);

  int max_sfd = listen_sfd_;
  struct timeval timeout;

  while(1) {
    printf("Tcp server[localhost:%d] waiting...\n", ntohs(serv_addr_.sin_port));

    cpy_reads = reads;
    timeout.tv_sec = 5;
    timeout.tv_usec = 0;
    int ret = select(max_sfd + 1, &cpy_reads, NULL, NULL, &timeout);
    if (ret == -1) {
      perr_handling("select", "error");
    } else if (ret == 0) {
      continue;
    } else {
      for (int sfd = 0; sfd <= max_sfd; ++sfd) {
        if (!FD_ISSET(sfd, &cpy_reads)) {
          continue;
        }
        // if: listen_sfd io event
        // else: other sfd io event
        if (sfd == listen_sfd_) {
          int clnt_sfd = accept(listen_sfd_, (struct sockaddr*) &clnt_addr, &clnt_addr_len);
          if (clnt_sfd == -1) {
            perr_handling("accept", "error");
          }

          //Add clnt_sfd to select
          FD_SET(clnt_sfd, &reads);
          max_sfd = std::max(clnt_sfd, max_sfd);

          printf("[%s:%d] connected.\n", inet_ntoa(clnt_addr.sin_addr), ntohs(clnt_addr.sin_port));
        } else {
          HandleIoEvent(sfd);
          close(sfd);

          // Remove clnt_sfd to select
          FD_CLR(sfd, &reads);
          max_sfd = (max_sfd == sfd)?(max_sfd - 1):max_sfd;
          printf("[%s:%d] disconnected.\n", inet_ntoa(clnt_addr.sin_addr), ntohs(clnt_addr.sin_port));
        }
      }
    }
  }
}

} // namespace utils
