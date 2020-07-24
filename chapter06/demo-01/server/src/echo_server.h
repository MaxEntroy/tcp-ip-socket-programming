#ifndef ECHO_ECHO_SERVER_H_
#define ECHO_ECHO_SERVER_H_

#include "common/udp_server.h"

namespace echo {

class EchoServer : public utils::UdpServer {
 public:
  EchoServer() = default;

  EchoServer(const EchoServer&) = delete;
  EchoServer& operator=(const EchoServer&) = delete;

 private:
  void HandleIoEvent(int) override;

}; // class EchoServer

} // namespace echo

#endif // ECHO_ECHO_SERVER_H_
