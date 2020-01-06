#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>

int main(void) {

  int fd1 = socket(PF_INET, SOCK_STREAM, 0);
  int fd2 = open("test.dat", O_CREAT|O_WRONLY|O_TRUNC);
  int fd3 = socket(PF_INET, SOCK_DGRAM, 0);

  printf("fd1: %d\n", fd1);
  printf("fd2: %d\n", fd2);
  printf("fd3: %d\n", fd3);

  close(fd1);
  close(fd2);
  close(fd3);

  return 0;
}
