# README

## this is simple socket-serve form 

https://github.com/cloudwu/socket-server 

```C++

// 创建socket_server 实例
struct socket_server * socket_server_create();

// 释放socket_server 
void socket_server_release(struct socket_server *);

// 开启socket读写
void socket_server_start(struct socket_server *, uintptr_t opaque, int id);

// 发送
int64_t socket_server_send(struct socket_server *, int id, const void * buffer, int sz);

// socket 事件循环
int socket_server_poll(struct socket_server *, struct socket_message *result, int *more);

```




