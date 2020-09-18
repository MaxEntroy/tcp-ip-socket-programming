#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void) {
  pid_t pid = fork();
  int status = 0;

  if (pid == 0) {
    printf("I am child process.\n");
    sleep(3);
  } else {
    printf("I am parent process, child is %d\n", pid);

    while(!waitpid(pid, &status, WNOHANG)) {
      //sleep(3);
    }

    if (WIFEXITED(status)) {
      printf("Child is terminated, message form chid is %d.\n", WEXITSTATUS(status));
    }
  }

  if (pid == 0) {
    printf("Child Ends.\n");
    return 1;
  } else {
    printf("Parent Ends\n");
    return 0;
  }
}
