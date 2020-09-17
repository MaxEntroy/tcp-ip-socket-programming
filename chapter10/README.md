## chapter10 多进程服务器端

### Basic

1. 进程概念及其应用
2. 进程和僵尸进程
3. 信号处理
4. 基于多任务的并发服务器
5. 分割TCP的IO程序

q:理解进程？
>占用系统资源，正在运行的程序

q:如何创建进程？
```cpp
#include <unistd.h>

pid_t fork(void);
// 成功，返回进程ID; 父进程，返回子进程ID；子进程，返回0
// 失败，返回-1
```

q:zombie process?
>僵尸进程：进程执行结束，但是资源还未被回收。处在这种状态的进程，叫做僵尸进程。之所有会有僵尸进程，主要是os把他们变成这样的。

q:如何解决zombie?
1. 子进程调用exit/return向os传递返回值
2. 父进程主动向os获取子进程的返回值(结束状态值)来回收僵尸

### 实践

- demo-01

两个简单的case，测试父子进程的输出，以及僵尸的存在。但这个case没有回收僵尸进程。
没有问题的原因在于，这个例子父进程肯定会被回收，所以子进程也一定会被回收。担心的是父进程在while(1)当中，无法被回收.
这个时候，需要父进程主动获取子进程结束的信息，来回收僵尸

- demo-02

这个demo主要给出3种回收僵尸的形式，分别是同步阻塞，同步非阻塞，异步非阻塞

```cpp
// 同步阻塞
// 即使child sleep，parent阻塞在wait不会向下执行
// 同步，时间线性；阻塞，空间一致；代码好写，好理解。缺点是，程序会被阻塞。这个程序体现不出来，如果多进程的master，阻塞了无法accept connection.
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

    wait(&status);
    if (WIFEXITED(status)) {
      printf("Child is terminated, message form chid is %d.\n", WEXITSTATUS(status));
    }
  }

  if (pid == 0) {
    printf("Child Ends.\n");
    return 0;
  } else {
    printf("Parent Ends\n");
    return 0;
  }

}
```

```cpp
// 同步非阻塞
// 这里还是需要进行同步操作。父进程必须保证对子进程进行回收，所以无论如何要等它
// 不等可以不，可以。父进程执行结束，子进程还未结束，这时候子进程被init进程收养变成孤儿进程，init进程完成对后者的回收。
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
    return 0;
  } else {
    printf("Parent Ends\n");
    return 0;
  }

}
```
