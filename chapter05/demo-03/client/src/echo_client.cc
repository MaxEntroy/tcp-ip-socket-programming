#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <iostream>
#include <string>

#include <gflags/gflags.h>

#include "common/cal_protocol.h"
#include "common/err.h"
#include "common/io_aux.h"
#include "build64_release/proto/cal_header.pb.h"
#include "build64_release/proto/cal_req.pb.h"
#include "build64_release/proto/cal_res.pb.h"

DEFINE_string(ip, "localhost", "server address");
DEFINE_int32(port, 54321, "server port");

static void echo_client(const char* ip, int port);
static void do_io_event(int clnt_sfd);
static void make_cal_req(cal::CalReq& req);
static void do_write(int clnt_sfd, const cal::CalReq& req);
static void do_read(int clnt_sfd, cal::CalRes& res);
static void do_res(const cal::CalRes& res);

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
  cal::CalReq req;
  cal::CalRes res;

  make_cal_req(req);

  do_write(clnt_sfd, req);

  do_read(clnt_sfd, res);

  do_res(res);
}

void make_cal_req(cal::CalReq& req) {
  // make cal req
  req.set_seqno("1");

  std::string optr;
  std::cout << "Operator: ";
  std::cin >> optr;
  req.set_optr(optr);

  int opnd_cnt = 0;
  std::cout << "Operation count: ";
  std::cin >> opnd_cnt;

  int opnd = 0;
  for(int i = 0; i < opnd_cnt; ++i) {
    std::cout << "Operand " << i + 1 << ": ";
    std::cin >> opnd;
    req.add_opnd_arr(opnd);
  }
}

void do_write(int clnt_sfd, const cal::CalReq& req) {
  std::string req_buf;
  req.SerializeToString(&req_buf);

  // make cal header
  cal::CalHeader req_header;
  req_header.set_msg_len(req_buf.size());
  std::string header_buf;
  req_header.SerializeToString(&header_buf);

  // send cal header
  write(clnt_sfd, header_buf.data(), header_buf.size());

  // send cal req
  write(clnt_sfd, req_buf.data(), req_buf.size());
}

void do_read(int clnt_sfd, cal::CalRes& res) {
  // recv cal header
  char buf[MSG_HEAD_SZ];
  io_read_n(clnt_sfd, buf, MSG_HEAD_SZ);
  cal::CalHeader cal_header;
  cal_header.ParseFromArray(buf, MSG_HEAD_SZ);

  // recv cal res
  int res_len = cal_header.msg_len();
  std::string res_buf(res_len, ' ');
  io_read_n(clnt_sfd, &res_buf[0] ,res_len);
  res.ParseFromString(res_buf);
}

void do_res(const cal::CalRes& res) {
  std::cout << "Operation result: " << res.result() << std::endl;
}
