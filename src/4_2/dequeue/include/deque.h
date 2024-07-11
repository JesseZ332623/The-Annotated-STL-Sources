#ifndef __MY_DEQUE_H_
#define __MY_DEQUE_H_

#include "../../simple_allocator/simpleAlloc.h"
#include "./deque_iterator.h"

#include <memory>
#include <exception>
#include <initializer_list>

/**
 * @brief STL 双端队列的实现，
 *        双端队列（deque）是可以在容器的头尾以常数级复杂度 O(1) 进行出队和入队的容器。
 *        
 * @brief - deque 内部采用了一个 `Type **` 类型的指针数组 `map` 作为中控，
 *          数组中的每一个元素都是指向了堆上一片连续内存空间（称作缓冲区）的指针。
 * 
 * @brief - SGI STL 的实现允许用户指定缓冲区大小（但是在现代 STL 版本中，这个模板参数被屏蔽），
 *          默认值为 0 意味着使用 512 个 Type 类型大小的缓冲区。
 * 
 * @tparam Type         双端队列内数据类型
 * @tparam BufferSize   缓冲区大小
 * @tparam Alloc        分配器类型，默认为 `std::allocator<Type>`
*/
template <typename Type, std::size_t BufferSize = 0, typename Alloc = std::allocator<Type>>
class My_Deque
{
    public:
        typedef Type                    value_type;
        typedef value_type *            pointer;
        typedef value_type &            reference;
        typedef const value_type &      const_reference;

        typedef std::size_t             size_type;
        typedef std::ptrdiff_t          difference_type;
    
    public:
        typedef Deque_Iterator<Type, Type &, Type *, BufferSize>        iterator;
        typedef const Deque_Iterator<Type, Type &, Type *, BufferSize>  const_iterator;

    protected:
        typedef pointer *       map_pointer;

        /**
         * deque 专属的空间配置器，每次配置一个 Type 类型的数据。
        */
        typedef Simple_Alloc<value_type, Alloc>  data_allocator;

        /**
         * deque 专属的空间配置器，每次配置一个 Type * 类型的指针。
        */
        typedef Simple_Alloc<pointer, Alloc>      map_allocator;

        iterator start;        // map 的第一个节点的迭代器
        iterator finish;       // map 的最后一个节点的迭代器

        /**
         * map 是一个 `Type **` 类型的指针数组，
         * 数组中的每一个元素都是指向了堆上一片连续内存空间（称作缓冲区）的指针。
        */
        map_pointer map;

        size_type   map_size;  // map 数组内可以容纳多少指针。

        enum { MININUM_NODES = 8 };

        /**
         * @brief 返回最少的中控节点数（默认是 8 个 nodes）。
        */
        size_type initial_map_size() { return MININUM_NODES; }

        /**
         * @brief 为 `map` 内的单个现用节点配置缓冲区。
        */
        pointer allocate_node() 
        {  
            return data_allocator::allocate(iterator::getBufferSize());
        }

        /**
         * @brief 销毁单个 `map` 内节点所指的缓冲区。
        */
        void deallocate_node(pointer __node) 
        {
            data_allocator::deallocate(__node, iterator::getBufferSize());
        }

        /**
         * @brief 辅助函数，创建 `map` 中控和中控内的节点。
         * 
         * @param __numElemnts 需要创建的元素个数
        */
        void create_map_and_nodes(size_type __numElements);

        /**
         * @brief 辅助函数，负责产生和安排好 `deque` 的结构，并将元素的初值设定妥当。
         * 
         * @param __n       需要创建的元素个数
         * @param __value   给每个元素设定的初值
        */
        void fill_initialize(size_type __n, const value_type & __value);

        /**
         * @brief 辅助函数，负责产生和安排好 `deque` 的结构，
         *        并将元素的初值从初始化列表拷贝到容器。
        */
        template <typename ForwardIterator>
        void range_initialize(ForwardIterator __first, ForwardIterator __last);

        /**
         * @brief 辅助函数，
         *        在 `push_back()` 操作遭遇当前缓冲区容量不足的情况时，
         *        需要移动到下一个缓冲区甚至重新分配一个 `map`，再将值构建到缓冲区中。
        */
        void push_back_aux(const value_type & __value);
        
