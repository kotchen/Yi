一个server要做的东西

1 socket() bind()
2 accept()

上面这些操作结合 io复用 epoll
就可以一个线程/进程来 处理多个客户端发来的请求。
但是其实，我们可以将客户端发来的数据（函数命+参数）
存入队列   谁把数据存入队列? 多线程，从这个队列中拿数据
一旦队列非空，xx 就可以从队列中获取要执行的函数和参数，然后调用函数，得到返回结果之后，发送package回客户端

3 read()/write()