#include "cal_client.h"

#include <iostream>
#include <string>
#include <string.h>

#include "build64_release/protocal/cal_header.pb.h"
#include "protocal/cal_header.h"

namespace cal {

void CalClient::HandleIoEvent(int sfd, const sockaddr_in& serv_addr) {
  cal::CalReq req;
  cal::CalRes res;

  MakeReq(req);
  SendReq(sfd, serv_addr, req);

  RecvRes(sfd, res);
  ShowRes(res);
}

void CalClient::MakeReq(CalReq& req) {
  req.set_seqno("101");

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

void CalClient::SendReq(int sfd, const sockaddr_in& serv_addr, const CalReq& req) {
  std::string req_buf;
  req.SerializeToString(&req_buf);

  CalHeader req_header;
  req_header.set_msg_len(req_buf.size());
  std::string header_buf;
  req_header.SerializeToString(&header_buf);

  sendto(sfd, header_buf.data(), header_buf.size(), 0, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

  sendto(sfd, req_buf.data(), req_buf.size(), 0, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
}

void CalClient::RecvRes(int sfd, CalRes& res) {
  static char header_buf[HEADER_LEN];
  recvfrom(sfd, header_buf, HEADER_LEN, 0, NULL, NULL );
  CalHeader cal_header;
  cal_header.ParseFromArray(header_buf, HEADER_LEN);

  int res_len = cal_header.msg_len();
  std::string res_buf(res_len, ' ');
  recvfrom(sfd, &res_buf[0], res_len, 0, NULL, NULL);
  res.ParseFromString(res_buf);
}

void CalClient::ShowRes(const CalRes& res) {
  std::cout << "Operation result: " << res.result() << std::endl;
}

} // namespace cal
