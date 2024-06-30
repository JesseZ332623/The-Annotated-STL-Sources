#ifndef __MY_DEQUE_H_
#define __MY_DEQUE_H_

#include "../../simple_allocator/simpleAlloc.h"
#include "./deque_iterator.h"

#include <algorithm>
#include <memory>
#include <initializer_list>

/**
 * @brief STL 双端队列的实现，
 *        双端队列（deque）是可以在容器的两头以常数复杂度 O(1) 进行出队和入队的容器。
 *        
 * @brief - deque 内部采用了一个 `Type **` 类型的指针数组 `map` 作为中控，
 *          数组中的每一个元素都是指向了堆上一片连续内存空间（称作缓冲区）的指针。
 * 
 * @brief - SGI STL 的实现允许用户指定缓冲区大小（但是在现代 STL 版本中，这个模板参数被屏蔽），
 *          默认值为 0 意味着使用 512 字节大小的缓冲区。
 * 
 * @tparam Type         双端队列内数据类型
 * @tparam BufferSize   缓冲区大小
 * @tparam Alloc        分配器类型，默认为 `std::allocator<Type>`
*/
template <typename Type, std::size_t BufferSize = 0, typename Alloc = std::allocator<Type>>
class My_Deque
{
    public:
        using value_type = Type;
        using pointer    = value_type *;

        using size_type       = std::size_t;
        using difference_type = std::ptrdiff_t;

    protected:
        using map_pointer = pointer *;

        /**
         * map 是一个 `Type **` 类型的指针数组，
         * 数组中的每一个元素都是指向了堆上一片连续内存空间（称作缓冲区）的指针。
        */
        map_pointer map;

        size_type   map_size;  // map 内可以容纳多少指针。
};

#endif // __MY_DEQUE_H_