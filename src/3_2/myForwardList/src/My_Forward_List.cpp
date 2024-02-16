#include "./include/My_Forward_List.h"

template <typename Type>
template <typename InputIterator>
void MyForwardList<Type>::rangeInitializerList(InputIterator __begin, InputIterator __end)
{
    for (InputIterator iter = __begin; iter != __end; ++iter)
    {
        this->insertEnd(*iter);
    }
}

template <typename Type>
MyForwardList<Type>::MyForwardList(std::initializer_list<Type> __firstInitList) noexcept
{
    if (__firstInitList.size() == 0) { *this = MyForwardList<Type>(); return; }

    *this = MyForwardList<Type>();

    try
    {
        rangeInitializerList(__firstInitList.begin(), __firstInitList.end());
    }
    catch (const std::bad_alloc & __failedAlloc)     // 捕获
    {
        std::cerr << __failedAlloc.what() << '\n';   // 输出异常原因
        this->~MyForwardList();                     // 调用析构函数清理
        std::current_exception();                   // 保存异常
    }  
}

template <typename Type>
MyForwardList<Type>::MyForwardList(SizeType __nodeNumber) noexcept
{
    *this = MyForwardList<Type>();

    for (SizeType remainingNodes = __nodeNumber; remainingNodes > 0; --remainingNodes)
    {
        try
        {
            this->insertFront(0);
        }
        catch (const std::bad_alloc & __failedAlloc)     // 捕获
        {
            std::cerr << __failedAlloc.what() << '\n';   // 输出异常原因
            this->~MyForwardList();                     // 调用析构函数清理
            std::current_exception();                   // 保存异常
        }
    }
}

template <typename Type>
MyForwardList<Type>::MyForwardList(const MyForwardList<Type> &__forwardList) noexcept : nodeNumber(__forwardList.nodeNumber)
{
    /*处理自赋值的情况*/
    if (this == &__forwardList) { return; }

    /*
        将所有的拷贝逻辑扔进 try 块中，
        里面若有任何一个 new 抛出 std::bad_alloc 异常，都会被捕获并作清理。
    */
    try
    {
        /*创建一个临时迭代器暂存 __forwardList 第一个节点的迭代器*/
        ListIter tempIter = __forwardList.begin();

        /*处理第一个节点的拷贝*/
        if (tempIter != __forwardList.end())
        {
            /*
                将 __forwardList 对象头节点的值取出，
                构建出一个新节点，交由 __front 管理。

                当前链表仅有一个节点，头尾指针都指向同一个节点
            */
            __end = __front = new Item(tempIter->getValue(), nullptr);
            ++tempIter; // 临时迭代器自增，指向了 __forwardList 的第二个节点
        }

        /*
            接下来拷贝 __forwardList 对象头节点之外的节点，分为 3 步：
            - 将临时迭代器所指向节点的值取出，构建出新节点，其返回的指针设置为当前链表尾指针的下一个节点指针
            - 更新链表尾指针到新构建的节点上
            - 临时迭代器自增，指向了 __forwardList 的下一个个节点
        */
        while (tempIter != __forwardList.end())
        {
            __end->setNext(new Item(tempIter->getValue(), nullptr));
            __end = __end->next();
            ++tempIter;
        }
    }
    catch (const std::bad_alloc & __failedAlloc)     // 捕获
    {
        std::cerr << __failedAlloc.what() << '\n';   // 输出异常原因
        this->~MyForwardList();                     // 调用析构函数清理
        std::current_exception();                   // 保存异常
    }
}

template <typename Type>
MyForwardList<Type>::MyForwardList(MyForwardList<Type> && __forwardList) noexcept : 
nodeNumber(__forwardList.nodeNumber), __end(__forwardList.__end), __front(__forwardList.__front)
{
    __forwardList.nodeNumber = 0;
    __forwardList.__end = __forwardList.__front = nullptr;
}

template <typename Type>
void MyForwardList<Type>::insertFront(const Type __value)
{
    /*先创建一个新节点，将 __value 传入，并把 next 指针指向原链表的头部*/
    ItemPointer newNode = new MyListItem<Type>(__value, __front);

    /*若为空链表，直接将头尾指针全部指向这个新节点即可*/
    if (__front == nullptr) { __front = __end = newNode; }

    /*若不是空链表，直接将头节点指向该新节点，该节点就成为了新的头节点*/
    else { __front = newNode; }

    nodeNumber++; // 节点数 + 1
}

template <typename Type>
void MyForwardList<Type>::insertEnd(const Type __value)
{
    /*先创建一个新节点，将 __value 传入，并把 next 指针指向空*/
    ItemPointer newItem = new MyListItem<Type>(__value, nullptr);

    /*若为空链表，直接将头指针全部指向这个新节点即可*/
    if (__front == nullptr) { __front = newItem; }
    /*若不是空链表，在尾节指针点非空的情况下，将尾节点的下一个指针指向新节点*/
    else
    {
        if (__end)
        {
            __end->setNext(newItem);
        }
    }

    /*更新尾节点指针*/
    __end = newItem;

    nodeNumber++; // 节点数 + 1
}

