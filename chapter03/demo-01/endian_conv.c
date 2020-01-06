#include <stdio.h>
#include <arpa/inet.h>

int main() {
  uint16_t host_port = 0x1234;
  uint16_t net_port = htons(host_port);

  uint32_t host_addr = 0x12345678;
  uint32_t net_addr = htonl(host_addr);

  printf("host port: %#x\n", host_port);
  printf("net port: %#x\n", net_port);
  printf("host addr: %#x\n", host_addr);
  printf("net addr: %#x\n", net_addr);

  return 0;
}
