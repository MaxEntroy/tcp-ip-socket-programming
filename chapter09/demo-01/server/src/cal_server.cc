#include "cal_server.h"

#include <iostream>

#include "app_protocol/cal_protocol.h"
#include "build64_release/app_protocol/cal_header.pb.h"
#include "common/io_aux.h"

namespace cal {

void CalServer::HandleIoEvent(int clnt_sfd) {
  CalReq req;
  CalRes res;

  RecvReq(clnt_sfd, &req);

  DoCal(req, &res);

  SendRes(clnt_sfd, res);
}

void CalServer::RecvReq(int clnt_sfd, CalReq* req) {
  // recv cal header
  char buf[MSG_HEAD_SZ];
  utils::io_read_n(clnt_sfd, buf, MSG_HEAD_SZ);
  cal::CalHeader cal_header;
  cal_header.ParseFromArray(buf, MSG_HEAD_SZ);

  // recv cal req
  int req_len = cal_header.msg_len();
  std::string req_buf(req_len, ' ');
  utils::io_read_n(clnt_sfd, &req_buf[0], req_len);
  if (!req->ParseFromString(req_buf)) {
    std::cerr << "ParseFromString error." << std::endl;
  }
}

void CalServer::DoCal(const CalReq& req, CalRes* res) { res->set_seqno("2");

  int sz = req.opnd_arr_size();
  if (sz == 0) {
    res->set_result(0);
    return;
  }
  else if (sz == 1) {
    res->set_result(req.opnd_arr(0));
    return;
  }

  int ret = req.opnd_arr(0);
  switch(req.optr()[0]) {
    case '+' : {
      for(int i = 1; i < sz; ++i) ret += req.opnd_arr(i);
      res->set_result(ret);
      break;
    }
    case '-' : {
      for(int i = 1; i < sz; ++i) ret -= req.opnd_arr(i);
      res->set_result(ret);
      break;
    }
    case '*' : {
      for(int i = 1; i < sz; ++i) ret *= req.opnd_arr(i);
      res->set_result(ret);
      break;
    }
    case '/' : {
      for(int i = 1; i < sz; ++i) ret /= req.opnd_arr(i);
      res->set_result(ret);
      break;
    }
  }
}

void CalServer::SendRes(int clnt_sfd, const CalRes& res) {
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

} // namespace cal
