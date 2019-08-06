#include <stdio.h>

#include <gflags/gflags.h>

#include "inc/webserver.h"

DEFINE_string(ip, "localhost", "ip of web server");
DEFINE_int32(port, -1, "port of web server");
DEFINE_int32(backlog, -1, "maximum length of pending queue");

int main(int argc, char* argv[]) {
    gflags::ParseCommandLineFlags(&argc, &argv, true);

    webserver(FLAGS_ip, FLAGS_port, FLAGS_backlog);

    return 0;
}
