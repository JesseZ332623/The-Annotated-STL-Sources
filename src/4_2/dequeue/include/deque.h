#ifndef __MY_DEQUE_H_
#define __MY_DEQUE_H_

#include "../../simple_allocator/simpleAlloc.h"
#include "./deque_iterator.h"

#include <algorithm>
#include <memory>
#include <initializer_list>

/**
 * @brief STL 双端队列的实现，
 *        双端队列（deque）是可以在容器的头尾以常数级复杂度 O(1) 进行出队和入队的容器。
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
        using reference  = value_type &;

        using size_type       = std::size_t;
        using difference_type = std::ptrdiff_t;
    
    public:
        using iterator = Deque_Iterator<Type, Type &, Type *, BufferSize>;

    protected:
        using map_pointer    = pointer *;

        /**
         * deque 专属的空间配置器，每次配置一个 Type 类型的数据。
        */
        using data_allocator = Simple_Alloc<value_type, Alloc>;

        /**
         * deque 专属的空间配置器，每次配置一个 Type * 类型的指针。
        */
        using map_allocator  = Simple_Alloc<pointer, Alloc>;

        /**
         * map 是一个 `Type **` 类型的指针数组，
         * 数组中的每一个元素都是指向了堆上一片连续内存空间（称作缓冲区）的指针。
        */
        map_pointer map;

        size_type   map_size;  // map 数组内可以容纳多少指针。

        iterator start;        // map 的第一个节点的迭代器
        iterator finish;       // map 的最后一个节点的迭代器

        enum { MININUM_NODES = 8 };

        /**
         * @brief 返回最少的中控节点数（默认是 8 个 nodes）。
        */
        size_type initial_map_size() { return MININUM_NODES; }

        /**
         * @brief 为 map 内的单个现用节点配置缓冲区。
        */
        pointer allocate_node() 
        {  
            return data_allocator::allocate(iterator::getBufferSize());
        }

        /**
         * @brief 销毁单个 map 内节点所指的缓冲区。
        */
        void deallocate_node(pointer __node) 
        {
            data_allocator::deallocate(__node, iterator::getBufferSize());
        }

        /**
         * @brief 辅助函数，创建 map 中控和中控内的节点。
         * 
         * @param __numElemnts 需要创建的元素个数
        */
        void create_map_and_nodes(size_type __numElemnts);

        /**
         * @brief 辅助函数，负责产生和安排好 deque 的结构，并将元素的初值设定妥当。
         * 
         * @param __n       需要创建的元素个数
         * @param __value   给每个元素设定的初值
        */
        void fill_initialize(size_type __n, const value_type & __value);

        /**
         * 
        */
        void push_back_aux(const value_type & __value);
    
    public:
        My_Deque(size_type __n, const value_type & __value) : start(), finish(), map(nullptr), map_size(0ULL)
        {
            this->fill_initialize(__n, __value);
        }

        /**
         * @brief 获取 map 的第一个节点的迭代器
        */
        iterator begin() { return this->start; }

        /**
         * @brief 获取 map 最后一个节点的迭代器
        */
        iterator end()   { return this->finish; }

        /**
         * @brief 访问 deque 中第 __n 个元素的值的引用。
        */
        reference operator[](size_type __n) { 

            /**
             * 调用 reference operator[](difference_type __n);
            */
            return this->start[difference_type(__n)];
        }

        /**
         * @brief 取 map 中第一个节点所指向的缓冲区现行元素的值。
         *        调用 reference operator*() const;
        */
        reference front() { return *this->start; }

        /**
         * @brief 取 map 中最后节点所指向的缓冲区现行元素的值。
        */
        reference back()  
        { 
            iterator tempIter = this->finish;
            
            /**
             * 调用 self operator--(int); 和 reference operator*() const;
            */
            return *(--tempIter);
        }

        /**
         * @brief 求 map 中的当前节点数。
        */
        size_type size() const { return (this->finish - this->start); }

        /**
         * @brief 求 map 可以放的最大节点数?
        */
        size_type maxSize() const { return ULLONG_MAX; }

        /**
         * @brief 容器是否为空。
        */
        bool empty() const { return (this->start == this->finish); }
};

template <typename Type, std::size_t BufferSize, typename Alloc>
void My_Deque<Type, BufferSize, Alloc>::create_map_and_nodes(size_type __numElemnts)
{
    /**
     * 需要的节点数 = 需要创建的元素数 / 缓冲区可容纳的元素数 + 1
     * 若刚好整除，会多配一个节点。
    */
    size_type nodesCount = __numElemnts / iterator::getBufferSize() + 1;

    /**
     * 中控 map 需要管理几个节点，最少为 8 个，最多为 需要的节点数 + 2
    */
    this->map_size = std::max(this->initial_map_size(), nodesCount + 2);

    /**
     * 在 map 上配置 map_size 个节点。
    */
    this->map = map_allocator::allocate(this->map_size);

    map_pointer nStart  = this->map + (map_size - nodesCount) / 2;
    map_pointer nFinish = nStart + nodesCount - 1;
    map_pointer nCurrent;

    try
    {
        for (nCurrent = nStart; nCurrent < nFinish; ++nCurrent)
        {
            *nCurrent = this->allocate_node();
        }
    }
    catch(...)
    {
        map_pointer tempStart;
        for (tempStart = nStart; tempStart < nCurrent; ++tempStart)
        {
            this->deallocate_node(*tempStart);
        }

        throw;
    }

    this->start.setNode(nStart);
    this->finish.setNode(nFinish);
    start.current  = start.first;
    finish.current = finish.first + __numElemnts % iterator::getBufferSize();
}

template <typename Type, std::size_t BufferSize, typename Alloc>
void My_Deque<Type, BufferSize, Alloc>::fill_initialize(size_type __n, const value_type & __value)
{
    this->create_map_and_nodes(__n);
    
    map_pointer current;

    try
    {
        for (current = this->start.node; current < this->finish.node; ++current)
        {
            std::uninitialized_fill(*current, *current + iterator::getBufferSize(), __value);
        }

        /**
         * 最后一个节点的构建稍有不同（尾端可能有备用空间，不必设初值）。
        */
        std::uninitialized_fill(this->finish.first, this->finish.current, __value);
    }
    catch(...)
    {
        map_pointer tempStart;

        for (tempStart = this->start.node; tempStart < current; ++tempStart)
        {
            std::destroy(*tempStart, *tempStart + iterator::getBufferSize());
            this->deallocate_node(*tempStart);
        }

        throw;
    }
}

#endif // __MY_DEQUE_H_