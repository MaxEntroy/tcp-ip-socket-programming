#include <gflags/gflags.h>

#include "echo_client.h"

DEFINE_string(ip, "localhost", "server address");
DEFINE_int32(port, 54321, "server port");

int main(int argc, char* argv[]) {
  gflags::ParseCommandLineFlags(&argc, &argv, true);

  echo::EchoClient client;

  client.Init(FLAGS_ip.c_str(), FLAGS_port);
  client.Run();

  return 0;
}