        /**
         * @brief 辅助函数，
         *        在 `push_front()` 操作遭遇当前缓冲区容量不足的情况时，
         *        需要移动到下一个缓冲区甚至重新分配一个 `map`，再将值构建到缓冲区中。
        */
        void push_front_aux(const value_type & __value);

        /**
         * @brief 辅助函数，
         *        在 `pop_back()` 操作中遇到当前 `finish` 指针所指缓冲区内仅有一个元素的时候，
         *        调用该函数释放这个缓冲区，并调整 `finish` 指针的位置。
        */
        void pop_back_aux(void);

        /**
         * @brief 辅助函数，
         *        在 `pop_front()` 操作中遇到当前 `start` 指针所指缓冲区内没有两个或更多元素时，
         *        调用该函数释放这个缓冲区，并调整 `start` 指针的位置。
        */
        void pop_front_aux(void);

        /**
         * @brief 辅助函数，
         *        处理 `insert()` 函数指向非头尾插入的情况。
        */
        iterator insert_aux(iterator __pos, const value_type & __value);

        /**
         * @brief 辅助函数，
         *        在指针数组 map 装满的情况下，重新配置 map。
         * 
         * @param __nodesToAdd 需要插入多少节点
         * @param __addAtFront 是否为头部插入
        */
        void reallocate_map(size_type __nodesToAdd, bool __addAtFront);

        /**
         * @brief 辅助函数，从 map 的尾部添加节点。
        */
        void reserve_map_at_back(size_type __nodesToAdd = 1)
        {
            if (__nodesToAdd + 1 > this->map_size - (this->finish.node - this->map))
            {
                this->reallocate_map(__nodesToAdd, false);
            }
        }   

        /**
         * @brief 辅助函数，从 map 的头部添加节点。
        */
        void reserve_map_at_front(size_type __nodesToAdd = 1)
        {
            if (__nodesToAdd > this->start.node - this->map)
            {
                this->reallocate_map(__nodesToAdd, true);
            }
        }

        /**
         * @brief 辅助函数，供 at() 进行边界检查。
        */
        void range_check(size_type __n) const
        {
            if (__n >= this->size())
            {
                std::__throw_out_of_range_fmt(__N("deque::_M_range_check: __n "
				       "(which is %zu)>= this->size() "
				       "(which is %zu)"),
				   __n, this->size());
            }
        }

        static void swap(My_Deque & __a, My_Deque & __b) noexcept
        {
            using std::swap;

            swap(__a.start, __b.start);
            swap(__a.finish, __b.finish);
            swap(__a.map, __b.map);
            swap(__a.map_size, __b.map_size);
        }
    
    public:
        /**
         * @brief 默认构造函数，在 map 中间处分配 1 个缓冲区，
         *        并使用容器类型的默认构造函数来填充缓冲区的值。
        */
        My_Deque() : start(), finish(), map(nullptr), map_size(0ULL) { this->fill_initialize(0, Type()); }

        /**
         * @brief 构造函数，在 map 中间分配 n 个元素的内存，
         *        并全部构造值为 __value。
        */
        My_Deque(size_type __n, const value_type & __value) : My_Deque()
        {
            this->fill_initialize(__n, __value);
        }

        /**
         * @brief 构造函数，在 map 中间分配 __initList.size() 个元素的内存，
         *        并把初始化列表内的数据构造进容器。
        */
        My_Deque(const std::initializer_list<value_type> __initList) : My_Deque()
        {
            this->range_initialize(__initList.begin(), __initList.end());
        }

        My_Deque(const My_Deque & __deque) : My_Deque()
        {
            this->range_initialize(__deque.begin(), __deque.end());
        }

        My_Deque(My_Deque && __deque) : My_Deque()
        {
            if (this != &__deque) { this->swap(*this, __deque); }
        }

        /**
         * @brief 析构函数，析构并释放所有缓冲区，
         *        最后释放 map。
        */
        ~My_Deque() 
        {
            if (!this->empty())
            {
                this->clear();
                this->deallocate_node(this->start.first);

                delete[] this->map;
            }
        }

