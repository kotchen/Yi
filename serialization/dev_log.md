﻿在这里我们有两个序列化工具可以选择

protobuf
rapidjson

一开始想使用protobuf，但是感觉在定义函数参数的时候，因为不能随便写函数参数的顺序，可能导致开发量巨大
但是只要约定好开发所开发的函数的参数的类型顺序，比如 先写数值类型，再写字符串类型，再写自定义类型等，这样是可以使用protobuf的