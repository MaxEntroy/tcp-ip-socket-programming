#ifndef UTILS_UDP_CLIENT_H_
#define UTILS_UDP_CLIENT_H_

#include <unistd.h>
#include <arpa/inet.h>

namespace utils {

class UdpClient {
 public:
  UdpClient();

  virtual ~UdpClient() { close(sfd_); }
  UdpClient(const UdpClient&) = delete;
  UdpClient& operator=(const UdpClient&) = delete;

 public:
  void Init(const char* ip, int port);
  void Run();

 private:
  virtual void HandleIoEvent(int) = 0;

 private:
  int sfd_;
  struct sockaddr_in serv_addr_;
}; // class UdpClient

} // namespace utils

#endif // UTILS_UDP_CLIENT_H_
