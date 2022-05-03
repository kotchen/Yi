这里要熟悉client的流程

1 产生一个sock fd 填写sockaddr_in
对于一个client，它有什么属性？
    sockfd
    目标的addr
还有就是一个客户可以并发的向服务器发起请求，设置回调函数
这就说明，client是有多个线程的，随机选择一个线程来执行我们的函数调用？
这样connect and call_function都是交给线程去做
感觉很有道理，但是好像不是

sock用于读写
net用于连接，设置回调函数，比如
connect call back
write call back
read call back
disconnect call back

net中多个线程，

2 connect
3 write/read