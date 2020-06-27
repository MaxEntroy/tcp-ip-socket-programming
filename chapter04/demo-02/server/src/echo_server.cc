#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <gflags/gflags.h>
#include <glog/logging.h>

DEFINE_int32(port, 54321, "listening port");
DEFINE_int32(backlog, 10, "listening backlog");

#define TRUE 1
#define BUF_SZ 128

static void echo_server(int port, int backlog);
static void do_io_event(int clnt_sfd);

int main(int argc, char* argv[]) {
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  google::InitGoogleLogging(argv[0]);

  echo_server(FLAGS_port, FLAGS_backlog);

  return 0;
}

void echo_server(int port, int backlog) {
  // create a socket
  int serv_sfd = socket(PF_INET, SOCK_STREAM, 0);
  if(serv_sfd == -1) {
    LOG(ERROR) << "socket error";
  }

  // bind
  struct sockaddr_in serv_addr;
  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(port);

  int ret = bind(serv_sfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr));
  if(ret == -1) {
    LOG(ERROR) << "bind error";
  }

  // listen
  ret = listen(serv_sfd, backlog);
  if(ret == -1) {
    LOG(ERROR) << "listen error";
  }

  // serving-loop
  struct sockaddr_in clnt_addr;
  memset(&clnt_addr, 0, sizeof(clnt_addr));
  socklen_t clnt_addr_len = 0;
  while(TRUE) {
    LOG(INFO) << "Echo Server[localhost:" << port << "] waiting...";

    // accept
    int clnt_sfd = accept(serv_sfd, (struct sockaddr*) &clnt_addr, &clnt_addr_len);
    LOG(INFO) << "[" << inet_ntoa(clnt_addr.sin_addr) << ":" << ntohs(clnt_addr.sin_port) << "] connected." << std::endl;

    // IO
    do_io_event(clnt_sfd);

    close(clnt_sfd);
    LOG(INFO) << "[" << inet_ntoa(clnt_addr.sin_addr) << ":" << ntohs(clnt_addr.sin_port) << "] disconnected." << std::endl;
  }

  close(serv_sfd);
}

void do_io_event(int clnt_sfd) {
  char buf[BUF_SZ];

  int nread = read(clnt_sfd, buf, BUF_SZ);
  write(clnt_sfd, buf, nread);
}
