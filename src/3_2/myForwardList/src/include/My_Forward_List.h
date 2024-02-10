#ifndef _MY_FORWARD_LIST_H
#define _MY_FORWARD_LIST_H

#include "./List_Item.h"
#include "./List_Iterator.h"
#include <iostream>
#include <algorithm>

/**
 * @brief 一个自制的单项链表模板类
 * 
 * @tparam Type     每一个 MyListItem 所承载数据的类型
*/
template <typename Type>
class MyForwardList : public ListIterator<MyListItem<Type>>
{
    public:
        /*将某个 MyListItem<Type> 类 起个别名为 Item，代表一个链表节点*/
        using Item = MyListItem<Type>;

        /*将指向某个 MyListItem<Type> 类（链表节点）的指针起别名为 ItemPointer */
        using ItemPointer = MyListItem<Type> *;

        /*将 MyListItem<Type> 类型的链表迭代器起别名为 ListIter*/
        using ListIter = ListIterator<MyListItem<Type>>;

        /*给 std::size_t 起个别名，这个类型用于统计链表节点数*/
        using SizeType = std::size_t;
    private:
        ItemPointer __end;          // 指向链表头节点的指针
        ItemPointer __front;        // 指向链表尾节点的指针

        SizeType nodeNumber;        // 链表节点数

        void insertBetween(Type __value, ListIter __beforeIter, ListIter __targetIter);

        void deleteBetween(ListIter __beforeIter, ListIter __targetIter);

    public:
        /*默认构建函数，用于初始化链表和它的迭代器*/
        MyForwardList() : ListIter(__front), __end(nullptr), __front(nullptr), nodeNumber(0) {}

        void insertFront(const Type __value);     // 从头部插入
        void insertEnd(const Type __value);       // 从尾部插入

        bool deleteFront(void);             // 从头部删除
        bool deleteEnd(void);               // 从头部删除

        /**
         * @brief 往 __targetIter 所指向节点的前一个节点插入新节点
         * 
         * @param __value   要插入链表的值，会在函数内构造成 MyListItem<Type>
         * @param __targetIter  目标节点迭代器，会在目标节点之前插入
         * 
         * @return 是否插入成功，比如 __targetIter 所指向节点不存在表中时返回 false
        */
        bool insert(Type __value, const ListIter __targetIter);
        
        /**
         * @brief 删除 节点内容为 __target 的节点
         *
         * @param __targetIter  目标节点
         * 
         * @return 是否删除成功，比如 __targetIter 所指向节点不存在表中时返回 false
        */
        bool erase(const ListIter __targetIter);                   

        ListIter begin() const { return ListIter(__front); }                // 返回头节点的迭代器
        ListIter end() const { return ListIter(__end->next()); }            // 返回尾节点后面一个节点的迭代器

        std::size_t size() const { return nodeNumber; }         // 返回当前链表节点数

        /**
         * @brief 类友元函数，用于输出整张链表的数据到文件或标准输出。
         * 
         * @param __os          std::ostream 类的引用
         * @param __forwardList 单项链表类的引用
         * 
         * @return std::ostream 类的引用
        */
        friend std::ostream &operator<< <>(std::ostream & __os, const MyForwardList<Type> & __forwardList);

        ~MyForwardList();
};

#endif // _MY_FORWARD_LIST_H