template <typename Type>
void MyForwardList<Type>::deleteFront(void)
{
    /*若检查到是空链表，直接抛 bad_alloc 异常*/
    if (__front == nullptr) { throw std::length_error("Empty Forward List!\n"); }

    /*若检查到链表仅有一个节点*/
    else if (__front == __end)
    {
        delete __front;            /*删除该节点*/
        __front = __end = nullptr; /*把头尾指针重设为 空*/
    }
    /*若链表有多个节点*/
    else
    {
        /*暂存原首节点的地址*/
        ItemPointer tempNodePtr = __front;

        /*首节点指向下一个节点*/
        __front = __front->next();

        /*删除首节点*/
        delete tempNodePtr;
    }

    /*无论链表处在何种情况，进行删除操作后，节点数都 - 1*/
    --nodeNumber;
}

template <typename Type>
void MyForwardList<Type>::deleteEnd(void)
{
    /*若检查到是空链表，直接抛 bad_alloc 异常*/
    if (__front == nullptr) { throw std::length_error("Empty Forward List!\n"); }

    /*若检查到链表仅有一个节点*/
    else if (__front == __end)
    {
        delete __end;              /*删除该节点*/
        __front = __end = nullptr; /*把头尾指针重设为 空*/
    }
    /*若链表有多个节点*/
    else
    {
        /*暂存首节点的地址*/
        ItemPointer tempNodePtr = __front;

        /*从首节点开始遍历，直到当前尾节点前一个节点后停止*/
        while (tempNodePtr->next() != __end)
        {
            tempNodePtr = tempNodePtr->next();
        }

        delete __end;            /*删除原尾节点*/
        __end = tempNodePtr;     /*更新尾节点指针*/
        __end->setNext(nullptr); /*尾节点内部 next 指针指向空*/
    }

    /*无论链表处在何种情况，进行删除操作后，节点数都 - 1*/
    --nodeNumber;
}

template <typename Type>
void MyForwardList<Type>::insertBetween(ListIter __beforeIter, ListIter __targetIter, Type __value)
{
    ItemPointer newNode = new Item(__value, nullptr); /*创建一个独立节点，将 __value 写入*/

    /*将 目标节点的前一个节点的 next 指针指向新创建的节点*/
    __beforeIter->setNext(newNode);

    /*将新创建节点的 next 指针指向目标节点*/
    newNode->setNext(__targetIter.get());
}

template <typename Type>
bool MyForwardList<Type>::insert(Type __value, const ListIter __targetIter)
{
    /*先创造一个空节点*/
    ItemPointer newNode = nullptr;

    /*若 __targetIter 就是首节点的迭代器，直接调用头插*/
    if (__targetIter == begin())
    {
        insertFront(__value);
        return true;
    }
    /*若 __targetIter 就是尾节点的迭代器，直接调用尾插*/
    else if (__targetIter == end())
    {
        insertEnd(__value);
        return true;
    }
    else
    {
        ListIter beforeIter = begin();     // 保存 __targetIter 的前一个迭代器的迭代器
        ListIter currentIter = beforeIter; // 保存当前迭代器的迭代器

        /*
            由于单向链表的限制，
            必须从头开始搜索 __targetIter 的前一个迭代器
        */
        while (currentIter != __targetIter)
        {
            beforeIter = currentIter;
            ++currentIter;
        }

        /*当遍历到末尾都没发现目标迭代器*/
        if (beforeIter == end()) { return false; }

        /*调用内部的插入函数*/
        insertBetween(beforeIter, __targetIter, __value);

        ++nodeNumber; // 节点数 + 1
    }

    return true;
}

template <typename Type>
void MyForwardList<Type>::deleteBetween(ListIter __beforeIter, ListIter __targetIter)
{
    ListIter tempIter = __targetIter;
    __beforeIter->setNext(__targetIter->next());

    delete tempIter.get();
}

template <typename Type>
bool MyForwardList<Type>::erase(const ListIter __targetIter)
{
    /*若 __targetIter 就是首节点的迭代器，直接调用头插*/
    if (__targetIter == begin())
    {
        deleteFront();
        return true;
    }
    /*若 __targetIter 就是尾节点的迭代器，直接调用尾插*/
    else if (__targetIter == end())
    {
        deleteEnd();
        return true;
    }
    else
    {
        ListIter beforeIter = begin();     // 保存 __targetIter 前一个迭代器的迭代器
        ListIter currentIter = beforeIter; // 保存当前迭代器的迭代器

        /*
            由于单向链表的限制，
            必须从头开始搜索  __targetIter 前一个迭代器的迭代器
        */
        while (currentIter != __targetIter)
        {
            beforeIter = currentIter;
            ++currentIter;
        }

        /*当遍历到末尾都没发现目标迭代器*/
        if (beforeIter == end()) { return false; }

        deleteBetween(beforeIter, __targetIter);
    }

    --nodeNumber;
    return true;
}

