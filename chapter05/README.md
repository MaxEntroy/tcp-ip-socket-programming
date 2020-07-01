## chapter05 基于TCP的服务器端/客户端(2)

### Basic

q:本章是如何组织的?
5.1.回声客户端的完美实现<br>
5.2.TCP原理<br>
5.3.基于Windows的实现<br>

q:tcp io kernel buffer有什么特点？
- io buffer在每个tcp socket当中单独存在
- io buffer在创建tcp socket时由kernel自动生成
- 即使关闭socket也会继续传输 send buffer的内容
- 关闭socket，则会丢失recv buffer的内容

q:write/read函数返回的时间点？
>这个结合上一章的总结来看，主要考虑两个方面，一是是否是阻塞io，二是返回的时间点主要取决user space和kernel space io buffer之间的交互。
而不是说确保对端当下收到数据，对端肯定能收到数据，这个由kernel保证，但是返回的时候，不一定能收到。

q:tcp的三次握手?

q:tcp的数据交互?

q:tcp的四次挥手？

### 实践

1. demo-01解决了echo server没有考虑tcp传输特性的问题
2. demo-02自己设计应用层协议，解决了calculator.
3. demo-03还是自己设计应用层协议，采用pb + MSG_HEAD_SZ来作为应用层协议

- demo-01

q:目前实现的echo server有什么问题?
>我们分别看下server side和client side实现
以上的实现都具有一个显著特点就是:read只调用一次,那么这回导致什么问题呢?
1. 我们在transport layer是基于tcp协议开发的,tcp传输特性是数据没有边界
2. 考虑client发送的数据,包体太大,内核需要填充两次send buffer.但是此时server的read函数,有数据就返回,那么server端会丢掉客户端发送的第二次数据
3. 对于server发送的数据,客户端存在同样的问题.
4. 既然tcp协议的传输特性是没有数据边界,那么在进行数据传输时,需要制定传输数据的大小.

```cpp
// server
void do_io_event(int clnt_sfd) {
  char buf[BUF_SZ];

  int nread = read(clnt_sfd, buf, BUF_SZ);
  write(clnt_sfd, buf, nread);
}

// client
void do_io_event(int clnt_sfd) {
  const char* msg = "hello, echo server, i am echo client.";
  char buf[BUF_SZ];

  write(clnt_sfd, msg, strlen(msg));
  int nread = read(clnt_sfd, buf, sizeof(buf));
  buf[nread] = '\0';
  std::cout << buf << std::endl;
}
```

q:如何改进?
1. 对于server side,没必要按照数据大小来获取,server一直获取数据,直到client读取完全部数据,断开socket.此时server read eof,结束获取即可
2. 对于client side,那么必须要按照数据大小获取数据.发送了多少数据,必须接受多少数据.

```cpp
// server
void do_io_event(int clnt_sfd) {
  char buf[BUF_SZ];
  int nread = 0;

  while((nread = read(clnt_sfd, buf, BUF_SZ))) {
    write(clnt_sfd, buf, nread);
  }
}

// client
void do_io_event(int clnt_sfd) {
  const char* msg = "hello, echo server, i am echo client.";
  char buf[BUF_SZ];
  int str_len = strlen(msg);

  write(clnt_sfd, msg, str_len);
  read_n(clnt_sfd, buf, str_len);
  buf[str_len] = '\0';
  std::cout << buf << std::endl;
}

void read_n(int fd, char buf[], int n) {
  int total = 0;
  int nread = 0;
  while(total < n) {
    nread = read(fd, buf+total, BUF_SZ);
    if (nread == -1) {
      if (errno == EINTR)
        nread = 0;
      else
        return;
    }
    else if (nread == 0)
      break;

    total += nread;
  }
}
```

- demo-02

q:对于echo server目前的解决方案,有什么问题?
>对于这个case,我们可以知道client发送的数据大小,如果要是不能直接获取,怎么办?

**结论:如果问题不在回声客户端,定义应用层协议**

