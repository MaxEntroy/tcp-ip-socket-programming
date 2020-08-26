## chapter06 基于UDP的服务器端/客户端

### Basic

q:本章是如何组织的？
6.1.理解UDP<br>
6.2.实现基于UDP的服务器端/客户端<br>
6.3.UDP的数据传输特性和调用connect函数<br>

q:udp socket vs tcp socket
1. 流控制是区分tcp and udp的核心原则。对于tcp而言，如果去除流控制，那么也所剩无几
2. tcp的数据传输速率无法超过udp，只有当每次交换的数据量越大，tcp的传输速率才有可能接近udp

q:udp的作用？
>udp作为ip层次之上的协议，完成的就是端到端的传输。ip负责路由的选择，udp负责通过端口，来将数据转发到对应的端口(本质是进程)。

q:udp的高效实用？
1. 如果服务对于丢包0容忍，那么只能用tcp。比如传输一个压缩文件，丢掉任何一个包，都可能导致解压失败
2. 如果对于丢包能接受，且速度是非常重要的因素。那么udp可以考虑，比如实时视频服务。
3. tcp比udp慢，通常有以下两个原因
3.1. 三次握手/四次挥手的过程，即收发数据前后的准备和销毁工作
3.2. 数据收发过程中，为了保障可靠性增加的flow control

这么看，如果是**小包传输，且链接频繁**可以考虑使用udp协议

q:udp socket有哪些特点？
1. udp socket不维护状态，因此没有listen and connect/accept过程。udp socket只有创建和数据发送的过程(没有请求连接和请求受理的过程)
2. udp socket对于client/server均只有一个socket，其实从udp socket本身来说，没有client/server区别。是对等通信的方式，client/server的划分是我们结合业务来说的

q:udp socket io?
1. tcp socket创建完成后，数据传输时无需再增加地址信息。因为tcp socket维护连接，即知道对方的地址信息
2. udp socket由于不维护连接，所以无法知道对端的地址信息，需要在每次发送数据时，填写地址信息
3. tcp socket io基于流式，所以接收数据时需要业务侧保证收到指定字节的数据，socket无法保证。udp socket则保证收到指定字节的数据。

q:为什么udp socket需要bind address?
1. 每一个socket在进行网络通信的时候，都需要bind address(ip:port)
2. udp socket不需要维护连接状态，这根是否需要bind没有关系
3. tcp client/udp client都不需要bind原因是，tcp socket/udp socket需要使用地址，但是这个地址别人不在乎，只有他们自己在用,所以随机分配即可
4. tcp server/udp server都需要bind原因是，如果随机分配，tcp client/udp client不知道连接哪个哪个服务

### 实践

- demo-01

1.自己封装了udp-server/udp-client
2.基于封装，实现了echo-server/echo-client

在具体实践过程中，从代码层面体会udp/tcp的区别
1.没有connect(client)-accept(server)过程(这个过程本质是三次握手建立连接的过程)
2.server slide没有listen过程，不区分listen socket and communication socket，只有一个communication socket。这里也是因为没有connect-accept过程导致
3.采用recvfrom/sendto 进行io，这两个函数的特点是需要 获取/给出 对端地址(这里的根本原因是，没有连接,socket不会维护对端的地址，所有需要自己维护)
4.recvfrom一次获取。回顾tcp的数据获取过程，需要实现read_n函数，来保证流式数据中获取完整的数据包，但是udp传输不基于流式传输，一次获取可以获取完成

q:sendto errno=22?
>这里是一个比较常见的问题：
errno=22 表示，invalid argument，下面的参考里面说的很清楚。主要是两个可能，1.地址传递有问题 2.socket绑定有问题
>
>下面我们仔细查看了recvfrom api, 
If  src_addr  is  not NULL, and the underlying protocol provides the source address, this source address is filled in.  
When src_addr is NULL, nothing is filled in; in this case, addrlen is not used, and should also be NULL.  
The argument addrlen is a value-result argument, which the caller should initialize before the call to the size of the buffer associated with src_addr, 
and modi‐fied  on return to indicate the actual size of the source address.  
The returned address is truncated if the buffer provided is too small; 
in this case, addrlen will return a value greater than was supplied to the call
>
>文档里说的很清楚，addrlen是个传出参数，按照常理，这个参数需要被覆盖，那么对于他的初始值我们不在乎。
但是，文档里单独强调了，传入前，必须对这个值进行初始化，大小是结构体的大小。
>
>所以，这个问题的反思在于，还是主观给了基础假设，但实际上不是这样。所以，使用未知函数时，仔细看文档是重要的。

[socket sendto get the error 22 during udp packets](https://stackoverflow.com/questions/20502100/socket-sendto-get-the-error-22-during-udp-packets)

- demo-02

1.使用自己封装的udp-server/udp-client
2.开发cal-server/cal-client

这个demo的主要作用是和tcp的服务进行对比，明白两种协议的异同,主要的一些心得
1. 先不谈底层封装的细节，业务侧使用udp和tcp的唯一区别就是对于流式数据在发送/接受的处理。
2. udp不是流式数据，所以发送次数和接受次数严格相等.不会出现tcp流式数据，一次发送，多次接受的问题，原因主要在于udp数据非流式，有数据边界
3. 还有一个特别重要的点是：如何获得本次数据包体的大小，是应用层协议需要处理的，和传输层没有关系。因此我特意对同一个业务分别用tcp/udp实现的原因也在于此，哪些是传输层解决的，哪些是应用层解决的，需要了然。

q:bug?
>主要是resize/reserve的使用。
1.下面的代码，我第一次使用了reserve，是因为我考虑到如果使用resize，那么初始化的数据我其实不用，我真正用的是后面构造的。所以使用reserve
2.但是，接收的时候，需要给出header_buf[0]的地址，这就要求这个下标有实际的元素。所以需要构造，改为resize

```cpp
std::string header_buf;
header_buf.resize(HEADER_LEN); // 误用了header_buf.reserve
recvfrom(sfd, &header_buf[0], HEADER_LEN, 0, NULL, NULL);
```


