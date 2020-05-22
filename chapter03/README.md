[TOC]
## chapter03 地址族与数据序列

### Basic

q:本章是如何组织的?
3.1. 分配给套接字的ip地址与端口号
3.2. 地址信息的表示
3.3. 网络字节序与地址变换
3.4. 网络地址的初始化与分配
3.5. 基于windows的实现

q:如何表示ipv4地址
```c
struct sockaddr_in {
  sa_family_t sin_family;  // Address family                (4B)
  uint16_t sin_port;       // Port                          (2B)
  struct in_addr sin_addr; // Ip address(network order)     (4B)
  char sin_zero[8];        // Not used                      (8B)
};

struct in_addr {
  In_addr_t s_addr;
};

struct sockaddr {
  sa_family_t sin_family;
  char sa_data[14];
};
```

q:sockaddr_in是为ipv4设计的地址结构，那为什么还需要sin_family?
>为了和sockaddr兼容，后这不是为ipv4设计的。
我们在使用bind时本质是填充sockaddr

q:sin_zero的意义何在?
>同上。也是为了兼容sockaddr

q:sockaddr_in与sockaddr的关系
>前者是为ipv4设计的地址结构。后者是通用地址结构。所以我们需要通过填充sockaddr_in来间接填充sockaddr

q:什么是host byte order?
>cpu向内存保存数据的方式。通常有两种：
1. Big Endian:  高位字节存放到地位地址(逻辑高位存储到物理低位)
2. Small Endian: 高位字节存放到高位地址(逻辑高位存储到物理高位)

q:什么是network byte order
>Interl和Amd大都采用小端存储。以上讨论的均是host order,即cpu将数据存储在内存的方式。
network byte order则是数据在进行网络交互时,逻辑字节填充到物理字节的顺序.
引入网络字节序的问题，主要是为了统一不同host byte order在网络传输不一致的问题。
引入网络字节序之后：
1.发送数据前，主机先将host byte order -> network byte order
2.接受数据后，主机先将network byte order -> host byte order

```c
书面逻辑表示: 0x12345678
计算机存储表示(Intel AMD 小端存储)
0x78
0x56
0x34
0x12
计算机输出，从左向右，物理高位到低位(这个顺序是不变的) 0x12345678

大端存储
0x12
0x34
0x56
0x78
计算机输出，0x78563412
```

q:真实编程的时候，是否需要programmer进行如上操作？
>大部分情况都不需要，os api替我们做了这些事情。
除了向sockaddr_in当中填充数据的时候。

q:常见api?
```c
// Dotted Decimal Notation表示，转换为32位ip地址
in_addr_t inet_addr(const char *cp);

// 同上
// 自动填充struct in_addr
int inet_aton(const char *cp, struct in_addr *inp);

// 32位ip地址转化为Dotted Decimal Notation
char *inet_ntoa(struct in_addr in);
```

q:INADDR_ANY ?
>When a process wants to receive new incoming packets or connections,
it should bind a socket to a local interface address using bind.
>
>这句话这么理解，我们知道socket关联了一个connection。但是具体到主机，该由那个进程来获取这个connection的数据呢？
那么此时，把一个socket关联到local interface address(ip:port)，可以送到具体的进程。
>
>In this case, only one IP socket may be bound to any given local
(address, port) pair.When INADDR_ANY is specified in the bind call,
the socket will be bound to all local interfaces
>
>下面举一个例子：
比如你的机器有三个ip   
192.168.1.1   
202.202.202.202   
61.1.2.3    
如果你serv.sin_addr.s_addr=inet_addr("192.168.1.1");      
然后监听32879端口   
这时其他机器只有connect   192.168.1.1:32879才能成功。   
connect   202.202.202.202:32879和connect   61.1.2.3:32879都会失败。    
如果serv.sin_addr.s_addr=htonl(INADDR_ANY);无论连接哪个ip都可以连上的,只要是往这个端口发送的所有ip都能连上。
>
>一台机器有多个网卡常见，这么做的前提也是这台机器是当做逻辑上的一台机器使用才行。比如你要在每个网卡上各启一个不同的服务，那肯定不行。
只有启动一个服务(绑定一个端口)，才可以从不同的ip上获取数据。

[ip - Linux IPv4 protocol implementation](http://man7.org/linux/man-pages/man7/ip.7.html)<br>
[Understanding INADDR_ANY for socket programming](https://stackoverflow.com/questions/16508685/understanding-inaddr-any-for-socket-programming)<br>
[socket绑定的ip为INADDR_ANY 的意义](https://blog.csdn.net/qq_26399665/article/details/52932755)

```c
# define INADDR_ANY ((unsigned long int) 0x00000000)
```

### demo-01
主机字节序转换的demo

### demo-02
字符串信息转化为网络字节序的整数型

q:网络字节序的目的是什么？
>因为host order 不统一，有大端，有小端。所以在进行网络传输的时候，进行统一

q:ip地址不是字符串吗？
>当然不是，ip地址是32位无符号整形数据.只不过，平时我们采用点分十进制表示，是为了方便阅读.

```c
typedef uint32_t in_addr_t;

struct in_addr {
  in_addr_t s_addr;
};
```

q:inet_addr有什么bug?
>If the input is invalid, INADDR_NONE (usually -1) is 
returned.  Use of this function is problematic because -1 is a valid address (255.255.255.255)

### demo-03
提供安全的字符串信息转化为网络字节序的办法

### demo-04
网络字节序转化为字符串信息