        /**
         * @brief 获取 map 的第一个节点的迭代器
        */
        iterator begin() { return this->start; }
        iterator begin() const { return this->start; }
        const_iterator cbegin() const { return this->start; }

        /**
         * @brief 获取 map 最后一个节点的迭代器
        */
        iterator end() { return this->finish; }
        iterator end() const { return this->finish; }
        const_iterator cend() const { return this->finish; }

        /**
         * @brief 访问 deque 中第 __n 个元素的值的引用。
        */
        reference operator[](size_type __n) 
        { 
            /**
             * 调用 reference operator[](difference_type __n);
            */
            return this->start[difference_type(__n)];
        }

        /**
         * @brief 访问 deque 中第 __n 个元素的值的只读引用。
        */
        const_reference operator[](size_type __n) const
        {
            return this->start[difference_type(__n)];
        }

        /**
         * @brief 访问 deque 中第 
         *        __n 个元素的值的引用，但进行边界检查。
        */
        reference at(size_type __n)
        {
            this->range_check(__n);
            return (*this)[__n];
        }

        /**
         * @brief 访问 deque 中第 
         *        __n 个元素的值的只读引用，但进行边界检查。
        */
        const_reference at(size_type __n) const
        {
            this->range_check(__n);
            return (*this)[__n];
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
             * 调用 self operator--(int); 
            */
            --tempIter;
            
            /**
             * 调用 reference operator*() const;
            */
            return *tempIter;
        }

        /**
         * @brief 求 deque 中当前有多少元素。
        */
        size_type size() const { return (this->finish - this->start); }

        /**
         * @brief 求 map 可以存放的最大元素数
        */
        size_type maxSize() const { return SIZE_MAX; }

        /**
         * @brief 容器是否为空。
        */
        bool empty() const { return (this->finish == this->start); }

        /**
         * @brief 往容器末尾添加元素。
        */
        void push_back(const value_type & __value)
        {
            /**
             * 当 finish 迭代器所指缓冲区还有空位之时，
             * 直接在当前位置构造并调整 current 指针的位置即可。
            */
            if (this->finish.current != this->finish.last - 1)
            {
                std::_Construct(this->finish.current, __value);
                ++this->finish.current;
            }
            else
            { 
                /**
                 * 否则就要调用 push_back_aux() 重新在 
                 * *(finish.node + 1) 处配置一个新的缓冲区来存放新数据。
                */
                this->push_back_aux(__value); 
            }
        }

        /**
         * @brief 往容器开头添加元素
        */
        void push_front(const value_type & __value)
        {
            /**
             * 当 start 迭代器所指缓冲区还有空位之时，
             * 直接在当前位置构造并调整 current 指针的位置即可
            */
            if (this->start.current != this->start.first)
            {
                std::_Construct(this->start.current - 1, __value);
                --this->start.current;
            }
            else 
            { 
                /**
                 * 否则就要调用 push_back_aux() 重新在 
                 * *(start.node - 1) 处配置一个新的缓冲区来存放新数据。
                */
                this->push_front_aux(__value); 
            }
        }

        /**
         * @brief 删除容器开头的第一个元素。
        */
        void pop_front(void)
        {
            if (this->empty()) { return; }

            if (this->start.current != this->start.last - 1)
            {
                std::destroy_at(this->start.current);
                ++this->start.current;
            }
            else { this->pop_front_aux(); }
        }

        /**
         * @brief 删除容器末尾的最后一个元素。
        */
        void pop_back(void)
        {
            if (this->empty()) { return; }
            
            if (this->finish.current != this->finish.first)
            {
                --finish.current;
                std::destroy_at(this->finish.current);
            }
            else 
            { 
                //printf("Call function: push_front_aux()\n");
                this->pop_back_aux(); 
            }
        }

        /**
         * @brief 清空整个 deque，但要保留 map 中的第一个节点。
        */
        void clear(void) __attribute__((optimize("O0")));

