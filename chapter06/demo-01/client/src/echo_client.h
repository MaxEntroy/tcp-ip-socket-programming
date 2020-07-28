#ifndef ECHO_ECHOCLIENT_H_
#define ECHO_ECHOCLIENT_H_

#include "common/udp_client.h"

namespace echo {

class EchoClient : public utils::UdpClient {
 public:
  EchoClient() = default;

  EchoClient(const EchoClient&) = delete;
  EchoClient& operator=(const EchoClient&) = delete;

 private:
  void HandleIoEvent(int, const sockaddr_in&) override;

}; // class EchoClient

} // namespace echo

#endif // ECHO_ECHOCLIENT_H_
