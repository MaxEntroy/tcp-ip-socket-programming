## chapter08 域名及网络地址

### Basic

q:域名有什么用？
>ip:port形式难以记录，所以采用domain name进行替换

q:dns server?
>将domain name和ip:port进行转换，因为实际访问还是要通过ip:port

q:程序中有必要使用域名吗？
1.以docker服务来说，ip:port经常变换，如果写死ip:port，显然不是一个好办法
2.所以，应该在应用程序中使用名字。我们实践中也是这么做的。

### 实践