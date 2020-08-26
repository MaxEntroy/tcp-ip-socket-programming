#include <gflags/gflags.h>

#include "cal_server.h"

DEFINE_int32(port, 54321, "listening port");

int main(int argc, char* argv[]) {
  gflags::ParseCommandLineFlags(&argc, &argv, true);

  cal::CalServer server;

  server.Init(FLAGS_port);
  server.EventLoop();

  return 0;
}
