## chapter12 I/O复用

### Basic

1. 基于I/O复用的服务器端
2. 理解select函数并实现服务器端
3. 基于windows的实现

q:Blocking?
>Blocking. You’ve heard about it—now what the heck is it? In a nutshell, “block” is techie jargon for “sleep”.
当然，这里其实在os进程状态有过讨论，这里再给出一个简短的说明

q:函数blocking的原因是什么？
>Lots of functions block. accept() blocks. All the recv() functions block. The reason they can do this is
because they’re allowed to.
>
>这里要特别注意的是，对于网络相关的函数，blocking和socket的状态有关系。对于一个blocking fd，那么read函数在获取不到数据时，会进入blocking状态。
但是，如果一个socket被设置为non-blocking，那么read函数则表现为non-blocking行为。
所以，这里一定要理解，网络相关函数blocking的真正原因是什么

q:把socket fd设置为non-blocking，有什么好处？
>By setting a socket to non-blocking, you can effectively “poll” the socket for information.
当然，我觉得这句话写的不够好。更有效只是说，你可以得知数据是否ready。
但是，非阻塞真正的好处还是在于，占有cpu，可以继续做别的事，一旦阻塞让出cpu之后，就只能等待被调度
>
>当然，非阻塞程序会向下执行，而语义是同步的，我们只能轮训判断。显然，这种poll操作，会让cpu busy-waiting.

q:io复用的优点？
>从上面非阻塞fd这个角度来说，如果我们自己对socket fd进行polling，不够优雅。主要是会让cpu忙等。
此时，把这个能力交给内核。
we’re going to ask the operating system to do all the dirty work for us, and just let us know
when some data is ready to read on which sockets. In the meantime, our process can go to sleep, saving
system resources

### 实践

- demo-01

select server.

q:io event loop，也是循环处理，不处理完一个，无法处理下一个，这个效率高在哪里？
>虽然不处理玩一个无法处理下一个，但是此时cpu没有阻塞，而是在running.所以，cpu资源没有浪费。
这是io复用的优点，就是io ready时才返回，此时cpu处理即可，cpu并没有阻塞在io上，资源没有浪费。
其次，由于cpu处理很快，所以cpu的处理loop没有浪费处理机资源，再快也就是这样了

q:select一些坑？
>select对于fd_set以及timeout是传出参数，所以要保存副本。每次传入时进行更新

q:目前的实现是否有问题？
>HandleIoEvent实现的有问题。
多进程服务器，由于子进程进行fd的阻塞read，父进程没有被阻塞得以处理connection.
但是，io 复用的作用是监听所有fd io event.但是，父类规定的HandleIoEvent是处理完成所有数据。
当前程序没有bug，是因为一次发送数据发送完成了。我们在HandleIoEvent当中调用了io_aux当中的io_read_n函数。
保证tcp的多次传输，可以确保获取完整的数据。但是select只监听了一次，比如传递了部分数据。
HandleIoEvent第一次读不阻塞，但是没有获取数据完成，会继续调用read，阻塞住当前进程。导致io 复用失败。
>
如何解决，如果还要保持当前的结构。只能是HandleIoEvent保持状态，不然没有办法。只有获取的数据读取完毕，才能继续进行
leo给了建议，需要管理fd的buffer
>
暂时没有想到特别好的方式，先没有解决这个问题。先向下走，到epoll看看有没有好的办法

- demo-02

poll sever.面临和select sever一样的情形。

demo-02犯了一个非常典型的错误-对于指针的更新，只能利用二维指针

```cpp
下面是错误的代码，当然调用也一定错了。
1.c/c++基础类型，值传递. pfd_arr值传递，副本和原来的值，指向了同一个地址
2.AddfdToPoll更新了形参的值，但是实参没有改变
3.但是，sz发生了改变
4.下次poll的时候，pfd_arr没有改变，但是sz发生了改变。下溢
5.这段代码，还内存泄露了。形参开辟了dynamic memory，但是没有回收
static void AddfdToPoll(struct pollfd* pfd_arr, int fd, int* sz, int* cap) {
  if (*sz == *cap) {
    *cap = 2 * *cap;
    printf("cap = %d.\n", *cap);
    pfd_arr = static_cast<struct pollfd*>(realloc(*pfd_arr, (*cap) * sizeof(struct pollfd)));
    if (!pfd_arr) {
      err_handling("realloc", "error");
    }
  }

  pfd_arr[*sz].fd = fd;
  pfd_arr[*sz].events = POLLIN;

  (*sz)++;
}

// 正确的接口
static void AddfdToPoll(struct pollfd** pfd_arr, int fd, int* sz, int* cap)

// 课本的接口
static void AddfdToPoll(struct pollfd* pfd_arr[], int fd, int* sz, int* cap)

// 多问一个，如果一个指针数组，应该怎么给出更新时的形参地址
// 指针数组，每个元素是指针。一个*，数组再给一个*。如果形参要更新，再给一个*

```

