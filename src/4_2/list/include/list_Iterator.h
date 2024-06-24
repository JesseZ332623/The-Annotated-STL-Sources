#ifndef _LIST_ITERATOR_H_
#define _LIST_ITERATOR_H_

#include <iterator>
#include "./listNode.h"

/**
 * 双向链表迭代器设计，
 * list 的迭代器不能够像 vector 那样使用裸指针，因为其节点不能保证在存储空间中连续存在。
 * list 迭代器必须有能力进行正确的递增，递减，取值，成员存取等操作。
 * 
 * 所以需要设计一个 Bidrectional Iterators（双向迭代器）
 * 
 * 此外，list 有一个重要的性质，插入（insert）和拼接（splice）操作都不会使原 list 迭代器失效，
 * 甚至 list 的删除节点操作（erase）都只是让单个节点的迭代器失效，表中的其他迭代器均不受影响。
*/

template <typename Type, typename Ref, typename Ptr>
struct ListIterator
{   
    /**
     * 定义了一个全功能的迭代器类型，
     * `Type &`, `Type *` 分别指明了引用和指针类型
    */
    using iterator = ListIterator<Type, Type &, Type *>;

    /**
     *  一个更通用的迭代器类型别名，
     *  它允许在模板参数中使用 `Ref` 和 `Ptr`，这使得迭代器可以接受不同类型的引用和指针。
     * 
     *  - 这种设计灵活性更高，可以在需要时传递不同的引用和指针类型，例如 `const` 或 `volatile` 修饰的类型。
     *  `self` 用于指代当前模板实例的类型，方便在类内部使用。
    */
    using self     = ListIterator<Type, Ref, Ptr>;

    /* 
        STL Bidrectional Iterators（双向迭代器）标记，
        - `std::bidirectional_iterator_tag` 是一个预定义的类型标签，表明这个迭代器属于双向迭代器类别。
           这是为了满足 STL 迭代器分类的要求，使得算法可以根据迭代器的类型来确定可以调用哪些操作。
    */
    using iteratorCategory = std::bidirectional_iterator_tag;

    using typeValue         = Type;
    using pointer           = Type *;
    using reference         = Ref;
    using linkType          = ListNode<Type> *;     // 指向了 `ListNode<Type>` 节点的指针类型
    using sizeType          = std::size_t;
    using differenceType    = std::ptrdiff_t;

    linkType node;          // 指向了一个 `ListNode<Type>` 节点的指针

    /**
     * @brief 默认构造函数。 
    */
    ListIterator() {}

    /**
     * @brief 参数构造函数，
     *        传入 `ListNode<Type>` 节点的指针构造一个迭代器。
    */
    ListIterator(linkType __x) : node(__x) {}

    /**
     * @brief 拷贝构造函数。
    */
    ListIterator(const iterator & __x) : node(__x.node) {}

    /**
     * @brief 比较运算符重载，
     *        用于比较两个迭代器所保存的节点地址是否相等。
    */
    bool operator==(const self & __x) const { return (this->node = __x.node); }

    /**
     * @brief 比较运算符重载，
     *        用于比较两个迭代器所保存的节点地址是否不相等。
    */
    bool operator!=(const self & __x) const { return (this->node != __x.node); }

    /**
     * @brief 迭代器取值操作，
     *        取的是 `ListNode<Type>` 节点内 `nodeData` 的值。
    */
    reference operator*() const { return (*this->node).nodeData; }

    /**
     * @brief 成员访问运算符重载，
     *        获取 `ListNode<Type>` 节点内 `nodeData` 的地址。
    */
    pointer operator->() const { return &(this->operator*()); }

    /**
     * @brief 自增运算符重载，
     *        保存 `ListNode<Type>` 节点内 `next` 指针的地址（即移动到链表的下一个节点）
    */
    self & operator++()
    {
        this->node = (linkType)((*this->node).next);

        return *this;
    }

    /**
     * @brief 基于 `self & operator++()` 的自增运算符重载，
     *        迭代器自身向后移动一节点，但返回偏移前的迭代器。
     * 
     * @brief - 这种设计在需要先使用当前迭代器值然后再移动迭代器的情况下非常有用，
     *          例如在循环中同时遍历和修改容器内容时。
    */
    self & operator++(int)
    {
        self tempIter = (*this);
        ++(*this);

        return tempIter;
    }

    /**
     * @brief 自减运算符重载，
     *        保存 `ListNode<Type>` 节点内 `prev` 指针的地址（即移动到链表的上一个节点）
    */
    self & operator--()
    {
        this->node = (linkType)((*this->node).prev);

        return *this;
    }

    /**
     * @brief 和 `self & operator++(int)` 性质相似。
    */
    self & operator--(int)
    {
        self tempIter = (*this);
        --(*this);

        return tempIter;
    }
};

#endif // _LIST_ITERATOR_H_