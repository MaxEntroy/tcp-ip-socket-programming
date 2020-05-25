## chapter04 基于TCP的服务器端/客户端(1)

### Basic

q:本章是如何组织的？
4.1. 理解TCP和UDP
4.2. 实现基于TCP的服务器端/客户端
4.3. 实现迭代服务器端/客户端
4.4. 基于windows的实现

q:如何理解tcp/ip协议栈
>我认为作者从宏观上给出了非常好的理解，作者的想法如下：
面对"基于互联网的有效数据传输"命题，并非通过一个庞大的协议解决问题。
而是化整为零，通过层次化方法-协议栈的方式来解决

q:tcp/ip协议诞生的背景?
>基于互联网的有效数据传输，并非只需要软件专家或者硬件专家，而是需要硬件，软件，算法(路由)，系统不同领域的专家。
来解决这样一个涉及软件，硬件，算法，系统的问题。
1. 把一个大问题划分成若干小问题，逐个攻破，提升效率
2. 通过层级的关系又将各个子领域关联起来，建立紧密联系

q:简单介绍下tcp/ip协议
1. link layer: 物理连接领域标准化的结果。专门定义LAN,WLAN,MAN等网络标准
2. internet layer: 物理连接准备完毕，需要解决传输数据当中的路由选择问题，网络层主要解决这个问题。ps: 提供不可靠的服务
3. transport layer: 网络层解决实际数据传输路径选择的问题。传输层在网络层基础上，建立数据传输channel，为应用数据提供端到端的服务。ps:tcp提供可靠服务
4. application layer:在传输层提供的端到端的数据传输基础上，根据应用的特点，制定应用对应的数据传输规则。给出应用层数据传输语义。ps:以上3层均由kernel支持，本层则由programmer支持

参考<br>
[link-layer](https://en.wikipedia.org/wiki/Internet_protocol_suite#Link_layer)

### 实践

- demo-01

echo-server实现，注意以下
- echo-client没必要实现成while-loop
- tcp没有数据边界

本次实现的问题，看看io_event的实现
```c
// client
void do_io_event(int clnt_sfd) {
  const char* msg = "hello, echo server, i am echo client.";
  char buf[BUF_SZ];

  write(clnt_sfd, msg, strlen(msg));
  int nread = read(clnt_sfd, buf, sizeof(buf));
  buf[nread] = '\0';
  printf("%s\n", buf);
}

// server
void do_io_event(int clnt_sfd) {
  char buf[BUF_SZ];

  int nread = read(clnt_sfd, buf, BUF_SZ);
  write(clnt_sfd, buf, nread);
}
```

先来补充一些read/write函数的知识:

q:write函数，为什么会阻塞？
>ssize_t write(int fd, const void *buf, size_t count);
write函数成功返回，只是buf当中的数据全部被复制到kernel当中的tcp send buffer.至于，数据什么时候被发送到网络?什么时候被对方主机接受?什么时候被对方进程接受?
不做保证.但可以保证的是，tcp send buffer当中的数据，一定会被发送数据
>
>write阻塞的条件是：kernel的对应socket的发送缓冲区已满时
对于send buffer:
1.已经发送到网络的数据依然需要暂存在send buffer中，因为ip层提供不可靠的服务，需要tcp层保证。
2.只有收到对方的ack后，kernel才从buffer中清除这一部分数据，为后续发送数据腾出空间

q:read函数，为什么会阻塞?
>ssize_t read(int fd, void *buf, size_t count);
read函数总是在，receive buffer有数据时，立即返回。接收端将收到的数据暂存在receive buffer中，自动进行确认。
但如果socket所在的进程不及时将数据从receive buffer中取出，拷贝到应用层buffer当中，最终导致receive buffer填满，由于TCP的滑动窗口和拥塞控制，接收端会阻止发送端向其发送数据
>
>read阻塞的条件是：只有当receive buffer为空时，才会阻塞

**需要注意的一点是，上述的io都是blocking io，否则我们讨论阻塞也没有什么意义(read/write默认就是blocking io)**<br>
- write返回时，我们可以保证应用层数据都可以发送给接收端
- read返回时，我们不能保证，应用层数据收到了全部希望收到的数据

所以，echo client当中do_io_event没有办法保证，获取到全部的应用层希望收到的数据

q:non-blocking io和blocking io的区别?
>对于read函数:
- 1.1.blocking模式下,receive buffer为空，阻塞
- 1.2.non-blocking模式下，receive buffer为空，非阻塞，返回-1(erron为EAGAIN或EWOULDBLOCK)
>对于write函数:
- 2.1.blocking模式下，只要应用层数据没有完全拷贝到kernel tcp send buffer，阻塞
- 2.2.1.non-blocking模式下，应用层buf <= tcp send buffer，返回应用层buf字节数
- 2.2.2.non-blocking模式下，应用层buf > tcp send buffer and tcp send buffer != 0，返回实际拷贝到tcp send buffer的字节数
- 2.2.3.non-blocking模式下，应用层buf > tcp send buffer and tcp send buffer == 0，返回-1

再回过头来说下echo_client和echo_server当中的do_io_event的问题

q:echo_client当中的do_io_event存在哪些问题?
>我们从上面的分析可以得知，read在blocking模式下，只要receive buffer有数据，立马就返回。
考虑这样的情形，client发送100byte,但是发送端是30-30-40这么发送过去的，server端也是30-30-40这么发送回来的。
显然，上面的代码不足以收集完成所有的数据。
但是，有一个点，client是清楚的，client发送了多少字节的数据，自己清楚，因此在接受的时候可以保证收集完整之后，再退出。

q:echo_server当中的do_io_event存在哪些问题?
>不做细节讨论，从宏观上来说，目前这个版本的echo-server实现，server端无法得知client端到底有多少数据要发送，按照目前的实现，
假如read了部分数据，read返回，然后write返回，整个io_event就结束了，根本没有办法保证，接收到client端所有数据才结束。
但是目前又没有应用层协议来保证，比如说，发个标志。只能依赖对方断开socket来获知对方的数据发送完成。非常不优雅。

参考<br>
[浅谈TCP/IP网络编程中socket的行为](https://www.cnblogs.com/promise6522/archive/2012/03/03/2377935.html)<br>
[Unix/Linux中的read和write函数](https://www.cnblogs.com/xiehongfeng100/p/4619451.html)

- demo-02

采用blade-build编译 使用c++兼容c的形式
