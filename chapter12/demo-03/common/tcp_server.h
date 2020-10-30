#ifndef UTILS_TCP_SERVER_H_
#define UTILS_TCP_SERVER_H_

#include <arpa/inet.h>
#include <unistd.h>

#include <iostream>
#include <vector>

#include "app_buffer.h"

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
  virtual void HandleIoEvent(Buffer* input_buf,
                             Buffer* output_buf) = 0;

  bool OnSocketRead(int sfd, Buffer* buf);
  bool OnSocketWrite(int sfd, Buffer* buf);

 private:
  int listen_sfd_;
  struct sockaddr_in serv_addr_;
  std::vector<Buffer> input_buf_list_;
  std::vector<Buffer> output_buf_list_;
}; // TcpServer

} // namespace utils

#endif // UTILS_TCP_SERVER_H_
