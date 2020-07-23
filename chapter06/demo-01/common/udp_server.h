#ifndef UTILS_UDP_SERVER_H_
#define UTILS_UDP_SERVER_H_

#include <arpa/inet.h>
#include <unistd.h>

namespace utils {

class UdpServer {
 public:
  UdpServer();

  virtual ~UdpServer() { close(sfd_); }
  UdpServer(const UdpServer&) = delete;
  UdpServer& operator=(const UdpServer&) = delete;

  void Init(int port);
  void EventLoop();

 private:
  virtual void HandleIoEvent(int) = 0;

 private:
  int sfd_;
  struct sockaddr_in serv_addr_;
}; // class UdpServer

} // namespace utils

#endif // UTILS_UDP_SERVER_H_
