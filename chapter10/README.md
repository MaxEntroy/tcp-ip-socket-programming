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

```cpp
// 异步非阻塞
// 通过分析master代码我们发现，当主进程是一个while(true)-loop
// 即主进程是server，同步操作会导致主进程无法进行其余操作
// 此时采用异步的方式，主进程向os注册事件，以及事件处理器。
// 当事件发生时，os调度主进程执行handler代码。
// 当然，这种调用，本质还是主进程的调用，只不过基于事件机制，我们叫做异步调用
// 异步的好处是，主进程根本 不用关心对于子进程的回收，专注在自己的do something即可
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

```

- demo-03

实现多进程服务器，代码的修改如下
1. 增加reuse addr逻辑
2. 异步非阻塞处理子进程回收事件
3. 增加子进程逻辑。子进程处理业务io，父进程处理请求io

q:子进程当中为什么没有close(listen_sfd_)？
>这么说不对。看起来没有，其实关闭了。代码验证没有问题。
这里的原因在于对于子进程的理解有误，真正的父进程是main。不是一个tcp_server binary
所以，子进程是从fork开始，到剩余的代码结束。也是main的代码，main当中tcp server析构会释放listen_sfd
当然，如果想显示关闭，也可以。那么此时需要直接结束进程exit(EXIT_SUCCESS);这么写才可以。
目前的代码是直接返回，EventLoop执行结束。main向下执行会析构对象

q:对于多进程的理解？
>从本次实践来看。客户端代码大都是同步的，肯定需要建立连接之后，再开始准备数据，发送数据。
而服务器处理一个请求连接是很快的，就会发现。迭代服务器，程序会阻塞在连接建立完成之后的业务io上。处理机的能力没有充分利用。
此时有请求来，确实无法处理。
但是多进程，是把对于业务io的阻塞从主进程当中接耦出去，交给子进程处理。没有问题，增加了并发性。