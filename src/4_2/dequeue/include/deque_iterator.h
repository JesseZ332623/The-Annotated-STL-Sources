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
    static std::size_t setBufferSize(void) { return __dequeBufferSize(Buffer_Size, sizeof(Type)); }

    void setNode(map_pointer __newNode) 
    { 
        this->node  = __newNode;
        this->first = *__newNode;
        this->last  = this->first + difference_type(this->setBufferSize());
    }

    /**
     * @brief 取当前缓冲区现行元素的值。
    */
    reference operator*()  const { return *this->current; }

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
     * 
    */
    difference_type operator-(const self & __x) const
    {
        if (this == &__x) { return difference_type(0LL); }

        return difference_type(this->setBufferSize()) * (this->node - __x.node - 1) + 
               (this->current - this->first) + (__x.last - __x.current);
    }
};

#endif // __DEQUE_ITERATOR_H_