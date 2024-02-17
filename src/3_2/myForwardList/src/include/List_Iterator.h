#ifndef _LIST_ITERATOR_H_
#define _LIST_ITERATOR_H_

#include <stdexcept>

/**
 * @brief 为 MyForwardList 设计的迭代器，这款迭代器无论 单/双 向链表都适配。
 * @brief - 双向链表还需要重载 - 和 -- 运算符
 * 
 * @tparam Item 链表节点类型，一般都为 MyListItem<Type>
*/
template <typename Item>
class ListIterator
{
    private:
        Item * itemPointer;     // 指向某个节点的指针

    public:

        /**
         * @brief 构建函数，用于初始化迭代器
         * 
         * @param __itemPtr 指向某个节点的指针（默认为空，即空迭代器）
        */
        ListIterator(Item * __itemPtr = nullptr) noexcept : itemPointer(__itemPtr) {}

        /**
         * @brief 获取指向某个节点的指针（裸指针）
         * 
         * @return 指向某个节点的指针（裸指针）
        */
        Item * get() const { return itemPointer; }

        /**
         * @brief 重载 * 运算符
         * 
         * @return  该节点整体
        */
        Item & operator*() const { return *itemPointer; }

        /**
         * @brief 重载 * 运算符
         * 
         * @return  指向某个节点的指针
        */
        Item * operator->() const { return itemPointer; }

        /**
         * @brief 重载 ++ 运算符，用于实现 pre-increment（前置自增）
         * 
         * @return 返回自增后的迭代器本身的引用
        */
        ListIterator & operator++() { itemPointer = itemPointer->next(); return *this; }

        /**
         * @brief 重载 ++ 运算符，用于实现 post-increment（后置自增）
         * 
         * @return 返回自增后的迭代器本身
        */
        ListIterator operator++(int) { ListIterator tempIter = *this; ++(*this); return tempIter; }

        /**
         * @brief 重载 + 运算符，用于实现指定迭代器偏移到某个节点的功能
         * 
         * @param __val     偏移量，必须为正数
         * 
         * @return 返回自增后的迭代器本身的引用
        */
        ListIterator operator+(int64_t __val)
        { 
            if (__val < 0)      // 当偏移量小于 0 时，直接抛无效参数异常
            { 
                throw std::invalid_argument("From ListIterator::operator+(std::size_t __val): \nParameter val cannot be negative for forward iterator.\n"); 
            }

            for (std::size_t index = 0; index < __val; ++index) { itemPointer = itemPointer->next(); }

            return *this;
        }

        /**
         * @brief 重载 == 运算符，用于判断两个迭代器是否相等
        */
        bool operator==(const ListIterator & __iter) const { return (itemPointer == __iter.itemPointer); }

        /**
         * @brief 重载 != 运算符，用于判断两个迭代器是否不相等
        */
        bool operator!=(const ListIterator & __iter) const { return (itemPointer != __iter.itemPointer); }
};

#endif // _LIST_ITERATOR_H_