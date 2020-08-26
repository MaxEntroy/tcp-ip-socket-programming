#ifndef CAL_CAL_SERVER_H_
#define CAL_CAL_SERVER_H_

#include "build64_release/protocal/cal_req.pb.h"
#include "build64_release/protocal/cal_res.pb.h"
#include "common/udp_server.h"

namespace cal {

class CalServer : public utils::UdpServer {
 public:
  CalServer() = default;

  CalServer(const CalServer&) = delete;
  CalServer& operator=(const CalServer&) = delete;

 private:
  void HandleIoEvent(int sfd) override;

  void RecvReq(int sfd, struct sockaddr_in& clnt_addr, socklen_t& clnt_addr_len, CalReq& req);
  void MakeRes(const CalReq& req, CalRes& res);
  void SendRes(int sfd, const struct sockaddr_in& clnt_addr, socklen_t clnt_addr_len, const CalRes& res);

}; // class CalServer

} // namespace cal

#endif // CAL_CAL_SERVER_H_
