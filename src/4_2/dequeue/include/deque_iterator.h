#ifndef __DEQUE_ITERATOR_H_
#define __DEQUE_ITERATOR_H_

#include <iterator>
#include <type_traits>

/**
 * @brief 全局函数，
 *        供迭代器内 `setBufferSize()` 静态成员方法调用，计算 deque 内缓冲区大小。
 * 
 * @param __n     请求的缓冲区大小
 * @param __size  类型 Type 的大小
 * 
 * @return 计算后应该分配的缓冲区大小。
*/
inline std::size_t __dequeBufferSize(std::size_t __n, std::size_t __size)
{
    /**
     * 若 __n 不为 0，则传回 __n，表示缓冲区大小是由用户自定义的。
     * 
     * 若 __n 为 0，则需要判断 deque 内数据类型的大小是否小于 512 字节，
     * 是的话返回 512 / __size，不是的话返回 1。
    */
    return (__n != 0) ? __n : ((__size < 512) ? std::size_t(512 / __size) : std::size_t(1ULL)) ;
}

/**
 * @brief 供 deque 使用的迭代器，采用的是 `random access iterator`（随机访问迭代器），
 *        但双端队列的连续只是逻辑上的，所以不能像 `vector` 那样粗暴的使用裸指针当迭代器。
 * 
 * @tparam Type         双端队列内数据类型
 * @tparam Ref          Type 的引用
 * @tparam Ptr          Type 的指针
 * @tparam BufferSize   缓冲区大小
*/
template <typename Type, typename Ref, typename Ptr, std::size_t Buffer_Size>
struct Deque_Iterator
{
    using iterator        = Deque_Iterator<Type, Type &, Type *, Buffer_Size>;
    using const_iterator  = Deque_Iterator<const Type, const Type &, const Type *, Buffer_Size>;
    using self = Deque_Iterator;

    /**
     * 由于没有继承 std::iterator，
     * 所以必须自己写上相应的迭代器类型，才能被 STL 算法识别。
    */
    using iterator_category = std::random_access_iterator_tag;
    using iterator_traits   = std::iterator_traits<Type *>;

    using value_type         = Type;
    using pointer            = Type *;
    using reference          = Ref;
    using sizeType           = std::size_t;
    using difference_type    = std::ptrdiff_t;
    using map_pointer        = Type **;

    Type * first;       // 指向缓冲区第一个元素的指针
    Type * last;        // 指向缓冲区尾部（包含备用空间）的指针
    Type * current;     // 指向缓冲区现行元素的指针

    map_pointer node;   // 指向中控 map 的指针

    /**
     * @brief 计算应该分配多少缓冲区大小。
    */
    static std::size_t getBufferSize(void) { return __dequeBufferSize(Buffer_Size, sizeof(Type)); }

    /**
     * @brief 设置新节点，传入一个节点值 `__newNode`，
     *        可以随意切换到 map 中的任意节点
    */
    void setNode(map_pointer __newNode) 
    { 
        this->node  = __newNode;
        this->first = *__newNode;
        this->last  = this->first + difference_type(this->getBufferSize());
    }

    /**
     * @brief 取当前缓冲区现行元素的值。
    */
    reference operator*() const { return *this->current; }

    /**
     * @brief 取当前缓冲区现行元素的地址。
    */
    pointer   operator->() const { return &(this->operator*()); }

    /**
     * @brief 获取两个迭代器的距离。
     *        由于双端队列在逻辑上是连续的，所以要在分散的内存空间中维持连续的假象。
     * 
     * @brief - 假设有迭代器 a，b 指向 map 的不同节点。那么
     * 
     * @brief - 迭代器 a 和 b 的距离 = 两个节点的差值 * 缓冲区大小 + 
     *                                迭代器 a 在缓冲区内的相对位置 + 迭代器 b 在缓冲区内的相对位置
    */
    difference_type operator-(const self & __x) const
    {
        if (this == &__x) { return difference_type(0LL); }

        return difference_type(this->getBufferSize()) * (this->node - __x.node - 1) + 
               (this->current - this->first) + (__x.last - __x.current);
    }