- demo-03

解决了demo-01/demo-02存在的问题。这里我想强调一点，只有有了思路。不管这个思路的好坏，在你没有别的思路的前提下。把它实现了就好，因为一个不好的办法，总比什么都没有好。
并且，不好的方法，有时候可能也是非常主观的。比如，这个例子最后的办法，就是提供对于fd buffer的管理，否则无法管理状态。tcp确实存在分包的问题，这个没办法，所以一开始的思路就是正确的。
只是，最后怀疑自己的方法不够好，没有实现

下面，我们开始尝试解决这个问题。在解决过程中，顺带解决了很多其他有意义的问题，这里也一并进行总结。

q:当我们使用io多路复用的时候，为什么一定要设置listen_sfd nonblocking?
>这个问题需要小心的地方在于，当我们不使用select时，这么做没有问题。
当我们使用多路复用时，这个bug会出现。对于这个问题的解决，unp给了非常好的解释。当然，这又需要我们了解一下其他的问题

q:Connection Abort before accept Returns
>参照参考文献1当中时序图，问题是很明确的<br>
1.client调用connect与server完成三次握手，此时connection被放入queue of pending connections(listen第二个参数指定queue大小)<br>
2.接下来调用accpet从连接就绪队列中拿出一个connection，创建新的sfd<br>
3.问题就处在1和2之间，作者假设一个busy web server(这种情况在现实中很少，但确实可能存在)，当1结束后，来不及处理2，即1和2之间存在一定time gap.但此时，一个connection被发送了一个rst<br>
4.真正的问题在于，对于这个aborted connection的处理，大家实现的并不统一<br>
4.1.Berkeley-derived implementations handle the aborted connection completely within the kernel, and the server process never sees it<br>
4.2.The steps involved in Berkeley-derived kernels that never pass this error to the process can be followed in TCPv2(由于api不返回任何错误信息，所以进程无法感知到这个aborted connecton已经被移除了)<br>
4.3.The RST is processed on p. 964, causing tcp_close to be called. This function calls in_pcbdetach on p. 897, which in turn calls sofree on p. 719. sofree (p. 473) finds that the socket being aborted is still on the listening socket's completed connection queue and removes the socket from the queue and frees the socket(这里我们明确知道，内核把aborted connection从pending queue中移除了)
4.4.When the server gets around to calling accept, it will never know that a connection that was completed has since been removed from the queue
从以上信息我们可以得知，berkely实现的根本问题在于：内核移除aborted connecton，但是不返回任何信息，所以用户进程不知道。用户进程不知道，就会一直阻塞在accept这里，等待peding queue有数据时才处理<br>

那么，我们现在开始讨论，为什么listen_sfd nonblocking和select一起使用时，会出问题。
1. 当然，这里先说下，listen_sfd和select开始配置时，设置blocking的原因是，select返回后，如果有请求来，listen_sfd一定可读，此时accept不会阻塞，没有设置nonblocking的理由
2. 一旦出现Connection Abort before accept Returns，并且此时是berkely-derived impletation，并且pending queue中除了刚才被移除的aborted connection，没有其余connection。此时，由于accept不返回任何信息，进程会阻塞在accept这里。
3. 由于此时是通过select进行fd的监听，由于进程阻塞在accept这里，有请求来也无法处理。进程会永远阻塞在这里，server出现pending.

q:如何解决这个问题？
1. Always set a listening socket to nonblocking when you use select to indicate when a connection is ready to be accepted.
2. Ignore the following errors on the subsequent call to accept: EWOULDBLOCK (for Berkeley-derived implementations, when the client aborts the connection), ECONNABORTED (for POSIX implementations, when the client aborts the connection), EPROTO (for SVR4 implementations, when the client aborts the connection), and EINTR (if signals are being caught).

参考<br>
[Connection Abort before accept Returns](http://www.masterraghu.com/subjects/np/introduction/unix_network_programming_v1.3/ch05lev1sec11.html#ch05lev1sec11)<br>
[16.6 Nonblocking accept](http://www.masterraghu.com/subjects/np/introduction/unix_network_programming_v1.3/ch16lev1sec6.html)