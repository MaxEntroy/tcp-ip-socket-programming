### demo-01

echo-server实现，注意以下
- echo-client没必要实现成while-loop
- tcp没有数据边界

本次实现的问题，看客户端io_event的实现
```c
void do_io_event(int clnt_sfd) {
  const char* msg = "hello, echo server, i am echo client.";
  char buf[BUF_SZ];

  write(clnt_sfd, msg, strlen(msg));
  int nread = read(clnt_sfd, buf, sizeof(buf));
  buf[nread] = '\0';
  printf("%s\n", buf);
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


q:non-blocking io和blocking io的区别?
>对于read函数:
1.1.blocking模式下,receive buffer为空，阻塞
1.2.non-blocking模式下，receive buffer为空，非阻塞，返回-1(erron为EAGAIN或EWOULDBLOCK)
对于write函数:
2.1.blocking模式下，只要应用层数据没有完全拷贝到kernel tcp send buffer，阻塞
2.2.1.non-blocking模式下，应用层buf <= tcp send buffer，返回应用层buf字节数
2.2.2.non-blocking模式下，应用层buf > tcp send buffer and tcp send buffer != 0，返回实际拷贝到tcp send buffer的字节数
2.2.3.non-blocking模式下，应用层buf > tcp send buffer and tcp send buffer == 0，返回-1

参考<br>
[浅谈TCP/IP网络编程中socket的行为](https://www.cnblogs.com/promise6522/archive/2012/03/03/2377935.html)<br>
[Unix/Linux中的read和write函数](https://www.cnblogs.com/xiehongfeng100/p/4619451.html)