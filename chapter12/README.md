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

