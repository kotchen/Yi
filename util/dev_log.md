# 内存池开发日志

这里自己写一个内存池，记录遇到的问题和解决的方法

## 最简单的array的实现
在这里直接用指针指向内存，在alloc and free的时候都会遇到问题
1 alloc的时候，后面不够但是前面有，怎么解决
2 free的时候，前面free了，下次alloc怎么用上
有了这两个问题，我们就不能简单的使用一块什么标记都没有的内存
## 使用freelist
所以，我们写一个Node节点
```
class Node
{
    size_t _size;
    void*  _memptr;
};
```
还需要一个freelist
这是一个multimap key 是 内存块的大小 value是地址的起点
每次alloc内存，需要先找到一块合适的内存，然后分裂这块内存
因为我们的size是key，所以必须删除一次节点，然后再插入一次节点

free的时候需要合并内存块，我们只给内存池一个指针，内存池如何知道这块内存有多大?
这里这个数据结构可以使用hashmap，这样可以通过指针查到size
之后就需要遍历freelist
## freelist and used list
两个都是红黑树
合并可以选择在内存不够的时候再合并
我们这里使用的alloc的策略，只要内存足够，alloc的时间复杂度就是O(1)