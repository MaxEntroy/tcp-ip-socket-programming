#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>

int main(void) {
  const char* ip1 = "1.2.3.256";
  struct sockaddr_in addr1;
  memset(&addr1, 0, sizeof(addr1));

  int ret = inet_aton(ip1, &(addr1.sin_addr));
  if(ret) {
    printf("%#x\n",addr1.sin_addr.s_addr );
  }
  else{
    fprintf(stderr, "%s\n", "Invalid ip.");
  }

  const char* ip2 = "255.255.255.255";
  struct sockaddr_in addr2;
  memset(&addr2, 0, sizeof(addr2));
  ret = inet_aton(ip2, &(addr2.sin_addr));
  if(ret) {
    printf("%#x\n", addr2.sin_addr.s_addr);
  }
  else {
    fprintf(stderr, "%s\n", "Invalid ip.");
  }

  const char* ip3 = "1.2.3.4";
  struct sockaddr_in addr3;
  memset(&addr3, 0, sizeof(addr3));
  ret = inet_aton(ip3, &(addr3.sin_addr));
  if(ret) {
    printf("%#x\n", addr3.sin_addr.s_addr);
  }
  else {
    fprintf(stderr, "%s\n", "Invalid ip.");
  }

  return 0;
}
