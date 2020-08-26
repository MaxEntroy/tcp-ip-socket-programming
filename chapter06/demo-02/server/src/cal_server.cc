#include "cal_server.h"

#include <iostream>
#include <string>
#include <string.h>

#include "build64_release/protocal/cal_header.pb.h"
#include "protocal/cal_header.h"

namespace cal {

void CalServer::HandleIoEvent(int sfd) {
  CalReq req;
  CalRes res;

  struct sockaddr_in clnt_addr;
  socklen_t clnt_addr_len;
  memset(&clnt_addr, 0, sizeof(clnt_addr));

  RecvReq(sfd, clnt_addr, clnt_addr_len, req);
  MakeRes(req, res);
  SendRes(sfd, clnt_addr, clnt_addr_len, res);
}

void CalServer::RecvReq(int sfd, struct sockaddr_in& clnt_addr, socklen_t& clnt_addr_len, CalReq& req) {
  std::string header_buf;
  header_buf.resize(HEADER_LEN);
  recvfrom(sfd, &header_buf[0], HEADER_LEN, 0, (struct sockaddr*)&clnt_addr, &clnt_addr_len);
  CalHeader cal_header;
  cal_header.ParseFromString(header_buf);

  int req_len = cal_header.msg_len();
  std::string req_buf;
  req_buf.resize(req_len);
  recvfrom(sfd, &req_buf[0], req_len, 0, NULL, NULL);
  req.ParseFromString(req_buf);
}

void CalServer::MakeRes(const CalReq& req, CalRes& res) {
  res.set_seqno(req.seqno());

  int sz = req.opnd_arr_size();
  if ( sz < 1 ) {
    res.set_result(0);
    return;
  }
  if ( sz < 2 ) {
    res.set_result(req.opnd_arr(0));
    return;
  }

  std::string optr = req.optr();
  int tmp = req.opnd_arr(0);
  switch(optr[0]) {
    case '+' : { for(int i = 1; i < sz; ++i) {tmp += req.opnd_arr(i);} break; }
    case '-' : { for(int i = 1; i < sz; ++i) {tmp -= req.opnd_arr(i);} break; }
    case '*' : { for(int i = 1; i < sz; ++i) {tmp *= req.opnd_arr(i);} break; }
    case '/' : { for(int i = 1; i < sz; ++i) {tmp /= req.opnd_arr(i);} break; }
  }
  res.set_result(tmp);
}

void CalServer::SendRes(int sfd, const struct sockaddr_in& clnt_addr, socklen_t clnt_addr_len, const CalRes& res) {
  std::string res_buf;
  res.SerializeToString(&res_buf);

  CalHeader res_header;
  res_header.set_msg_len(res_buf.size());
  std::string res_header_buf;
  res_header.SerializeToString(&res_header_buf);

  sendto(sfd, res_header_buf.data(), res_header_buf.size(), 0, (struct sockaddr*)&clnt_addr, clnt_addr_len);
  sendto(sfd, res_buf.data(), res_buf.size(), 0, (struct sockaddr*)&clnt_addr, clnt_addr_len);
}

} // namespace cal
