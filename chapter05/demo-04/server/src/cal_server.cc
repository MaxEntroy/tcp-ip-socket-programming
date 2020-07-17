#include "cal_server.h"

#include "app_protocol/cal_protocol.h"
#include "build64_release/app_protocol/cal_header.pb.h"

namespace cal {

void CalServer::HandleIoEvent(int clnt_sfd) {
  CalReq req;
  CalRes res;

  DoRecv(clnt_sfd, &req);

  DoCal(req, &res);

  DoSend(clnt_sfd, res);
}

void DoRecv(int clnt_sfd, CalReq* p_req) {
  // recv cal header
  char buf[MSG_HEAD_SZ];
  io_read_n(clnt_sfd, buf, MSG_HEAD_SZ);
  cal::CalHeader cal_header;
  cal_header.ParseFromArray(buf, MSG_HEAD_SZ);

  // recv cal req
  int req_len = cal_header.msg_len();
  std::string req_buf(req_len, ' ');
  io_read_n(clnt_sfd, &req_buf[0], req_len);
  p_req->ParseFromString(req_buf);
}

void DoCal(const CalReq& req, CalRes* p_res) {}

void DoSend(int clnt_sfd, const CalRes& res) {}

} // namespace cal
