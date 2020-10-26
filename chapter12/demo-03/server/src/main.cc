#include <gflags/gflags.h>

#include "cal_server.h"

DEFINE_int32(port, 54321, "listening port");
DEFINE_int32(backlog, 10, "listening backlog");

int main(int argc, char* argv[]) {
  gflags::ParseCommandLineFlags(&argc, &argv, true);

  cal::CalServer server;

  server.Init(FLAGS_port, FLAGS_backlog);
  server.EventLoop();

  return 0;
}
