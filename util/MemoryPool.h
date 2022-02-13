#ifndef __YI_MEMORY_POOL__
#define __YI_MEMORY_POOL__
#include <map>
#include <unordered_map>
namespace yi
{
    class MemoryPool
    {
    private:
        void *_memptr;
        std::multimap<size_t, void *> _free_list;
        std::unordered_map<void *, size_t> _used_list;
        // std::map<void*, size_t> _used_list;

    public:
        MemoryPool(size_t capacity)
        {
            _memptr = ::operator new(capacity);
            _free_list.insert({capacity, _memptr});
        }

        ~MemoryPool()
        {
            ::operator delete(_memptr);
        }
        // template <typename ValueType, typename... Args>
        // ValueType *alloc(Args&&... args)
        // {
        //     auto size = sizeof(ValueType);
        //     auto ite = _free_list.begin();
        //     for (; ite != _free_list.end(); ite++)
        //     {
        //         if (size < ite->first)
        //             break;
        //     }

        //     if (ite == _free_list.end())
        //         return nullptr;
        //     else
        //     {
        //         auto [best_fit_size, ptr] = *ite;
        //         _free_list.erase(ite);
        //         _free_list[best_fit_size - size] = ((uint8_t *)ptr + size);
        //         _used_list[ptr] = size;
        //         ::new (ptr) ValueType(std::forward<Args>(args)...);
        //         return (ValueType *)ptr;
        //     }
        // }

        void *alloc(size_t size)
        {
            auto ite = _free_list.begin();
            for (; ite != _free_list.end(); ite++)
            {
                if (size <= ite->first)
                    break;
            }

            if (ite == _free_list.end())
                return nullptr;
            else
            {
                auto [best_fit_size, ptr] = *ite;
                _free_list.erase(ite);
                // _free_list[best_fit_size - size] = ((uint8_t *)ptr + size);
                _free_list.insert({{best_fit_size - size, ((uint8_t *)ptr + size)}});
                _used_list[ptr] = size;
                // ::new (ptr) ValueType(std::forward<Args>(args)...);
                return ptr;
            }
        }

        // template<typename ValueType>
        // void free(ValueType *ptr)
        // {
        //     auto size = _used_list[ptr];
        //     if (size == 0)
        //         return;
        //     auto ite = _free_list.begin();
        //     for (; ite != _free_list.end(); ite++)
        //     {
        //         if (ite->second == ((uint8_t *)ptr + size))
        //             break;
        //     }
        //     if (ite == _free_list.end())
        //         _free_list[size] = ptr;
        //     else
        //     {
        //         auto next_size = ite->first;
        //         _free_list.erase(ite);
        //         _free_list[size + next_size] = ptr;
        //     }
        //     _used_list.erase(ptr);
        // }

        // void free(void *ptr)
        // {
        //     auto size = _used_list[ptr];
        //     if (size == 0)
        //         return;
        //     auto ite = _free_list.begin();
        //     for (; ite != _free_list.end(); ite++)
        //     {
        //         if (ite->second == ((uint8_t *)ptr + size))
        //             break;
        //     }
        //     if (ite == _free_list.end())
        //         _free_list.insert({size, ptr});
        //     else
        //     {
        //         auto next_size = ite->first;
        //         _free_list.erase(ite);
        //         _free_list.insert({size+next_size, ptr});
        //     }
        //     _used_list.erase(ptr);
        // }

        void free(void *ptr)
        {
            auto size = _used_list[ptr];
            if (size == 0)
                return;
            _free_list.insert({size, ptr});
            _used_list.erase(ptr);
        }
        // O(n^2)
        bool coalesce()
        {
            // _free_list
            auto ite = _free_list.begin();
        }
    };
}

#endif