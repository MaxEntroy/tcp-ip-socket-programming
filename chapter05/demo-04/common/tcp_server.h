#ifndef UTILS_TCP_SERVER_H_
#define UTILS_TCP_SERVER_H_

#include <unistd.h>

#include <memory>

namespace utils {

struct SocketDeleter {
  void operator()(int* p_sfd) {
    close(*p_sfd);
  }
};

class TcpServer {
 public:
  explicit TcpServer(int port) {}

  TcpServer(const TcpServer&) = delete;
  TcpServer& operator=(const TcpServer&) = delete;

 private:
  std::unique_ptr<int, SocketDeleter> sfd_ptr_;
}; // TcpServer

} // namespace utils

#endif // UTILS_TCP_SERVER_H_
