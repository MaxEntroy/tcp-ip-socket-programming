### demo-01

一个基础的客户端-服务器模式的server，尚未实现echo功能

- 内存泄露
```c
  while(1) {
    printf("[%s:%s] listening...\n", argv[1], argv[2]);

    int clnt_sfd = accept(serv_sfd, (struct sockaddr*) &clnt_addr, &clnt_addr_sz);
    if(clnt_sfd == -1) {
      perr_handling("accept", "error");
    }
    printf("[%s:%u] connected!\n", inet_ntoa(clnt_addr.sin_addr), clnt_addr.sin_port);

    // send message to client
    const char message[] = "hello,world!";
    write(clnt_sfd, &message, sizeof(message));

    // 这里特别小心，一旦丢失socket的句柄，对应资源会发生泄露。
    // 进程如果挂了，并没事，进程结束资源会回收。
    // 如果在while loop里面，则会出泄露
    close(clnt_sfd);
    printf("[%s:%u] disonnected!\n", inet_ntoa(clnt_addr.sin_addr), clnt_addr.sin_port);
  }
```

### demo-02

q:什么是底层?
>通俗来说，越接近硬件，越底层。当我们在网络编程谈起底层时，我们是说与标准无关的、操作系统独立提供的。
eg: Linux提供的文件IO函数，就非ANSI提供。

