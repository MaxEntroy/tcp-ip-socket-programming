#ifndef CAL_CAL_CLIENT_H_
#define CAL_CAL_CLIENT_H_

#include "build64_release/app_protocol/cal_req.pb.h"
#include "build64_release/app_protocol/cal_res.pb.h"
#include "common/tcp_client.h"

namespace cal {

class CalClient : public utils::TcpClient {
 public:
  CalClient() = default;

  CalClient(const CalClient&) = delete;
  CalClient& operator=(const CalClient&) = delete;

 private:
  void HandleIoEvent(int) override;

  void MakeReq(CalReq* req);
  void SendReq(int clnt_sfd, const CalReq& req);
  void RecvRes(int clnt_sfd, CalRes* res);
  void ShowRes(const CalRes& res);

}; // class CalClient

} // namespace cal


#endif // CAL_CAL_CLIENT_H_
