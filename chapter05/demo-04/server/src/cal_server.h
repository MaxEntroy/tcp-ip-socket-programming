#ifndef CAL_CAL_SERVER_H_
#define CAL_CAL_SERVER_H_

#include "build64_release/app_protocol/cal_req.pb.h"
#include "build64_release/app_protocol/cal_res.pb.h"
#include "common/tcp_server.h"

namespace cal {

class CalServer : public utils::TcpServer {
 public:
  CalServer() = default;

  CalServer(const CalServer&) = delete;
  CalServer& operator=(const CalServer&) = delete;

 private:
   void HandleIoEvent(int) override;

   void RecvReq(int clnt_sfd, CalReq* req);
   void DoCal(const CalReq& req, CalRes* res);
   void SendRes(int clnt_sfd, const CalRes& res);
}; // class CalServer

}; // namespace cal

#endif // CAL_CAL_SERVER_H_
