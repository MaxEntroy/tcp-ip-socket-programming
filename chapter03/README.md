### demo-01

大端小端

q:大端小端什么区别？
>Big Endian: 逻辑高位存储在物理低位
Little Endian: 逻辑高位存储在物理低位

Interl和Amd大都采用小端存储。以上讨论的均是host order,即cpu将数据存储在内存的方式。
net order则是数据在进行网络交互时，存储的方式。
