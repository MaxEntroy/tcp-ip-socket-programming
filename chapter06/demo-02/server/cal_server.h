#ifndef CAL_CAL_SERVER_H_
#define CAL_CAL_SERVER_H_

#include "common/udp_server.h"

namespace cal {

class CalServer : public utils::UdpServer {
 public:
  CalServer() = default;

  CalServer(const CalServer&) = delete;
  CalServer& operator=(const CalServer&) = delete;

 private:
  void HandleIoEvent(int) override;

}; // class CalServer

} // namespace cal

#endif // CAL_CAL_SERVER_H_
