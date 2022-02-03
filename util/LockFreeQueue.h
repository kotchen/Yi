#ifndef __YI_LOCKFREEQUEUE__
#define __YI_LCOKFREEQUEUE__

#include <atomic>
#include <thread>
#include <vector>
#include <cstring>
#include <string>
#include <iostream>
#include <deque>

namespace yi
{

    template <typename ValueType>
    class LockFreeQueue
    {
    private:
        static constexpr size_t CACHELINE_ALIGNMENT = 64 - sizeof(size_t);
        struct Node
        {
            ValueType _data;
            bool _used;
        };

    public:
        size_t _capacity;
        Node *_node_head;
        std::atomic<size_t> _head;
        char _cacheline_alignment[CACHELINE_ALIGNMENT];
        std::atomic<size_t> _tail;
        LockFreeQueue(size_t queue_size)
            : _capacity(queue_size),
              _head(0),
              _tail(0),
              _cacheline_alignment{0}
        {
            _node_head = ::new Node[_capacity];
            for (int i = 0; i < _capacity; i++)
                _node_head[i]._used = false;
        }
        ~LockFreeQueue()
        {
            delete[] _node_head;
        }

        bool Push(const ValueType &data)
        {
            while (true)
            {
                auto tail = _tail.load();
                if (_node_head[tail]._used)
                    return false;
                if (_tail.compare_exchange_weak(tail, (tail + 1) % _capacity))
                {
                    _node_head[tail]._used = true;
                    _node_head[tail]._data = data;
                    return true;
                }
            }
        }

        bool Pop(ValueType &ret)
        {
            while (true)
            {
                auto head = _head.load();
                if (!_node_head[head]._used)
                    return false;
                if (_head.compare_exchange_weak(head, (head + 1) % _capacity))
                {
                    _node_head[head]._used = false;
                    ret = _node_head[head]._data;
                    return true;
                }
            }
        }
    };

};
#endif