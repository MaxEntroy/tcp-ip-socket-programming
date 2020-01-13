### demo-01

这个demo解决了chapter04/demo-01中提到的io_event的问题,我们看下实现

```c
// client
void do_io_event(int clnt_sfd) {
  const char* msg = "hello, echo server, i am echo client.";
  size_t msg_len = strlen(msg);
  char buf[BUF_SZ];

  write(clnt_sfd, msg, msg_len);
  Read(clnt_sfd, buf, msg_len);
  buf[msg_len] = '\0';
  printf("%s\n", buf);
}

ssize_t Read(int fd, void* buf, size_t count) {
  size_t nread = 0;
  while(nread < count) {
    ssize_t n = read(fd, buf+nread, count);
    if(n == -1) {
      if(errno == EINTR) {
        n = 0;
      }
      else {
        return -1;
      }
    }
    else if(n == 0) {
      break;
    }

    nread += n;
  }
  return nread;
}


// server
void do_io_event(int clnt_sfd) {
  char buf[BUF_SZ];
  int nread = 0;

  while((nread = read(clnt_sfd, buf, BUF_SZ)) > 0) {
    write(clnt_sfd, buf, nread);
  }
}
```

q:echo_client实现的依据?
>Read函数做的比较全面了，主要考虑了３种case
1.读失败
2.读结束(比如文件这种，有eof作为结束标志的，但是网络io没有)
3.读正常

q:echo_server实现的依据?
>server端无法得知client端结束的依据，只能根据以下2种case判断
1.读结束(磁盘io适合，有eof作为结束标志)
2.读失败(等待client端结束，关闭socket，server端知道读端结束了)

### demo-02

- 支持zlog

### demo-03

- 测试zlog文件转档

zlog根据时间戳就行文件转档确实很方便


### demo-04

q:为什么发送结构体，不是好的数据通信方式？好的数据通信方式又有哪些标准？
>先说好的数据通信方式：可扩展，跨语言，跨平台
结构体之所以不是好的数据通信方式，从这3个点说就好。跨语言？显然结构体不夸语言。跨平台？cpu体系结构不一致，同一个类型的数据，所占字节不一致，内存对其方式也不一致。
可扩展？不方便实现，比如本次的需求，传递多个opnd，可能需要每次都改struct 结构。但是pb repeated field就能很好的解决这个问题.

q:cstdio and stdio.h的选择?
>采用陈硕的建议，除了cpp标准库，其余都采用.h的形式
需要特别注意的是，c兼容的库，要注意变量是否在std中，我看到的应该都是在全局命名空间中
