#include <stdio.h>
#include <unistd.h>

int gval = 20;

int main(void) {
  int lval= 10;

  gval += 10; // 30
  lval += 5;  // 15

  pid_t pid = fork();

  if (pid == 0) {
    gval += 1; // 31
    lval += 1; // 16
  } else {
    gval += 10; // 40
    lval += 5;  // 20
  }

  if (pid == 0) {
    printf("Child Proc: [%d, %d]\n", gval, lval);
  } else {
    printf("Parent Proc: [%d, %d]\n", gval, lval);
  }

  return 0;
}
