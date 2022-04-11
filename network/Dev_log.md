这里实现net

net直接理解就是我们的网络
连接，相应都是使用这个net

如果没有多线程，那么net的设计就是自己调用socket的API来实现功能

但是现在有了多线程，那么处理客户端发来的请求这个事情就交给其他线程去做就好了，主线程就处理连接？再加上异步的理解，就更加糊涂了


前人的逻辑是
net中有多线程，其实就是一个vector中含有dispatcher，dispatcher的数量等于CPU核心数量

初始化就是初始化这个vector，向里面push dispatcher。 具体如何初始化dispatcher先按下不表？

初始化多线程之后，就是设置call back， 
服务器回遇到什么样的call? 
1 connnect  disconnect
2 write
connect call back  这个是比较好理解的 需要将function info map中的数据封装发送出去


异步，我们这里使用的是模拟proactor模式的异步

靠的就是epoll_wait
主线程