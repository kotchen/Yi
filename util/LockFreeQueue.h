#ifndef __LCOK_FREE_QUEUE__
#define __LCOK_FREE_QUEUE__
#include <queue>
#include <atomic>
#include <iostream>
#include <condition_variable>
#include "TLSF.hpp"

template<typename ValueType>
struct Node;

template<typename ValueType>
struct Pointer
{
    Node<ValueType>* ptr;
    uint32_t count;
    bool operator== (const Pointer<ValueType>& rhs)
    {
        return (ptr == rhs.ptr) && (count == rhs.count);
    }
};

template<typename ValueType>
struct Node
{
    ValueType _data;
    std::atomic<Pointer<ValueType>> _next;
    Node () : _data(ValueType()), _next({nullptr, 0})
    {
    }
    Node (ValueType data) : _data(data), _next({nullptr, 0})
    {
    }
    Node (const Node<ValueType>&) = delete;
    Node& operator= (const Node<ValueType>&) = delete;
};
template<typename ValueType>
struct PopReturn
{
    ValueType _ret;
    bool      _state;
};

template<typename ValueType>
class LockFreeQueue
{
private:
    /* data */
    std::atomic<Pointer<ValueType>> _head;
    std::atomic<Pointer<ValueType>> _tail;

    // std::shared_ptr<TLSF>           _tlsf;
    void destruct_helper(Pointer<ValueType> pointer)
    {
        if (pointer.ptr)
        {
            auto next = pointer.ptr->_next.load(std::memory_order_relaxed);
            delete  pointer.ptr; 
            destruct_helper(next);
        }
    }
    void print_helper(Pointer<ValueType> pointer)
    {
        if (pointer.ptr)
        {
            std::cout << pointer.ptr->_data << ' ';
            print_helper(pointer.ptr->_next);
        }

    }
public:
    // LockFreeQueue(std::shared_ptr<TLSF> tlsf) : _tlsf(tlsf)
    LockFreeQueue()
    {
        auto node = new Node<ValueType> ();
        _head = {node, 0};
        _tail = {node, 0};
    }
    ~LockFreeQueue()
    {
        destruct_helper(_head.load(std::memory_order_relaxed));
    }

    void Push(const ValueType& data)
    {
        Node<ValueType>* node = new Node<ValueType> (data);
        Pointer<ValueType> tail;
        Pointer<ValueType> next;
        while (true)
        {
            tail = _tail.load();
            next = tail.ptr->_next;
            if (tail == _tail.load(std::memory_order_relaxed))
            {
                if (!next.ptr)
                {
                    if (tail.ptr->_next.compare_exchange_weak(next, {node, next.count+1}, std::memory_order_relaxed))
                        break;
                }
                else    
                    _tail.compare_exchange_weak(tail, {next.ptr, tail.count+1}, std::memory_order_relaxed);
            }

        }
        _tail.compare_exchange_weak(tail, {node, tail.count+1}, std::memory_order_relaxed);
    }

    PopReturn<ValueType> Pop()
    {
        PopReturn<ValueType> ret;
        Pointer<ValueType> head;
        Pointer<ValueType> tail;
        while (true)
        {
            head = _head.load(std::memory_order_relaxed);
            tail = _tail.load();
            // auto next = head->
            Pointer<ValueType> next = {head.ptr->_next.load(std::memory_order_relaxed).ptr, 0};
            if (head == _head.load(std::memory_order_relaxed))
            {
                if (head.ptr == tail.ptr)
                {
                    if (!next.ptr)
                        // empty
                    {
                        ret._state = false;
                        return ret;
                    }
                    else
                        _tail.compare_exchange_weak(tail, {next.ptr, tail.count+1}, std::memory_order_relaxed);
                }
                else
                {
                    ret._ret = next.ptr->_data;
                    if (_head.compare_exchange_weak(head, {next.ptr, head.count+1}, std::memory_order_relaxed))
                        break;
                }
            }
        }
        delete head.ptr;
        ret._state = true;
        return ret;
    }

    void print()
    {
        Pointer<ValueType> pointer = _head.load(std::memory_order_relaxed);
        print_helper(pointer.ptr->_next);
    }
};




#endif