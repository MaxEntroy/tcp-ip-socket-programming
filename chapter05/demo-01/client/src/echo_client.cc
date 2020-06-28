#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <iostream>
#include <string>

#include <gflags/gflags.h>

#define BUF_SZ 128

DEFINE_string(ip, "localhost", "server address");
DEFINE_int32(port, 54321, "server port");

static void echo_client(const std::string& ip, int port);
static void do_io_event(int clnt_sfd);
static void read_n(int fd, char buf[], int n);

int main(int argc, char* argv[]) {
  gflags::ParseCommandLineFlags(&argc, &argv, true);

  echo_client(FLAGS_ip, FLAGS_port);
  return 0;
}

void echo_client(const std::string& ip, int port) {
  // create a socket
  int clnt_sfd = socket(PF_INET, SOCK_STREAM, 0);
  if(clnt_sfd == -1) {
    std::cerr << "socket error";
  }

  struct sockaddr_in serv_addr;
  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  inet_aton(ip.c_str(), &(serv_addr.sin_addr));
  serv_addr.sin_port = htons(port);

  // connect to server
  int ret = connect(clnt_sfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr));
  if(ret == -1) {
    std::cerr << "socket error";
  }
  std::cout << "Connected to [" << ip << ":" << port << "]" << std::endl;

  // IO
  do_io_event(clnt_sfd);

  close(clnt_sfd);
}

void do_io_event(int clnt_sfd) {
  const char* msg = "hello, echo server, i am echo client.";
  char buf[BUF_SZ];
  int str_len = strlen(msg);

  write(clnt_sfd, msg, str_len);
  read_n(clnt_sfd, buf, str_len);
  buf[str_len] = '\0';
  std::cout << buf << std::endl;
}

void read_n(int fd, char buf[], int n) {
  int total = 0;
  int nread = 0;
  while(total < n) {
    nread = read(fd, buf+total, BUF_SZ);
    if (nread == -1) {
      if (errno == EINTR)
        nread = 0;
      else
        return;
    }
    else if (nread == 0)
      break;

    total += nread;
  }
}
