#ifndef UTILS_TCP_SERVER_H_
#define UTILS_TCP_SERVER_H_

#include <arpa/inet.h>
#include <unistd.h>

#include <iostream>

namespace utils {

class TcpServer {
 public:
  TcpServer();

  virtual ~TcpServer() { close(listen_sfd_); }
  TcpServer(const TcpServer&) = delete;
  TcpServer& operator=(const TcpServer&) = delete;

  void Init(int port, int backlog);
  void EventLoop();

 private:
  virtual void HandleIoEvent(int) = 0;

 private:
  int listen_sfd_;
  struct sockaddr_in serv_addr_;
}; // TcpServer

} // namespace utils

#endif // UTILS_TCP_SERVER_H_
