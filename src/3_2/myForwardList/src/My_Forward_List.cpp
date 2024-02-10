#include "./include/My_Forward_List.h"

template <typename Type>
void MyForwardList<Type>::insertFront(const Type __value)
{
    ItemPointer newNode = new MyListItem<Type>(__value, __front);

    if (__front == nullptr)
    {
        __front = __end = newNode;
    }
    else
    {
        __front = newNode;
    }

    nodeNumber++;
}

template <typename Type>
void MyForwardList<Type>::insertEnd(const Type __value)
{
    ItemPointer newItem = new MyListItem<Type>(__value, nullptr);

    if (__front == nullptr)
    {
        __front = newItem;
    }
    else
    {
        if (__end)
        {
            __end->setNext(newItem);
        }
    }

    __end = newItem;

    nodeNumber++;
}

template <typename Type>
bool MyForwardList<Type>::deleteFront(void)
{
    if (__front == nullptr) { return false; }
    else if (__front == __end) 
    {
        delete __front;
        __front = __end = nullptr;
        --nodeNumber;

        return true;
    }
    else
    {
        ItemPointer tempNodePtr = __front;

        __front = __front->next();
        
        delete tempNodePtr;

        --nodeNumber;
    }

    return true;
}

template <typename Type>
bool MyForwardList<Type>::deleteEnd(void)
{
    if (__front == nullptr) { return false; }
    else if (__front == __end) 
    {
        delete __front;
        __front = __end = nullptr;
        --nodeNumber;

        return true;
    }
    else
    {
        ItemPointer tempNodePtr = __front;

        while (tempNodePtr->next() != __end)
        {
            tempNodePtr = tempNodePtr->next();
        }

        delete __end;
        __end = tempNodePtr;
        __end->setNext(nullptr);
        --nodeNumber;
    }

    return true;
}

template <typename Type>
void MyForwardList<Type>::insertBetween(Type __value, ListIter __beforeIter, ListIter __targetIter)
{
    ItemPointer newNode = new Item(__value, nullptr);    /*创建一个独立节点，将 __value 写入*/

    __beforeIter->setNext(newNode);
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
        ListIter beforeIter = begin();          // 保存 __targetIter 前一个迭代器的迭代器
        ListIter currentIter = beforeIter;      // 保存当前迭代器的迭代器

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
        if (beforeIter == end()) 
        { 
            //throw std::out_of_range("Iterator out of range!");
            return false; 
        }

        insertBetween(__value, beforeIter, __targetIter);
    }

    ++nodeNumber;
    return true;
}

template <typename Type>
void MyForwardList<Type>::deleteBetween(ListIter __beforeIter, ListIter __targetIter)
{
    ListIter tempIter = __beforeIter->next();
    __beforeIter->setNext(__targetIter.get());

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
        ListIter beforeIter = begin();          // 保存 __targetIter 前一个迭代器的迭代器
        ListIter currentIter = beforeIter;      // 保存当前迭代器的迭代器

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
std::ostream &operator<<(std::ostream & __os, const MyForwardList<Type> & __forwardList)
{
    __os << "Node Count = " << __forwardList.size() << '\n';

    std::for_each(__forwardList.begin(), __forwardList.end(), 
                 [&__os](const MyListItem<Type> & __listItem) { __os << __listItem.getValue() << '\t'; }
                 );

    return __os;
}


template <typename Type>
MyForwardList<Type>::~MyForwardList()
{
    ListIter tempIter = begin();

    while (tempIter != end())
    {
        ItemPointer tempItemPtr = tempIter.get();
        ++tempIter;
        delete tempItemPtr;
    }
}