        /**
         * @brief 移除迭代器 __pos 所指向的元素。
         * 
         * @return 返回移除点 __pos 之后一个元素的迭代器。
        */
        iterator erase(iterator __pos)
        {
            iterator next = __pos;
            ++next;

            /**
             * 计算清除点 __pos 之前的元素数。
            */
            difference_type index = __pos - this->start;

            /**
             * 如果清除点前的元素较少，
             * 就把清除点之前的所用元素直接向后移动一个元素进行覆盖，然后再移除第一个元素即可。
             * 
             * (this->size() >> 1) 等价于 (this->size() / 2)，计算容器内总元素数的一半。
            */
            if (index < (this->size() >> 1))
            {
                std::copy_backward(this->start, __pos, next);
                this->pop_front();
            }
            /**
             * 如果清除点后的元素较少，
             * 就把清除点之后的所用元素直接向前移动一个元素进行覆盖，然后再移除最后一个元素即可。
            */
            else
            {
                std::copy(next, this->finish, __pos);
                this->pop_back();
            }

            return this->start + index;
        }

        /**
         * @brief 移除迭代器 [__first，__last) 区间内的所有元素。
        */
        iterator erase(iterator __first, iterator __last)
        {
            if (__first == this->start && __last == this->finish)
            {
                this->clear();
                return this->finish;
            }
            else
            {
                difference_type n = __last - __first;                   // 计算要清除的元素数
                difference_type element_before = __first - this->start; // 计算清除区间前方的元素数

                /**
                 * 若 清除区间前方的元素数 < (清除区间前方的元素数 + 清除区间后方的元素数 / 2)
                 * 即代表 清除区间前方的元素数 较少。
                */
                if (element_before < (this->size() - n) >> 1)
                {
                    std::copy_backward(this->start, __first, __last);
                    iterator newStart = this->start + n;
                    std::destroy(this->start, newStart);

                    for (map_pointer cur = this->start.node; cur < newStart.node; ++cur)
                    {
                        data_allocator::deallocate(*cur, iterator::getBufferSize());
                    }

                    this->start = newStart;
                }
                else // 若 清除区间后方的元素数 较少
                {
                    std::copy(__last, this->finish, __first);
                    iterator newFinish = this->finish - n;
                    std::destroy(newFinish, this->finish);

                    for (map_pointer cur = this->finish.node; cur < newFinish.node; ++cur)
                    {
                        data_allocator::deallocate(*cur, iterator::getBufferSize());
                    }

                    this->finish = newFinish;
                }

                return this->start + element_before;
            }
        }

        /**
         * @brief 往迭代器 __pos 之前插入一个元素 __value。
         * 
         * @return 插入完成后 __value 所在位置的迭代器。
        */
        iterator insert(iterator __pos, const value_type & __value)
        {
            if (this->start == __pos) 
            {  
                this->push_front(__value);
                return this->start;
            }
            else if (this->finish == __pos)
            {
                this->push_back(__value);
                iterator tempFinish = this->finish;
                return (--tempFinish);
            }
            else
            {
                return this->insert_aux(__pos, __value);
            }
        }
};

template <typename Type, std::size_t BufferSize, typename Alloc>
void My_Deque<Type, BufferSize, Alloc>::create_map_and_nodes(size_type __numElements)
{
    /**
     * 需要的节点数 = 需要创建的元素数 / 缓冲区可容纳的元素数 + 1
     * 若刚好整除，会多配一个节点。
    */
    size_type nodesCount = __numElements / iterator::getBufferSize() + 1;

    /**
     * 中控 map 需要管理几个节点，最少为 8 个，最多为 需要的节点数 + 2
    */
    this->map_size = std::max(this->initial_map_size(), nodesCount + 2);

    /**
     * 在 map 上配置 map_size 个节点。
    */
    this->map = map_allocator::allocate(this->map_size);

    map_pointer nStart  = this->map + (this->map_size - nodesCount) / 2;
    map_pointer nFinish = nStart + nodesCount - 1;
    map_pointer nCurrent;

    try
    {
        for (nCurrent = nStart; nCurrent <= nFinish; ++nCurrent)
        {
            *nCurrent = this->allocate_node();
        }
    }
    catch(...) 
    {
        map_pointer tempStart = nStart;

        for (tempStart; tempStart <= nCurrent; ++tempStart)
        {
            this->deallocate_node(*tempStart);
        }

        throw;
    }

    this->start.setNode(nStart);
    this->finish.setNode(nFinish);
    start.current  = start.first;
    finish.current = finish.first + __numElements % iterator::getBufferSize();
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
        map_pointer tempStart = this->start.node;
        
        size_type haveConstruct = current - tempStart; // 计算当前已经构建完成多少个节点

        std::destroy_n(tempStart, haveConstruct);      // 通通销毁

        throw;                                         // 完成后再抛出给调用端。
    }
}

