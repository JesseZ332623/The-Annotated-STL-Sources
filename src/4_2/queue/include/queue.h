#ifndef __QUEUE_H__
#define __QUEUE_H__

#include <deque>
#include <cctype>

/**
 * @brief 单向队列队列是一个先进先出结构的数据类型，
 * 在 STL 的实现中，它是基于 std::list 或者 std::deque（默认） 的。
 */

template <typename Type, typename Sequence = std::deque<Type>>
class queue
{
    /**
     * @brief 判断两个 queue 是否完全相等
     */
    friend bool operator==(const queue & __a, const queue & __b) { return __a.contain == __b.contain; }

    /**
     * @brief 判断 queue a 是否小于 queue b
     */
    friend bool operator<(const queue & __a, const queue & __b)  { return __a.contain < __b.contain; }

    public:
        typedef Type                value_type;
        typedef Type &              reference;
        typedef const Type &        const_reference;
        typedef Type *              pointer;
        typedef const Type *        const_pointer;

        typedef std::size_t         size_type;
        typedef std::ptrdiff_t      difference_type;

    protected:
        Sequence contain;       // queue 的底层容器
    
    public:

        bool empty(void) const noexcept { return (this->contain.size() == 0); }

        size_type size(void) const noexcept { return this->contain.size(); }

        reference front(void) noexcept { return this->contain.front(); }
        const_reference front(void) const noexcept { return this->contain.front(); }

        reference back(void) noexcept { return this->contain.back(); }
        const_reference back(void) const noexcept { return this->contain.back(); }

        void push(value_type & __value) { this->contain.push_front(__value); }
        void push(value_type && __value) { 
            this->contain.push_front(std::move(__value));
        }

        void pop(void) { this->contain.pop_back(); }
};

#endif // __QUEUE_H__