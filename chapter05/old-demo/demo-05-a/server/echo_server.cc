#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <string>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

extern "C" {
#include "common/err.h"
}
#include "proto/cal_req.pb.h"
#include "proto/cal_res.pb.h"

#define TRUE 1
#define BACKLOG 15
#define BUF_SZ 128

static void echo_server(uint16_t port);
static void do_io_event(int clnt_sfd);
static void do_cal(const cal::CalRequest& req, cal::CalResponse* p_res);

int main(int argc, char* argv[]) {
  if(argc != 2) {
    err_handling(argv[0], "error\nUsage:\t./echo_server port");
  }

  echo_server(atoi(argv[1]));

  return 0;
}

void echo_server(uint16_t port) {
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
    printf("[INFO]:Echo Server[localhost:%u] waiting...\n", port);

    // accept
    int clnt_sfd = accept(serv_sfd, (struct sockaddr*) &clnt_addr, &clnt_addr_len);
    printf("[INFO]:[%s:%u] connected.\n", inet_ntoa(clnt_addr.sin_addr), ntohs(clnt_addr.sin_port));

    // IO
    do_io_event(clnt_sfd);

    close(clnt_sfd);
    printf("[INFO]:[%s:%u] disconnected.\n", inet_ntoa(clnt_addr.sin_addr), ntohs(clnt_addr.sin_port));
  }

  close(serv_sfd);
}

void do_io_event(int clnt_sfd) {
  // recv from client
  char buf[BUF_SZ];
  ssize_t nread = read(clnt_sfd, buf, BUF_SZ);

  cal::CalRequest req;
  std::string recv_buf(buf, nread);
  req.ParseFromString(recv_buf);

  if(!req.has_seqno() || !req.has_left() || !req.has_right() || !req.has_optr()) {
    fprintf(stderr, "%s\n", "Invalid CalRequest.");
    return;
  }

  // do caculation
  cal::CalResponse res;
  do_cal(req, &res);

  // send to client
  std::string send_buf;
  res.SerializeToString(&send_buf);

  write(clnt_sfd, send_buf.data(), send_buf.size());
}

void do_cal(const cal::CalRequest& req, cal::CalResponse* p_res) {
  std::string seqno = req.seqno();
  int left = req.left();
  int right = req.right();
  const char* optr = req.optr().c_str();

  int result = 0;
  switch(optr[0]) {
    case '+': result = left + right; break;
    case '-': result = left - right; break;
    case '*': result = left * right; break;
    case '/':
              {
                if(!right) {
                  fprintf(stderr, "%s\n", "zero can not be divisor.");
                  result = 0;
                }
                else
                  result = left / right;
                break;
              }
    default: result = 0; break;
  }

  p_res->set_seqno(seqno);
  p_res->set_result(result);
}
