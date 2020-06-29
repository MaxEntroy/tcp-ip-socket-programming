#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <vector>

#include <gflags/gflags.h>

#include "common/cal_protocol.h"
#include "common/err.h"
#include "common/io_aux.h"

DEFINE_int32(port, 54321, "listening port");
DEFINE_int32(backlog, 10, "listening backlog");

#define TRUE 1

static void echo_server(int port, int backlog);
static void do_io_event(int clnt_sfd);
static void do_read_buf(int clnt_sfd, char buf[], char& optr, std::vector<int>& opnd_arr);
static int do_cal(char optr, const std::vector<int>& opnd_arr);

int main(int argc, char* argv[]) {
  gflags::ParseCommandLineFlags(&argc, &argv, true);

  echo_server(FLAGS_port, FLAGS_backlog);
  return 0;
}

void echo_server(int port, int backlog) {
  // create a socket
  int serv_sfd = socket(PF_INET, SOCK_STREAM, 0);
  if(serv_sfd == -1) {
    err_handling("socket", "error");
  }

  // bind
  struct sockaddr_in serv_addr;
  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(port);

  int ret = bind(serv_sfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr));
  if(ret == -1) {
    err_handling("bind", "error");
  }

  // listen
  ret = listen(serv_sfd, backlog);
  if(ret == -1) {
    err_handling("listen", "error");
  }

  // serving-loop
  struct sockaddr_in clnt_addr;
  memset(&clnt_addr, 0, sizeof(clnt_addr));
  socklen_t clnt_addr_len = 0;
  while(TRUE) {
    printf("Caculate server[localhost:%d] waiting...\n", port);

    // accept
    int clnt_sfd = accept(serv_sfd, (struct sockaddr*) &clnt_addr, &clnt_addr_len);
    printf("[%s:%d] connected.\n", inet_ntoa(clnt_addr.sin_addr), ntohs(clnt_addr.sin_port));

    // IO
    do_io_event(clnt_sfd);

    close(clnt_sfd);
    printf("[%s:%d] disconnected.\n", inet_ntoa(clnt_addr.sin_addr), ntohs(clnt_addr.sin_port));
  }

  close(serv_sfd);
}

void do_io_event(int clnt_sfd) {
  char buf[BUF_SZ];

  char optr = '\0';
  std::vector<int> opnd_arr;

  do_read_buf(clnt_sfd, buf, optr, opnd_arr);

  int res = do_cal(optr, opnd_arr);

  write(clnt_sfd, (char*)&res, RES_RESULT_SZ);
}

void do_read_buf(int clnt_sfd, char buf[], char& optr, std::vector<int>& opnd_arr) {
  io_read_n(clnt_sfd, buf, REQ_OPTR_SZ + REQ_OPND_NUM_SZ);
  optr = *((char*)buf);

  int opnd_num = *((int*)(buf + REQ_OPTR_SZ));
  opnd_arr.resize(opnd_num);

  io_read_n(clnt_sfd, buf + REQ_OPTR_SZ + REQ_OPND_NUM_SZ, opnd_num * REQ_OPND_SZ);

  for(int i = 0; i < opnd_num; ++i) {
    opnd_arr[i] = *((int*)(buf + REQ_OPTR_SZ + REQ_OPND_NUM_SZ + i * REQ_OPND_SZ));
  }
}

int do_cal(char optr, const std::vector<int>& opnd_arr) {
  int sz = opnd_arr.size();
  if (sz == 0)
    return 0;
  else if (sz == 1)
    return opnd_arr[0];

  int res = opnd_arr[0];
  switch(optr) {
    case '+' : {
      for(int i = 1; i < sz; ++i) res += opnd_arr[i];
      break;
    }
    case '-' : {
      for(int i = 1; i < sz; ++i) res -= opnd_arr[i];
      break;
    }
    case '*' : {
      for(int i = 1; i < sz; ++i) res *= opnd_arr[i];
      break;
    }
    case '/' : {
      for(int i = 1; i < sz; ++i) res /= opnd_arr[i];
      break;
    }
  }
  return res;
}
