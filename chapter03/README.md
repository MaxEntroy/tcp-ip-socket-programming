### demo-01

大端小端

q:大端小端什么区别？
>Big Endian: 逻辑高位存储在物理低位
Little Endian: 逻辑高位存储在物理低位

Interl和Amd大都采用小端存储。以上讨论的均是host order,即cpu将数据存储在内存的方式。
net order则是数据在进行网络交互时，存储的方式。

### demo-02
字符串信息转化为网络字节序的整数型

q:网络字节序的目的是什么？
>因为host order 不统一，有大端，有小端。所以在进行网络传输的时候，进行统一

q:ip地址不是字符串吗？
>当然不是，ip地址是32位无符号整形数据.只不过，平时我们采用点分十进制表示，是为了方便阅读.

```c
typedef uint32_t in_addr_t;

struct in_addr {
  in_addr_t s_addr;
};
```

q:inet_addr有什么bug?
>If the input is invalid, INADDR_NONE (usually -1) is 
returned.  Use of this function is problematic because -1 is a valid address (255.255.255.255)
