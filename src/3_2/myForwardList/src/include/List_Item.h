#ifndef _LIST_ITEM_H_
#define _LIST_ITEM_H_

#include <vector>
#include <iostream>
#include <algorithm>

/**
 * @brief        链表节点的构成（主模板）
 * 
 * @tparam Type  每个节点内部数据的类型
*/
template <typename Type>
class MyListItem
{
    private:
        Type value;                 // 节点内部数据
        MyListItem * nextItem;      // 指向下一个节点的指针

    public:
        /**
         * @brief 构建函数，用于构建该节点
         * 
         * @param __value   传入的节点内部数据
         * @param __next    传入指向下一节点的指针
        */
        MyListItem(const Type __value, MyListItem<Type> * __next) noexcept : value(__value), nextItem(__next) {}

        /**
         * @brief 返回该节点的值，为了保护节点值，故采用值传递
         * 
         * @return 节点值
        */
        Type getValue() const { return value; }
        
        /**
         * @brief 返回指向下一个节点的指针
         * 
         * @return 指向下一个节点的指针
        */
        MyListItem * next() const { return nextItem; }

        /**
         * @brief 设置下一个节点的指针
         * 
         * @param __nextItemPtr 指向下一个节点的指针
         * 
         * @return non-return
        */
        void setNext(MyListItem * __nextItemPtr) { nextItem = __nextItemPtr; } 
};

/**
 * @brief 针对 std::vector<Type> 的特化版本，主要增加了输出 operator<<() 方面的改进
 * 
 * @tparam Type 和主模板不同，这里的 Type 指的是 vector 中每一个元素的类型
*/
template <typename Type>
class MyListItem<std::vector<Type>>
{
    public:
        using typeVec = std::vector<Type>;
        using sizeType = std::size_t;

    private:
        typeVec value;              // 节点内部数据（指定为 typeVec）
        MyListItem * nextItem;      // 指向下一个节点的指针

    public:
        /**
         * @brief 构建函数，用于构建该节点
         * 
         * @param __value   传入的节点内部数据（指定为 typeVec）
         * @param __next    传入指向下一节点的指针
        */
        MyListItem(const typeVec & __value, MyListItem<typeVec> * __next) noexcept : value(__value), nextItem(__next) {}

        /**
         * @brief   获取 typeVec 中元素节点的个数
        */
        sizeType valSize() const { return value.size(); }

        /**
         * @brief 返回该节点的值，为了保护节点值，故采用值传递
         * 
         * @return 节点值（指定为 typeVec）
        */
        typeVec getValue() const { return value; }

        /**
         * @brief 返回指向下一个节点的指针
         * 
         * @return 指向下一个节点的指针
        */    
        MyListItem * next() const { return nextItem; }

        /**
         * @brief 设置下一个节点的指针
         * 
         * @param 指向下一个节点的指针
         * 
         * @return non-return
        */
        void setNext(MyListItem * __nextItem) { nextItem = __nextItem; } 

        /**
         * @brief 针对 typeVec （连续且线性的容器）设计的输出 << 运算符重载
         * 
         * @param  __os         标准输出的引用
         * @param  __vecItem    MyListItem<typeVec> 类的引用
         * 
         * @param 返回 std::ostream 用于链式调用
        */
        friend std::ostream & operator<<(std::ostream & __os, MyListItem<typeVec> & __vecItem)
        {
            //__os << "Vector Size = " << __vecItem.valSize() << '\n';
            for (auto __element : __vecItem.getValue()) { __os << __element << ' '; }
            return __os;
        }
};

#endif // _LIST_ITEM_H_