#ifndef CAL_CAL_CLIENT_H_
#define CAL_CAL_CLIENT_H_

#include "build64_release/protocal/cal_req.pb.h"
#include "build64_release/protocal/cal_res.pb.h"
#include "common/udp_client.h"

namespace cal {

class CalClient : public utils::UdpClient {
 public:
  CalClient() = default;

  CalClient(const CalClient&) = delete;
  CalClient& operator=(const CalClient&) = delete;

 private:
  void HandleIoEvent(int sfd, const sockaddr_in& serv_addr) override;

  void MakeReq(CalReq& req);
  void SendReq(int sfd, const sockaddr_in& serv_addr, const CalReq& req);
  void RecvRes(int sfd, CalRes& res);
  void ShowRes(const CalRes& res);

}; // class CalClient

}; // namespace cal

#endif // CAL_CAL_CLIENT_H_