template <typename Type> 
void MyForwardList<Type>::swap(MyForwardList<Type> & __list)
{
    ItemPointer tempFrontPointer = this->__front;
    ItemPointer tempEndPointer = this->__end;
    SizeType tempNodeNumber = this->nodeNumber;

    this->__front = __list.__front;
    this->__end = __list.__end;
    this->nodeNumber = __list.nodeNumber;

    __list.__front = tempFrontPointer;
    __list.__end = tempEndPointer;
    __list.nodeNumber = tempNodeNumber;
}

template <typename Type> 
void MyForwardList<Type>::sort()
{
    if (nodeNumber <= 1) { return; }
  
    MyForwardList<Type> sortedList;
    
    try 
    {
        ListIter currentIter = begin();

        while (currentIter != end()) 
        {  
            ListIter nextIter = currentIter; 
            ++nextIter;

            Type nodeValue = currentIter->getValue();

            ListIter sortedListIter = sortedList.begin();
                
            if (sortedListIter != sortedList.end()) { 
                while (sortedListIter != sortedList.end() && sortedListIter->getValue() < nodeValue) { ++sortedListIter; }  
            }

            if (sortedList.empty() || sortedListIter == sortedList.end()) { sortedList.insertFront(nodeValue); }
            else { sortedList.insert(nodeValue, sortedListIter); }
                
            currentIter = nextIter; 
        }
    }
    catch (std::out_of_range& e) 
    {
        std::cerr << e.what() << '\n';
        sortedList.~MyForwardList();
        std::current_exception();
    }

    *this = std::move(sortedList); 
}

template <typename Type> 
template <typename Function>
void MyForwardList<Type>::sort(Function __sortRule)
{
    MyForwardList<Type> sortedList;

    ListIter currentIter = begin();

    while (currentIter != end())
    {
        ListIter nextIter = currentIter;
        ++nextIter;

        ListIter sortedListIter = sortedList.begin();
        while (sortedListIter != sortedList.end() && __sortRule()) { ++sortedListIter; }

        if (sortedListIter == sortedList.begin()) { sortedList.insertFront(currentIter->getValue()); }
        else { sortedList.insert(currentIter->getValue(), sortedListIter); }

        currentIter = nextIter;
    }

    *this = std::move(sortedList);
}

template <typename Type>
MyForwardList<Type> & MyForwardList<Type>::operator=(const MyForwardList<Type> & __forwardList)
{
    if (this == &__forwardList) { return *this; }

    try
    {
        ListIter tempIter = __forwardList.begin();

        if (tempIter != __forwardList.end())
        {
            __end = __front = new Item(tempIter->getValue(), nullptr);
            ++tempIter;
        }

        while (tempIter != __forwardList.end())
        {
            __end->setNext(new Item(tempIter->getValue(), nullptr));
            __end = __end->next();
            ++tempIter;
        }
    }
    catch (const std::bad_alloc & __failedAlloc)
    {
        std::cerr << __failedAlloc.what() << '\n';
        this->~MyForwardList();
        std::current_exception();
    }

    nodeNumber = __forwardList.nodeNumber;

    return *this;
}

template <typename Type>
MyForwardList<Type> & MyForwardList<Type>::operator=(MyForwardList<Type> && __forwardList)
{
    __front = __forwardList.__front;
    __end = __forwardList.__end;
    nodeNumber = __forwardList.nodeNumber;

    __forwardList.__front = __forwardList.__end = nullptr;
    __forwardList.nodeNumber = 0;

    return *this;
}

template <typename Type>
MyForwardList<Type> & MyForwardList<Type>::operator=(std::initializer_list<Type> __initList)
{
    if (__initList.size() == 0) { return std::move(MyForwardList<Type>()); }

    *this = std::move(MyForwardList<Type>());

    try
    {
        rangeInitializerList(__initList.begin(), __initList.end());
    }
    catch (const std::bad_alloc & failedAlloc)
    {
        std::cerr << failedAlloc.what() << '\n';
        this->~MyForwardList();
        std::current_exception();
    }
    return *this;
}

template <typename Type>
std::ostream &operator<<(std::ostream &__os, const MyForwardList<Type> &__forwardList)
{
    if (__forwardList.size() == 0) { return __os; }

    __os << "List Node Count = " << __forwardList.size() << '\n';

    std::for_each(__forwardList.begin(), __forwardList.end(),
                  [&__os](const MyListItem<Type> &__listItem) { __os << __listItem.getValue() << '\t'; }
                 );

    return __os;
}

template <typename Type>
MyForwardList<Type>::~MyForwardList()
{
    if (begin() == nullptr) { return; }     // 若为空链表，什么也不做直接退出就行

    ListIter tempIter = begin();

    while (tempIter != end())
    {
        // 当来到链表末尾时，直接退出，避免误删空指针
        if (tempIter->next() == nullptr) { return; }

        ItemPointer tempItemPtr = tempIter.get();
        tempIter = tempIter->next();
        delete tempItemPtr;
    }
}