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
#include "build64_release/proto/cal_header.pb.h"
#include "build64_release/proto/cal_req.pb.h"
#include "build64_release/proto/cal_res.pb.h"

DEFINE_int32(port, 54321, "listening port");
DEFINE_int32(backlog, 10, "listening backlog");

#define TRUE 1

static void echo_server(int port, int backlog);
static void do_io_event(int clnt_sfd);
static void do_read(int clnt_sfd, cal::CalReq& req);
static void do_cal(const cal::CalReq& req, cal::CalRes& res);
static void do_write(int clnt_sfd, const cal::CalRes& res);

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
  cal::CalReq req;
  cal::CalRes res;

  do_read(clnt_sfd, req);

  do_cal(req, res);

  do_write(clnt_sfd, res);
}

void do_read(int clnt_sfd, cal::CalReq& req) {
  // recv cal header
  char buf[MSG_HEAD_SZ];
  io_read_n(clnt_sfd, buf, MSG_HEAD_SZ);
  cal::CalHeader cal_header;
  cal_header.ParseFromArray(buf, MSG_HEAD_SZ);

  // recv cal req
  int req_len = cal_header.msg_len();
  std::string req_buf(req_len, ' ');
  io_read_n(clnt_sfd, &req_buf[0], req_len);
  req.ParseFromString(req_buf);
}

void do_cal(const cal::CalReq& req, cal::CalRes& res) {
  res.set_seqno("2");

  int sz = req.opnd_arr_size();
  if (sz == 0) {
    res.set_result(0);
    return;
  }
  else if (sz == 1) {
    res.set_result(req.opnd_arr(0));
    return;
  }

  int ret = req.opnd_arr(0);
  switch(req.optr()[0]) {
    case '+' : {
      for(int i = 1; i < sz; ++i) ret += req.opnd_arr(i);
      res.set_result(ret);
      break;
    }
    case '-' : {
      for(int i = 1; i < sz; ++i) ret -= req.opnd_arr(i);
      res.set_result(ret);
      break;
    }
    case '*' : {
      for(int i = 1; i < sz; ++i) ret *= req.opnd_arr(i);
      res.set_result(ret);
      break;
    }
    case '/' : {
      for(int i = 1; i < sz; ++i) ret /= req.opnd_arr(i);
      res.set_result(ret);
      break;
    }
  }
}

void do_write(int clnt_sfd, const cal::CalRes& res) {
  std::string res_buf;
  res.SerializeToString(&res_buf);

  cal::CalHeader res_header;
  res_header.set_msg_len(res_buf.size());
  std::string header_buf;
  res_header.SerializeToString(&header_buf);

  // send cal header
  write(clnt_sfd, header_buf.data(), header_buf.size());

  // send cal res
  write(clnt_sfd, res_buf.data(), res_buf.size());
}
