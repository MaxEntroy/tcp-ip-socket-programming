#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>

#include "common/err.h"

#define BUF_SZ 128

static void echo_client(const char* ip, uint16_t port);
static void do_io_event(int clnt_sfd);
static ssize_t Read(int fd, void* buf, size_t count);

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
  do_io_event(clnt_sfd);

  close(clnt_sfd);
}

void do_io_event(int clnt_sfd) {
  const char* msg = "hello, echo server, i am echo client.";
  size_t msg_len = strlen(msg);
  char buf[BUF_SZ];

  write(clnt_sfd, msg, msg_len);
  Read(clnt_sfd, buf, msg_len);
  buf[msg_len] = '\0';
  printf("%s\n", buf);
}

ssize_t Read(int fd, void* buf, size_t count) {
  size_t nread = 0;
  while(nread < count) {
    ssize_t n = read(fd, buf+nread, count);
    if(n == -1) {
      if(errno == EINTR) {
        n = 0;
      }
      else {
        return -1;
      }
    }
    else if(n == 0) {
      break;
    }

    nread += n;
  }
  return nread;
}

