在这里我们有两个序列化工具可以选择

protobuf
rapidjson

一开始想使用protobuf，但是感觉在定义函数参数的时候，因为不能随便写函数参数的顺序，可能导致开发量巨大
但是只要约定好开发所开发的函数的参数的类型顺序，比如 先写数值类型，再写字符串类型，再写自定义类型等，这样是可以使用protobuf的


在deserialize的时候
我们拿到函数名称 和 参数，然后就要调用函数了
我们这里是将函数名和参数添加 到队列中，然后线程去检测队列是否是空的，然后调用函数，问题是谁调用函数？


现在出了终极问题，在接收方，在接收参数的时候，如何存储，存储不同数据类型的容器

ok,std::vector<std::any>
这个东西无敌了。

现在基本的逻辑写好了，就是一些细节问题，比如string and char*之间的类型不匹配

这里遇到了什么问题？
就是比如一个函数add 服务方就是需要两个参数，这个消息，在客户端连接直接，传输给客户端，客户端也会有一个相应的服务函数表。

所以，
客户端成员：
1 function_callback_map  联系函数名和回调函数
2 function_info_map      这个是函数名和参数

服务端成员：
1 function_call_map      函数命 和调用的函数
2 function_info_map      这个是 函数名和参数

报文的格式 json str
1 package type
    - function info 服务方函数的信息，存储在双方的function_info_map中，由服务器发出的包，在客户端第一次连接的时候。这里好像不能热更新服务。
    - function call 函数调用，客户端发起的请求
2 request parameters
    - function info 如果是这个类型的包，参数就是一个字符串，比如i ii，表示返回值是一个int 参数是两个int
        数值   ： i ->int  d -> double
        字符串 ： c ->char s -> std::string
        布尔   :  b ->bool
    - function call 函数名 + 参数
        发送这个包的时候，先查 function_info_map，来构造json
    

返回错误：函数参数错误