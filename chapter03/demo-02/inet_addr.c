#include <stdio.h>
#include <arpa/inet.h>

int main(void) {
  const char* ip1 = "1.2.3.256";
  uint64_t ret1 = inet_addr(ip1);
  printf("%#lx\n", ret1);
  uint64_t ret11 = -1;
  printf("%#lx\n", ret11);

  const char* ip2 = "255.255.255.255";
  uint64_t ret2 = inet_addr(ip2);
  printf("%#lx\n", ret2);

  return 0;
}