template <typename Type, std::size_t BufferSize, typename Alloc>
template <typename ForwardIterator>
void My_Deque<Type, BufferSize, Alloc>::range_initialize(ForwardIterator __first, ForwardIterator __last)
{
    try
    {
        for (; __first != __last; ++__first)
        {
            this->push_back(*__first);
        }
    }
    catch (...)
    {
        this->clear();
        throw;
    }
}

template <typename Type, std::size_t BufferSize, typename Alloc>
void My_Deque<Type, BufferSize, Alloc>::reallocate_map(size_type __nodesToAdd, bool __addAtFront)
{
    size_type oldNodesCount = this->finish.node - this->start.node + 1;
    size_type newNodesCount = oldNodesCount + __nodesToAdd;

    map_pointer newNStart;

    if (this->map_size > 2 * newNodesCount)
    {
        newNStart = this->map + (this->map_size - newNodesCount) / 2 +
                    (__addAtFront ? __nodesToAdd : 0);

        if (newNStart < this->start.node)
        {
            std::copy(this->start.node, this->finish.node + 1, newNStart);
        }
        else
        {
            std::copy_backward(this->start.node, this->finish.node + 1, (newNStart + oldNodesCount));
        }
    }
    else
    {
        size_type newMapSize = this->map_size + std::max(this->map_size, __nodesToAdd) + 2;
        map_pointer newMap = map_allocator::allocate(newMapSize);

        newNStart = newMap + (newMapSize - newNodesCount) / 2 +
                    (__addAtFront ? __nodesToAdd : 0);
        
        std::copy(this->start.node, this->finish.node + 1, newNStart);

        delete[] this->map;

        this->map = newMap;
        this->map_size = newMapSize;
    }

    this->start.setNode(newNStart);
    this->finish.setNode(newNStart + oldNodesCount - 1);
}

template <typename Type, std::size_t BufferSize, typename Alloc>
void My_Deque<Type, BufferSize, Alloc>::push_front_aux(const value_type & __value)
{
    value_type valueCopy = __value;

    this->reserve_map_at_front();

    *(this->start.node - 1) = this->allocate_node();

    try
    {
        this->start.setNode(this->start.node - 1);
        this->start.current = start.last - 1;

        std::_Construct(this->start.current, valueCopy);
    }
    catch (...)
    {
        this->start.setNode(this->start.node + 1);
        this->start.current = this->start.first;
        this->deallocate_node(*(this->start.node - 1));

        throw;
    }
}

template <typename Type, std::size_t BufferSize, typename Alloc>
void My_Deque<Type, BufferSize, Alloc>::push_back_aux(const value_type & __value)
{
    value_type valueCopy = __value;

    this->reserve_map_at_back();

    /**
     * 配置一个新节点。
    */
    *(this->finish.node + 1) = this->allocate_node();

    /**
     * 旧版 STL 使用的是 __STL_TRY 和 __STL__UNWIND 宏来处理异常，
     * 但这基本被废弃，现代 C++ 编译器还有哪个不支持异常操作？
    */
    try
    {
        std::_Construct(this->finish.current, valueCopy);
        this->finish.setNode(this->finish.node + 1);
        this->finish.current = this->finish.first;
    }
    catch (...) 
    {
        this->deallocate_node(*(this->finish.node + 1));

        throw;
    }
}

