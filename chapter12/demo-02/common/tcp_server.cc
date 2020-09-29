#include "tcp_server.h"

#include <poll.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
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

static void AddfdToPoll(struct pollfd** pfd_arr, int fd, int* sz, int* cap) {
  if (*sz == *cap) {
    *cap = 2 * *cap;
    printf("cap = %d.\n", *cap);
    (*pfd_arr) = static_cast<struct pollfd*>(realloc(*pfd_arr, (*cap) * sizeof(struct pollfd)));
    if (!pfd_arr) {
      err_handling("realloc", "error");
    }
  }

  (*pfd_arr)[*sz].fd = fd;
  (*pfd_arr)[*sz].events = POLLIN;

  (*sz)++;
}

static void RemovefdFromPoll(struct pollfd* pfd_arr, int idx, int* sz) {
  pfd_arr[idx] = pfd_arr[*sz - 1];
  (*sz)--;
}

void TcpServer::EventLoop() {
  // serving-loop
  struct sockaddr_in clnt_addr;
  memset(&clnt_addr, 0, sizeof(clnt_addr));
  socklen_t clnt_addr_len = 0;

  int fd_cap = kDefaultPollConnections;
  struct pollfd* pfd_arr = static_cast<struct pollfd*>(malloc(fd_cap * sizeof(*pfd_arr)));
  if (!pfd_arr) {
    err_handling("malloc", "error");
  }
  bzero(pfd_arr, fd_cap);

  int fd_sz = 1;
  pfd_arr[0].fd = listen_sfd_;
  pfd_arr[0].events = POLLIN;

  while(1) {
    printf("Tcp server[localhost:%d] waiting...\n", ntohs(serv_addr_.sin_port));

    int ret = poll(pfd_arr, fd_sz, kDefaultPollTimeout);
    if (ret == -1) {
      perr_handling("poll", "error");
    } else if (ret == 0) {
      continue;
    } else {
      for (int i = 0; i <= fd_sz; ++i) {
        if (pfd_arr[i].revents == POLLIN) {
          int sfd = pfd_arr[i].fd;
          if (sfd == listen_sfd_) {
            int clnt_sfd = accept(listen_sfd_, (struct sockaddr*) &clnt_addr, &clnt_addr_len);
            if (clnt_sfd == -1) {
              perr_handling("accept", "error");
            }

            // Add clnt_sfd to poll
            AddfdToPoll(&pfd_arr, clnt_sfd, &fd_sz, &fd_cap);
            printf("[%s:%d] connected.\n", inet_ntoa(clnt_addr.sin_addr), ntohs(clnt_addr.sin_port));
          } else {
            HandleIoEvent(sfd);
            close(sfd);

            // Remove clnt_sfd to select
            RemovefdFromPoll(pfd_arr, i, &fd_sz);
            printf("[%s:%d] disconnected.\n", inet_ntoa(clnt_addr.sin_addr), ntohs(clnt_addr.sin_port));
          }
        }
      }// for
    }
  }
  free(pfd_arr);
}

} // namespace utils