q:为什么要定义应用层协议?
>从上面的例子中,我们可以看出来,对于数据收发边界这一类问题,不是传输层能解决的,因为这个问题和业务强耦合.
不同的业务有各自不同的选择,所以,设计应用层协议的目的就在此.针对特定应用,制定的数据传输规则集合,就叫做应用层协议。
其实，整个tcp协议栈就是解决好网络数据传输这么一件事情。

q:协议以什么样的形式体现？
1. 规则集合
2. 对于协议双方透明，且双方都需要遵守哦。

接下来，我们看一个实际的问题，从中体会定义应用层协议的过程。切记，应用层协议不是高深莫测的存在，只不过是针对特定应用，定义的数据传输规则。

q:实现一个如下计算器，直接给出操作界面，根据此实现这个服务？
>client side:
operand count: 3
operand 1: 12
operand 2: 13
operand 3: 11
operator: -
operation result: -12

此时，客户端无法向echo client那样，知道自己发送多少数据，相应的，会接受多少数据。所以，此时需要更为详细的应用层协议来定义数据传输规则。

q:如何定义应用层协议？
>根据我们前面的总结，关键是把数据边界描述清楚。尤其是对于接收端，要接受多少个字节的数据，需要定义清楚
1. 对于client，发送多少字节数据，这个信息怎么让server知道
2. 对于server，发送多少字节数据，这个信息怎么让client知道

**下面给出本次的协议：**
1. client发送数据协议，第1个字节操作符，第2-5个字节操作数数量。第6-n个字节存放操作数。
2. server发送数据协议，第1-4个字节存放结果。

<img width="700"  src="img/cal_protocol.png"/>

代码协议如下：
```cpp
#ifndef CAL_PROTOCOL_H_
#define CAL_PROTOCOL_H_

// Request protocol
#define REQ_OPND_NUM_SZ 4
#define REQ_OPND_SZ     4
#define REQ_OPTR_SZ     1

// Response protocol
#define RES_RESULT_SZ   4

#endif
```
- 对于server，先读取REQ_OPTR_SZ + REQ_OPND_NUM_SZ获取optr and opnd_num。解析出opnd_num，进行opnd_num轮读取，每次读取REQ_OPND_SZ个字节
- 对于client，读取RES_RESULT_SZ个字节即可。

q:实践过程中的问题？
1. 为什么直接利用buf发送，不用考虑字节序，大端小端的问题
2. io_read_n实现错误
3. dynamic array的选择
>对于第一个问题，这么考虑，即使序列化方法，最后还是只能用read/write这些方法进行网络io，所以总得用不需要序列化的读取/写入方法。这个方法就是直接进行字节流的传输。
本次的实现，我就是对于buf填充数据，直接进行字节流传输
>对于第二个问题，因为io_read_n实现的时候，具体调用read每次还是读取BUF_SZ,这和第三个参数矛盾了。比如，缓冲中有100byte数据，读取三次，30/30/40这么读取。
但是第一次读取的时候，直接读取了BUF_SZ，直接导致tcp recv buffer被读取完毕，再进行第二个读取的时候，触发阻塞条件。
>对于第三个方法，本身我在纠结用malloc/free 还是 new/delete，后来想，既然c++用了，合理的特性为什么不用。
尝试直接使用shared_ptr,后来查阅写cpp-primer,Best practices: Most applications should use a library container rather than dynamically allocated arrays. Using a container is easier, less likely to contain memorymanagement bugs, and is likely to give better performance. 
顿悟，采用vector解决

```cpp
void io_read_n(int fd, char buf[], int n) {
  int total = 0;
  while(total < n) {
    int nread = read(fd, buf+total, n - total);
    if (nread == -1) {
      if (errno == EINTR)
        nread = 0;
      else
        return;
    }
    else if (nread == 0)
      break;

    total += nread;
  }
}
```

目前保持这样的规范
1. 使用g++进行编译
2. 使用gtest/gflags，暂时不用glog
3. dynamic memory使用c++特性
4. io使用c特性，禁用c++特性

