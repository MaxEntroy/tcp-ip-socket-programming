#include <stdio.h>

#include <gflags/gflags.h>

#include "inc/webserver.h"

DEFINE_string(ip, "localhost", "ip of web server");
DEFINE_int32(port, -1, "port of web server");

int main(int argc, char* argv[]) {
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    printf("%s %d\n", FLAGS_ip.c_str(), FLAGS_port);

    return 0;
}
