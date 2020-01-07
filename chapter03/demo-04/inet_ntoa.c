#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>

#define BUF_SZ 128

int main(void) {
  char buf[BUF_SZ];

  struct sockaddr_in addr1;
  memset(&addr1, 0, sizeof(addr1));
  addr1.sin_addr.s_addr = htonl(0x01020304);

  const char* pip = inet_ntoa(addr1.sin_addr);
  strncpy(buf, pip, BUF_SZ);
  printf("Dotted-decimal notation: %s\n", buf);


  struct sockaddr_in addr2;
  memset(&addr2, 0, sizeof(addr2));
  addr2.sin_addr.s_addr = htonl(0x04030201);

  pip = inet_ntoa(addr2.sin_addr);
  strncpy(buf, pip, BUF_SZ);
  printf("Dotted-decimal notation: %s\n", buf);

  return 0;
}