    /**
     * @brief 参考 `More Effective C++`，
     *        条款 6: 区分自增运算符和自减运算符的前后缀形式。（此为后缀自增，拿到下一个元素的地址）
    */
    self & operator++()
    {
        ++this->current;

        if (this->current == this->last)    // 若达到当前缓冲区末尾
        {
            this->setNode(this->node + 1);  // 切换到下一个节点
            this->current = this->first;    // 指向下一个节点所指的缓冲区的第一个元素
        }

        return (*this);
    }

    /**
     * @brief 前缀自增
    */
    self operator++(int)
    {
        self tempIter = *this;
        ++*this;

        return tempIter;
    }

    /**
     * @brief 前缀自减，拿到前一个元素的地址。
    */
    self & operator--()
    {
        if (this->current == this->first) // 若达到当前缓冲区开头
        {
            /**
             * 切换到上一个节点最后一个元素的下一个位置。
            */
            this->setNode(this->node - 1);  
            this->current = this->last;      
        }

        --this->current;    // 切换至前一个元素

        return (*this);
    }

    /**
     * @brief 前缀自减。
    */
    self operator--(int)
    {
        self tempIter = *this;
        --*this;

        return tempIter;
    }

    /**
     * @brief 重载 += 运算符，实现 “随机存取” ，
     *        迭代器可以向前或向后跳跃 n 个元素的距离。
    */
    self & operator+=(difference_type __n)
    {
        /**
         * `(this->current - this->first)` 拿到当前缓冲区中剩余的字节数，
         * 加上 `__n` 得到偏移量 `offset`。
        */
        difference_type offset = __n + (this->current - this->first);

        /**
         * 偏移量在当前缓冲内 (0 < offset < getBufferSize())，直接偏移指针即可。
        */
        if (offset >= 0 && offset < difference_type(this->getBufferSize()))
        {
            this->current += __n;
        }
        else    // 偏移量 offset 超过了当前缓冲区，需要对节点 node 进行切换。
        {
            /**
             * 判断 offset 是否大于 0，来处理节点向前向后偏移的情况。
             * 
             * 1. 向前偏移，取 offset / bufferSize 的商（向上取整），作为向前偏移的节点数。
             * 
             * 2. 向后偏移，取 -(-offset - 1) / bufferSize 的商（避免向下取整的偏差），作为向后偏移的节点数。
            */
            difference_type nodeOffset = (offset > 0) ? offset / difference_type(this->getBufferSize())
                                                      : -difference_type((-offset - 1) / this->getBufferSize()) - 1;

            this->setNode(this->node + nodeOffset);

            this->current = this->first + (offset - nodeOffset * difference_type(this->getBufferSize()));
        }

        return (*this);
    }

    /**
     * @brief 参考 `More Effective C++` 条款 22: 
     *        `Consider using op= instead of stand-alone op`
     * 
     * @brief - 这句话鼓励程序员在设计类时优先考虑复合赋值运算符（如 operator+=, operator-=, operator*=, operator/= 等）的实现，
     *        而不是仅实现独立的二元运算符（如 operator+, operator-, operator*, operator/）
    */
    self operator+(difference_type __n) const
    {
        self temp = (*this);
        return temp += __n;
    }

    /**
     * @brief 用 operator+=() 来实现 operator-=()
    */
    self & operator-=(difference_type __n) { return *this += -__n; }

    self operator-(difference_type __n) const 
    {
        self temp = (*this);
        return temp -= __n;
    }

    /**
     * @brief 实现随机存取，迭代器可以直接跳跃 __n 个距离。
    */
    reference operator[](difference_type __n) const { return *(*this + __n); }

    /**
     * @brief 比较两个迭代器所指的现行元素的指针是否相等。
    */
    bool operator==(const self & __x) const { return (this->current == __x.current); }

    /**
     * @brief 比较两个迭代器所指的现行元素的指针是否不相等。
    */
    bool operator!=(const self & __x) const { return !(*this == __x); }

    /**
     * @brief 比较 (*this) 迭代器所指的现行元素的指针是否小于 __x 迭代器所指的现行元素的指针。
    */
    bool operator<(const self & __x) const 
    {  
        return (this->node == __x.node) ? (this->current < __x.current) : (this->node < __x.node);
    }
};

#endif // __DEQUE_ITERATOR_H_