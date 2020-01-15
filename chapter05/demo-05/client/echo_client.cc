#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <string>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>

extern "C" {
#include "common/err.h"
}
#include "proto/cal_req.pb.h"
#include "proto/cal_res.pb.h"

#define BUF_SZ 32

static void echo_client(const char* ip, uint16_t port);
static void do_io_event(int clnt_sfd);

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
  cal::CalRequest req;

  // init req
  char buf[BUF_SZ];
  snprintf(buf, BUF_SZ, "%u", (unsigned)time(NULL));
  int left_arg = 3;
  int right_arg = 4;
  const char* optr = "*";

  req.set_seqno(buf);
  req.set_left(left_arg);
  req.set_right(right_arg);
  req.set_optr(optr);
  printf("seqno: %s; left: %d; right: %d; optr: %s\n", req.seqno().c_str(), req.left(), req.right(), req.optr().c_str());

  // send to server
  int sz = req.ByteSize();
  req.SerializeToArray(buf, sz);

  write(clnt_sfd, buf, sz);

  // recv from server
  ssize_t nread = read(clnt_sfd, buf, BUF_SZ);

  cal::CalResponse res;
  res.ParseFromArray(buf, nread);

  if(!res.has_seqno() || !res.has_result()) {
    fprintf(stderr, "%s", "Invalid CalResponse.");
    return;
  }

  printf("seqno: %s; result: %d\n", res.seqno().c_str(), res.result());
}
