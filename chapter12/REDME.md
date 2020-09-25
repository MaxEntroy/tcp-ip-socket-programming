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
