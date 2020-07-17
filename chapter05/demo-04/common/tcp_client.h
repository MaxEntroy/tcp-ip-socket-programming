#ifndef UTILS_TCP_CLIENT_H_
#define UTILS_TCP_CLIENT_H_

#include <unistd.h>
#include <arpa/inet.h>

namespace utils {

class TcpClient {
 public:
  TcpClient();

  ~TcpClient() { close(clnt_sfd_); }
  TcpClient(const TcpClient&) = delete;
  TcpClient& operator=(const TcpClient&) = delete;

 public:
  void Init(const char* ip, int port);
  void Run();

 private:
  virtual void HandleIoEvent() = 0;

 private:
  int clnt_sfd_;
  struct sockaddr_in serv_addr_;
}; // class TcpClient

} // namespace utils

#endif // UTILS_TCP_CLIENT_H_
