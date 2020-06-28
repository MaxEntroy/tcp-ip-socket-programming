#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <gflags/gflags.h>

#include "common/cal_protocol.h"
#include "common/err.h"
#include "common/io_aux.h"

DEFINE_string(ip, "localhost", "server address");
DEFINE_int32(port, 54321, "server port");

static void echo_client(const char* ip, int port);
static void do_io_event(int clnt_sfd);
static int do_input(char buf[]);

int main(int argc, char* argv[]) {
  gflags::ParseCommandLineFlags(&argc, &argv, true);

  echo_client(FLAGS_ip.c_str(), FLAGS_port);
  return 0;
}

void echo_client(const char* ip, int port) {
  // create a socket
  int clnt_sfd = socket(PF_INET, SOCK_STREAM, 0);
  if(clnt_sfd == -1) {
    err_handling("socket", "error");
  }

  struct sockaddr_in serv_addr;
  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  inet_aton(ip, &(serv_addr.sin_addr));
  serv_addr.sin_port = htons(port);

  // connect to server
  int ret = connect(clnt_sfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr));
  if(ret == -1) {
    err_handling("connect", "error");
  }
  printf("Connected to [%s:%d].\n", ip, port);

  // IO
  do_io_event(clnt_sfd);

  close(clnt_sfd);
}

void do_io_event(int clnt_sfd) {
  char buf[BUF_SZ];

  int nsend = do_input(buf);
  int nrecv = RES_RESULT_SZ;
  int res = 0;

  write(clnt_sfd, buf, nsend);

  io_read_n(clnt_sfd, (char*)&res, nrecv);

  printf("Operation result: %d\n", res);
}

int do_input(char buf[]) {
  printf("Operator: ");
  scanf("%c", buf);

  printf("Operation count: ");
  scanf("%d", (int*)(buf + REQ_OPTR_SZ));
  int* opnd_cnt = (int*)(buf + REQ_OPTR_SZ);

  for(int i = 0; i < *opnd_cnt; ++i) {
    printf("Operand %d: ", i + 1);
    scanf("%d", (int*)(buf + REQ_OPTR_SZ + REQ_OPND_NUM_SZ + i * REQ_OPND_SZ));
  }

  return REQ_OPTR_SZ + REQ_OPND_NUM_SZ + *opnd_cnt * REQ_OPND_SZ;
}
