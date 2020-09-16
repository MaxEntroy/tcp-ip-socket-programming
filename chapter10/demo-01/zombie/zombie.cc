#include <stdio.h>
#include <unistd.h>

int main(void) {
  pid_t pid = fork();

  if (pid == 0) {
    printf("I am child process.\n");
  } else {
    printf("I am parent process, child is %d\n", pid);
    sleep(60);
  }

  if (pid == 0) {
    printf("Child Ends.\n");
  } else {
    printf("Parent Ends.\n");
  }

  return 0;
}
