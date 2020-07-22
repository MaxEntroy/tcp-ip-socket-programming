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
1. udp socket不维护状态，因此没有listen and accept过程。udp socket只有创建和数据发送的过程
2. udp socket对于client/server均只有一个socket，其实从udp socket本身来说，没有client/server区别。是对等通信的方式，client/server的划分是我们结合业务来说的

q:udp socket io?
1. tcp socket创建完成后，数据传输时无需再增加地址信息。因为tcp socket维护连接，即知道对方的地址信息
2. udp socket由于不维护连接，所以无法知道对端的地址信息，需要在每次发送数据时，填写地址信息


### 实践
