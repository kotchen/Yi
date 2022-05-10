#ifndef __YI_RING_BUFFER_H__
#define __YI_RING_BUFFER_H__
#include <cstdint>
namespace yi
{
    template <typename T>
    class RingBuffer
    {
    private:
        T *_buffer;
        uint32_t _read_pos;
        uint32_t _write_pos;
        uint32_t _capacity;
        uint32_t _size;
        /**
         * @brief 判断一个无符号整型是不是2的幂
         *        从2进制的角度来看，如果一个数是2的幂，那么它就只有一位是1，其余都是0
         *         这样我们让它-1，然就得到了一个后置位全是1的数。
         *          比如：4-> 0100，-1之后变成了0011，然后0100 & 0011，结果就是0
         *
         * @param n
         * @return true
         * @return false
         */
        bool _Is_Power_Of_2(uint32_t n)
        {
            if (n < 2)
                return false;
            return !(n & (n - 1));
        }
        /**
         * @brief 将一个不是二的幂的数转换成二的幂
         *          这里就是一个循环，算出n有多少位，然后将n左移这么多位，就可以得到一个二的幂
         *
         * @param n
         */
        void _Round_Up_Power_Of_2(uint32_t &n)
        {
            if (n < 2)
            {
                n = 2;
                return;
            }
            if (!_Is_Power_Of_2(n))
            {
                uint32_t count = 0;
                for (; n != 0;)
                {
                    n >>= 1;
                    count++;
                }
                n = 1U << count;
            }
        }

    public:
        RingBuffer(uint32_t capacity)
            : _read_pos(0), _write_pos(0), _size(0), _capacity(capacity)
        {
            _Round_Up_Power_Of_2(_capacity);
            _buffer = new T[_capacity];
        }
        ~RingBuffer()
        {
            delete[] _buffer;
        }
        bool IsEmpty()
        {
            return _read_pos == _write_pos;
        }
        bool IsFull()
        {
            return _capacity == _write_pos - _read_pos;
        }
        uint32_t Size()
        {
            return _write_pos - _read_pos;
        }
        uint32_t Remian()
        {
            return _capacity - Size();
        }

        uint32_t Readn(T *buffer, uint32_t n)
        {
            if (IsEmpty())
                return 0;
            uint32_t real_read_nums = std::min(n, Size());
            uint32_t first_round_read = std::min(real_read_nums, _capacity - (_read_pos & (_capacity - 1)));
            for (uint32_t i = 0; i < first_round_read; i++)
            {
                buffer[i] = _buffer[_read_pos++ & (_capacity - 1)];
            }

            for (uint32_t i = 0; i < real_read_nums - first_round_read; i++)
            {
                buffer[first_round_read + i] = _buffer[_read_pos++ & (_capacity - 1)];
            }
            return real_read_nums;
        }

        uint32_t Writen(T *buffer, uint32_t n)
        {
            if (n > Remian())
                return 0;
            uint32_t first_round_write = std::min(n, _capacity - (_write_pos & (_capacity - 1)));
            for (uint32_t i = 0; i < first_round_write; i++)
            {
                _buffer[_write_pos++ & (_capacity - 1)] = buffer[i];
            }

            for (uint32_t i = 0; i < n - first_round_write; i++)
            {
                _buffer[_write_pos++ & (_capacity - 1)] = buffer[first_round_write + i];
            }
            return n;
        }
    };

} // namespace yi

#endif