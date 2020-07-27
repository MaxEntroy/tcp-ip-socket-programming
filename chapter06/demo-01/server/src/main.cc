#include <gflags/gflags.h>

#include "echo_server.h"

DEFINE_int32(port, 54321, "listening port");

int main(int argc, char* argv[]) {
  gflags::ParseCommandLineFlags(&argc, &argv, true);

  echo::EchoServer server;

  server.Init(FLAGS_port);
  server.EventLoop();

  return 0;
}
