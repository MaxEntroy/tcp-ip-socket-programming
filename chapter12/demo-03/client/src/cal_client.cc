#include "cal_client.h"

#include <iostream>

#include "app_protocol/cal_protocol.h"
#include "build64_release/app_protocol/cal_header.pb.h"
#include "common/io_aux.h"

namespace cal {

void CalClient::HandleIoEvent(int clnt_sfd) {
  CalReq req;
  CalRes res;

  MakeReq(&req);
  SendReq(clnt_sfd, req);
  RecvRes(clnt_sfd, &res);
  ShowRes(res);
}


void CalClient::MakeReq(CalReq* req) {
  // make cal req
  req->set_seqno("1");

  std::string optr;
  std::cout << "Operator: ";
  std::cin >> optr;
  req->set_optr(optr);

  int opnd_cnt = 0;
  std::cout << "Operation count: ";
  std::cin >> opnd_cnt;

  int opnd = 0;
  for(int i = 0; i < opnd_cnt; ++i) {
    std::cout << "Operand " << i + 1 << ": ";
    std::cin >> opnd;
    req->add_opnd_arr(opnd);
  }
}

void CalClient::SendReq(int clnt_sfd, const CalReq& req) {
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

void CalClient::RecvRes(int clnt_sfd, CalRes* res) {
  // recv cal header
  char buf[MSG_HEAD_SZ];
  utils::io_read_n(clnt_sfd, buf, MSG_HEAD_SZ);
  cal::CalHeader cal_header;
  cal_header.ParseFromArray(buf, MSG_HEAD_SZ);

  // recv cal res
  int res_len = cal_header.msg_len();
  std::string res_buf(res_len, ' ');
  utils::io_read_n(clnt_sfd, &res_buf[0] ,res_len);
  res->ParseFromString(res_buf);
}

void CalClient::ShowRes(const CalRes& res) {
  std::cout << "Operation result: " << res.result() << std::endl;
}

} // namespace cal
