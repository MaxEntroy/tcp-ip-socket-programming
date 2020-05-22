[TOC]
## chapter02 套接字类型与协议设置

### Basis

q:本章是如何组织的?
2.1. 套接字协议及其数据传输特性
2.2. windows平台下的实现及验证

q:socket api
```c
#include <sys/socket.h>
int socket(int domain, int type, int protocol);
@domain: The domain argument specifies a communication domain; this selects the protocol family which will be used for communication
@type: which specifies the communication semantics.
@protocal: The protocol specifies a particular protocol to be used with the socket.
```

q:套接字类型的区别
1. SOCK_STREAM     Provides sequenced, reliable, two-way, connection-based byte streams.  An out-of-band data transmission mechanism may be supported.
2. SOCK_DGRAM      Supports datagrams (connectionless, unreliable messages of a fixed maximum length).

q:这里需要解释一个问题，什么是连接？
可以简单理解为，需要通过数据(state)来维护通信链路的状态。
如果一个通信链路有状态，我们就说它面向连接。否则，通信链路无连接

q:面向连接的套接字？
1. 数据有序
2. 数据不丢失
3. 不存在边界

对于数据不存在边界，可以想想家里的水龙头，我们不会考虑这个水流是否有边界，反正打开就能用。
我们也不在乎客户端发送了多少次数据，反正对于我们打开就可以用

q:面向消息的套接字?
1. 数据无序
2. 数据可能丢失
3. 数据存在边界(每次传输限制数据大小)
4. 强调快速传输
