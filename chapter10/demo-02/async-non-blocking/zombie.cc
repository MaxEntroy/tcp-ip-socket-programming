#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>

void child_handler(int sig) {
  printf("%d events happend.\n", sig);

  int status = 0;
  pid_t child_pid = waitpid(-1, &status, WNOHANG);

  if (WIFEXITED(status)) {
    printf("Remove child: %d.\n", child_pid);
    printf("Child send: %d.\n", status);
  }
}

void do_something(int& counter) {
  printf("Parent: do something.\n");
  counter += 5;
  sleep(3);
}

void register_event() {
  struct sigaction action;
  action.sa_handler = &child_handler;
  action.sa_flags = 0;
  sigemptyset(&action.sa_mask);

  sigaction(SIGCHLD, &action, NULL);
}

int child_routine() {
  printf("I am child process.\n");
  printf("Child Ends.\n");
  return 100;
}

int parent_routine() {
  int counter = 0;
  while(true) {
    do_something(counter);
    if (counter > 10) {
      break;
    }
  }
  printf("Parent Ends\n");
  return 0;
}

int main(void) {
  register_event();

  pid_t pid = fork();
  if (pid == 0) {
    return child_routine();
  } else {
    return parent_routine();
  }
}
