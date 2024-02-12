#ifndef _MY_FORWARD_LIST_H
#define _MY_FORWARD_LIST_H

#include "./List_Item.h"
#include "./List_Iterator.h"

#include <initializer_list>

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

        /**
         * @brief 从单向链表中指定位置插入节点的具体执行函数
         * 
         * @param __beforeIter  目标节点迭代器的前一个迭代器
         * @param __targetIter  目标迭代器
         * @param __value       要插入的值，在内部会被构造为 MyListItem<Type>
         * 
         * @return non-return
        */
        void insertBetween(ListIter __beforeIter, ListIter __targetIter, Type __value);

        /**
         * @brief 从单向链表中指定位置删除节点的具体执行函数
         * 
         * @param __beforeIter  目标节点迭代器的前一个迭代器
         * @param __targetIter  目标迭代器
         * 
         * @return non-return
        */
        void deleteBetween(ListIter __beforeIter, ListIter __targetIter);

        /**
         * @brief 加载初始化列表的数据到链表中
         * 
         * @param __begin 初始化列表首指针
         * @param __end   初始化列表尾指针
         * 
         * @return non-return
        */
        template <typename InputIterator>
        void rangeInitializerList(InputIterator __begin, InputIterator __end);

    public:
        /*默认构建函数，用于初始化链表和它的迭代器*/
        MyForwardList() noexcept : ListIter(__front), __end(nullptr), __front(nullptr), nodeNumber(0) {}

        /*参数构造函数，可以通过初始化列表 {1, 2, 3} 来初始化这张单向链表*/
        MyForwardList(std::initializer_list<Type> __initList) noexcept; 

        /*拷贝构建函数，传入另一个 MyForwardList<Type> 类对象，对其进行深拷贝*/
        MyForwardList(const MyForwardList & __forwardList) noexcept;

        /*移动构造函数，转移另一个 MyForwardList<Type> 类对象链表的所有权*/
        MyForwardList(MyForwardList && __forwardList) noexcept;

        /**
         * @brief 从链表头部插入
         * 
         * @param __value 要插入的值，会在内部被构建为一个节点（MyListItem<Type>）
         * 
         * @return non-return
        */
        void insertFront(const Type __value);

        /**
         * @brief 从链表尾部插入
         * 
         * @param __value 要插入的值，会在内部被构建为一个节点（MyListItem<Type>）
         * 
         * @return non-return
        */
        void insertEnd(const Type __value);

        /**
         * @brief 从链表头部删除节点
         * 
         * @return 是否删除成功，若对空链表执行该操作会抛异常
        */
        void deleteFront(void);

        /**
         * @brief 从链表尾部删除节点
         * 
         * @return 是否删除成功，若对空链表执行该操作会抛异常
        */
        void deleteEnd(void);

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

        ListIter begin(void) const { return ListIter(__front); }                // 返回头节点的迭代器
        ListIter end(void) const { return ListIter(__end->next()); }            // 返回尾节点后面一个节点的迭代器

        SizeType size(void) const { return nodeNumber; }                        // 返回当前链表节点数

        /**
        *  @brief 对这张单向链表进行排序（使用插入排序），默认情况为升序，
        *         并且使用 std::move() 转移排序后链表的所有权。
        * 
        *  @return non-return
        */
        void sort(void);
        
        /**
        * @brief 对这张单向链表进行排序（使用插入排序），但提供一个 `__sortRule` 参数，
        *         用户可以编写函数指针，Lamba 表达式，仿函数来指定排序的规则，并且使用 `std::move()` 转移排序后链表的所有权。
        * 
        * @param __sortRule 可以是 函数指针，Lamba 表达式或仿函数，用于指定排序规则
        * 
        * @return non-return
        */
        template <typename Function>
        void sort(Function __sortRule);

        /**
         * @brief 拷贝构造运算符
         * 
         * @param __forwardList MyForwardList<Type> 类对象的左值引用
         * 
         * @return MyForwardList<Type> 类对象的左值引用
        */
        MyForwardList & operator=(const MyForwardList & __forwardList);

        /**
         * @brief 移动构造运算符
         * 
         *  @param __forwardList MyForwardList<Type> 类对象的右值引用
         * 
         * @return MyForwardList<Type> 类对象的右值引用
        */
        MyForwardList & operator=(MyForwardList && __forwardList);
        
        /**
         * @brief 类友元函数，用于输出整张链表的数据到文件或标准输出。
         * 
         * @param __os          std::ostream 类的引用
         * @param __forwardList 单项链表类的引用
         * 
         * @return std::ostream 类的引用
        */
        friend std::ostream &operator<< <>(std::ostream & __os, const MyForwardList<Type> & __forwardList);

        /**
         * @brief 析构函数，通过循环逐个释放链表的节点
        */
        ~MyForwardList();
};

#endif // _MY_FORWARD_LIST_H