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

大端小端

q:大端小端什么区别？
>Big Endian: 逻辑高位存储在物理低位
Little Endian: 逻辑高位存储在物理低位

Interl和Amd大都采用小端存储。以上讨论的均是host order,即cpu将数据存储在内存的方式。
net order则是数据在进行网络交互时，存储的方式。