template <typename Type, std::size_t BufferSize, typename Alloc>
void My_Deque<Type, BufferSize, Alloc>::pop_back_aux(void)
{
    /**
     * 1. 释放掉迭代器 finish 所指的整个缓冲区。
     * 
     * 2. 把 finish 迭代器向前移动一位。
     * 
     * 3. 调整 current 指针的位置到当前缓冲区的最后一个元素。
     * 
     * 4. 析构掉这个元素，操作完成。
    */
    this->deallocate_node(this->finish.first);
    this->finish.setNode(this->finish.node - 1);
    this->finish.current = this->finish.last - 1;
    std::destroy_at(this->finish.current);
}

template <typename Type, std::size_t BufferSize, typename Alloc>
void My_Deque<Type, BufferSize, Alloc>::pop_front_aux(void)
{
    /**
     * 1. 析构掉迭代器 start 所指的现行元素。
     * 
     * 2. 释放掉迭代器 start 所指的整个缓冲区。
     * 
     * 3. 调整 start 迭代器到 map 的下一个缓冲区。
     * 
     * 4. 设置迭代器 start 所指的现行元素为当前缓冲区的第一个元素。
    */
    std::destroy_at(this->start.current);
    this->deallocate_node(this->start.first);
    this->start.setNode(this->start.node + 1);
    this->start.current = this->start.first;
}

template <typename Type, std::size_t BufferSize, typename Alloc>
void My_Deque<Type, BufferSize, Alloc>::clear(void)
{
    /**
     * 先析构和释放 map 中除头尾节点外的节点数据。
    */
    for (map_pointer node = this->start.node + 1; node < this->finish.node; ++node)
    {
        std::destroy(*node, *node + iterator::getBufferSize());
        data_allocator::deallocate(*node, iterator::getBufferSize());
    }

    /**
     * 然后再处理头尾两个节点的数据，
     * 注意需要保留迭代器 this->start 所指的缓冲区，不得释放。
    */
    if (this->start.node != this->finish.node)
    {
        std::destroy(this->start.current, this->start.last);
        std::destroy(this->finish.first, this->finish.current);
        data_allocator::deallocate(this->finish.first, iterator::getBufferSize());
    }
    else    // 处理只有一个迭代器的结果
    {
        std::destroy(this->start.current, this->finish.current);      
    }

    this->finish = this->start; // 调整首尾迭代器
}

template <typename Type, std::size_t BufferSize, typename Alloc>
typename My_Deque<Type, BufferSize, Alloc>::iterator 
My_Deque<Type, BufferSize, Alloc>::insert_aux(iterator __pos, const value_type & __value)
{
    difference_type index = __pos - this->start;    // 计算插入点 __pos 之前的元素数。
    value_type valueCopy = __value;

    if (index < (this->size() >> 1))    // 若插入点之前的元素较少
    {
        /**
         * 调用 push_front 将当前容器内第一个元素的值插入，
         * 这通常会触发 push_front_aux() 甚至 reserve_map_at_front() 操作，往前面增加缓冲区。
        */
        this->push_front(this->front());

        /**
         * 创建两个迭代器 front_1 和 front_2，用于标记。
        */
        iterator front_1 = this->start; ++front_1;
        iterator front_2 = front_1; ++front_2;

        // 重新定位插入点。
        __pos = this->start + index;
        iterator pos_1 = __pos; ++pos_1;

        std::copy(front_2, pos_1, front_1);
    }
    else
    {
        /**
         * 调用 push_back 将当前容器内第一个元素的值插入，
         * 这通常会触发 push_back_aux() 
         * 甚至 reserve_map_at_back() 操作，往后面增加缓冲区。
        */
        this->push_back(this->back());

        /**
         * 创建两个迭代器 back_1 和 back_2，用于标记。
        */
        iterator back_1 = this->finish; --back_1;
        iterator back_2 = back_1;       --back_2;

        __pos = start + index;
        
        //执行反向拷贝操作。
        std::copy_backward(__pos, back_2, back_1);
    }

    *__pos = valueCopy;
    return __pos;
}

#endif // __MY_DEQUE_H_