- demo-03

q:demo-02的实现有什么问题？
1. 没有问题
2. 但是非常繁琐，相当于自己实现了序列化的过程。

q:demo-02回顾？
1. 先强调demo-02当中忽略的一点，传输层为什么要选择tcp协议？因为,后端服务之间的通信，为了减少网络连接打开和关闭带来的开销，一般都会选用tcp长连接作为通信传输协议的
2. tcp会带来什么问题？TCP/IP由于是长连接且是面向连接设计，因此需要设计应用层的规范。具体来说tcp的传输特性是一种没有边界的字节流，因此必须通过设计应用层协议来判断一个包是否收齐

q:对于demo-03的问题如何解决？
1. 序列化的问题，可以选用一种通用的数据交换格式，后者会提供序列化和反序列化的方法，不用自己实现，即数据交换协议不用自己实现。demo-02的数据交换协议完全是自己设计的(每个操作数的大小，操作符的大小。)
2. 序列化的问题只是解决了数据交换格式的协议，这只是应用层协议设计的一部分。对于tcp没有边界字节流的特性，任然要进行应用层设计。(demo-02的设计，不仅规定了操作数的大小，也规定了包的顺序，我们知道解析第二个字段，4字节，可以获得实际数据包体大小，然后再去解析操作数。)因此，这里任然需要设计一个协议，来保证可以获得完整的数据包。

q:应用层协议应该如何设计？
1. 参考1当中的文章，从0到1，设计并实现了了数据体，数据头。
2. 参考2当中的文章，统一采用pb来作为包头和包体，好处是避免了自己实现序列化方法。同时，规定了8个字节，作为包头标识，和包头长度。(仅仅通过一个pb，我们没法确定这个包体到底有多大。参考一是通过结构体，可以进行准确的计算得知，当然，如果前者有string这样的变量也不行，但是它的包头结构体只有一个uint32_t，这个是可以确定大小。)
3. 终归到底，需要考虑tcp的传输特性来进行协议的设计
4. 结合倒数第二篇参考文献，原来tcp传输特性导致的数据包无法识别的问题(字节流，没有边界，自然无法识别哪里开始，哪里结束)叫做粘包
5. 结合最后一篇文献，讲到了，有些地方需要外部系统进行设计，有些则是pb解决的

q:我的方案？
1. 对于包体长度，虽然可以指定4字节标示，但是我还是想放到一个规范的结构里面来表示
2. 规范的结构，可以放pb，但是pb序列化之后的大小又没法确定
3. 直接研究proto buffer encoding，看下是否能找到一个方案确定大小，结论如下：
3.1. binary format编码，key(field_number + wire_type) + value
3.2. 其中field_number由于在1-15内，所以整体key整体占用1byte, msg_length如果采用fix32形式，固定长度为4bytes。整个包体序列化(binary format)的长度为5 bytes.

最终，我可以给出一个序列化之后确定长度的包体表示
```proto
message MsgHead = {
  required fixed32 msg_length = 1;
}
```

参考<br>
[一种简单应用通信协议的设计](https://zhuanlan.zhihu.com/p/84749337)<br>
[开源项目SMSS发开指南（三）——protobuf协议设计](https://www.cnblogs.com/learnhow/p/12200200.html)<br>
[Netty(三) 什么是 TCP 拆、粘包？如何解决？](https://crossoverjie.top/2018/08/03/netty/Netty(3)TCP-Sticky/)<br>
[Protobuf协议在Nebula的应用](https://zhuanlan.zhihu.com/p/65520632)<br>
[结合RPC框架通信谈 netty如何解决TCP粘包问题](https://cloud.tencent.com/developer/article/1187023)<br>
[什么 Protobuf 的默认序列化格式没有包含消息的长度与类型？](https://www.cnblogs.com/Solstice/archive/2011/04/13/2014362.html)<br>
[Is it possible to use an std::string for read()?](https://stackoverflow.com/questions/10105591/is-it-possible-to-use-an-stdstring-for-read)