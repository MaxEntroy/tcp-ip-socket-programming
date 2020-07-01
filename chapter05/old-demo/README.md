以下为old_demo内容

- demo-04

q:为什么发送结构体，不是好的数据通信方式？好的数据通信方式又有哪些标准？
>先说好的数据通信方式：可扩展，跨语言，跨平台
结构体之所以不是好的数据通信方式，从这3个点说就好。跨语言？显然结构体不夸语言。跨平台？cpu体系结构不一致，同一个类型的数据，所占字节不一致，内存对其方式也不一致。
可扩展？不方便实现，比如本次的需求，传递多个opnd，可能需要每次都改struct 结构。但是pb repeated field就能很好的解决这个问题.

q:cstdio and stdio.h的选择?
>采用陈硕的建议，除了cpp标准库，其余都采用.h的形式
需要特别注意的是，c兼容的库，要注意变量是否在std中，我看到的应该都是在全局命名空间中

- demo-05/demo-05-a

本小节采用pb实现，避免上文提到的struct 作为data interchange format的问题。
本质上来说，data interchage format显然是应用从层的协议

虽然解决了data interchange format的问题，但是又带来了另外的两个问题
- tcp socket没有数据边界，如何标示数据的首尾
- pb序列化接口非常多，该用哪个

q:echo server在client端和server端的实现，保证了收发数据的完整，为什么本节的demo不行?
>回归echo server，client端作为发起数据的一方，知道字节发送数据的大小。所以，在read时，实现成Read保证需要字节的读取完成。
server端根本不在乎数据的开始和结尾，read有数据就write，毕竟完整的数据在client端可以控制。
但是，这个小节的问题在于，server端需要收到完整的数据，要进行进一步的逻辑。
>
>一个显然的办法是，在发送业务数据前，先发送该数据的大小，让server端按照这个大小进行对于业务数据的获取。
但是，这回带来一个问题是，业务数据的大小，server端怎么能保证接受完整。所以，可以发送这个业务数据大小的大小。显然，这陷入了一个死循环过程中。
>
>这个问题，我们也留到后面去看怎么解决。所以，本小节我在实现的时候，就没有考虑数据收发的完整，更多的是学到对于pb这种data interchange format的使用

q:pb序列化接口这么多，应该怎么使用?

- ParseFromArray/SerializeToArray

```cpp
void do_io_event(int clnt_sfd) {
  // recv from client
  char buf[BUF_SZ];
  ssize_t nread = read(clnt_sfd, buf, BUF_SZ);

  cal::CalRequest req;
  req.ParseFromArray(buf, nread);

  if(!req.has_seqno() || !req.has_left() || !req.has_right() || !req.has_optr()) {
    fprintf(stderr, "%s\n", "Invalid CalRequest.");
    return;
  }

  // do caculation
  cal::CalResponse res;
  do_cal(req, &res);

  // send to client
  int sz = res.ByteSize();
  res.SerializeToArray(buf, sz);

  write(clnt_sfd, buf, sz);
}
```

```cpp
bool	
ParseFromArray(const void * data, int size)
//Parse a protocol buffer contained in an array of bytes.

bool	
SerializeToArray(void * data, int size) const
//Serialize the message and store it in the given byte array.
```

通过这个demo，我看可以看出。```SerializeToArray```这个接口的使用，buf和sz都没有什么问题。```ParseFromArray```这个接口的问题在于，sz如何确定，还是需要知道

- ParseFromString/SerializeToString
```cpp
bool SerializeToString(string* output) const 
// serializes the message and stores the bytes in the given string. Note that the bytes are binary, not text; we only use the string class as a convenient container.
// 这点特别要注意，不是unp当中提到的text strings，就是binary bytes，不是character bytes.即前者不具有可解释性
bool ParseFromString(const string& data);
// parses a message from the given string.
```

```cpp
void do_io_event(int clnt_sfd) {
  // recv from client
  char buf[BUF_SZ];
  ssize_t nread = read(clnt_sfd, buf, BUF_SZ);

  cal::CalRequest req;
  std::string recv_buf(buf, nread);
  req.ParseFromString(recv_buf);

  if(!req.has_seqno() || !req.has_left() || !req.has_right() || !req.has_optr()) {
    fprintf(stderr, "%s\n", "Invalid CalRequest.");
    return;
  }

  // do caculation
  cal::CalResponse res;
  do_cal(req, &res);

  // send to client
  std::string send_buf;
  res.SerializeToString(&send_buf);

  write(clnt_sfd, send_buf.data(), send_buf.size());
}
```

对于这两个接口，注意如下：
1.SerializeToString没什么说的，write时，注意是send_buf.data()，之前一直写的是send_buf.c_str(). 通过c_str()我们知道，这个方法，返回的是包括null-terminated sequence of characters.
通过上面的接口，我们知道，string做的只是一个convenient container，是binary bytes.
data：returns a pointer to the first character of a string 
c_str：returns a non-modifiable standard C character array version of the string 

2.ParseFromString这个接口，虽然是对string进行反序列化，但是string的构造要注意，不能直接通过buf，否则会截断


[ParseFromString function of protobuf returns false](https://stackoverflow.com/questions/51805951/parsefromstring-function-of-protobuf-returns-false)
[Serialize C++ object to send via sockets to Python - best approach?](https://stackoverflow.com/questions/23785155/serialize-c-object-to-send-via-sockets-to-python-best-approach)
[protobuf进行ParseFromString解包失败的一个常见问题](https://blog.csdn.net/dreamvyps/article/details/30070877)
