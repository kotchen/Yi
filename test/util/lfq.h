// write a lock-free queue
template<class T>
class LockFreeQueue
{
public:
    LockFreeQueue()
    {
        head = tail = 0;
    }

    bool Push(const T& data)
    {
        Node* node = new Node;
        node->data = data;
        node->next = nullptr;

        for (;;)
        {
            Node* oldHead = head.load();
            node->next = oldHead;

            if (head.compare_exchange_weak(oldHead, node))
                return true;
        }
    }

    bool Pop(T& data)
    {
        for (;;)
        {
            Node* oldTail = tail.load();
            Node* newTail = oldTail->next;

            if (oldTail == tail.load())
            {
                if (newTail == nullptr)
                    return false;

                tail.compare_exchange_weak(oldTail, newTail);
            }
        }
    }

    bool Empty()
    {
        return tail.load() == head.load();
    }

private:
    struct Node
    {
        T data;
        Node* next;
    };

    std::atomic<Node*> head;
    std::atomic<Node*> tail;
};

