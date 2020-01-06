#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "common/err.h"

void write_file();
void read_file();

int main(void) {

  write_file();
  read_file();

  return 0;
}

void write_file() {
  int fd = open("data.txt", O_CREAT|O_TRUNC|O_WRONLY);
  if(fd == -1)
    perr_handling("open", "error");

  printf("The file descriptor: %d\n", fd);
  char buf[] = "hello,world";
  int nwrite = write(fd, buf, sizeof(buf));
  printf("nwrite: %d\n", nwrite);

  close(fd);
}

void read_file() {
  int fd = open("data.txt", O_RDONLY);
  if(fd == -1)
    perr_handling("open", "error");

  char buf[32];
  int nread = read(fd, buf, sizeof(buf));
  printf("nread: %d\n", nread);
  buf[nread] = '\0';

  printf("%s\n", buf);
  close(fd);
}
