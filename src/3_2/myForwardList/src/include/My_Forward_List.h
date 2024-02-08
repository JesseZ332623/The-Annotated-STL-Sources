#ifndef _MY_FORWARD_LIST_H
#define _MY_FORWARD_LIST_H

#include "./List_Item.h"
#include "./List_Iterator.h"
#include <iostream>
#include <algorithm>

template <typename Type>
class MyForwardList : public ListIterator<MyListItem<Type>>
{
    public:
        using Item = MyListItem<Type> *;
        using ListIter = ListIterator<MyListItem<Type>>;
    private:
        MyListItem<Type> *__end;
        MyListItem<Type> *__front;

        std::size_t __size;

    public:
        MyForwardList() : ListIter(__front), __end(nullptr), __front(nullptr), __size(0) {}

        void insertFront(Type __value);
        void insertEnd(Type __value);

        void deleteFront(void);
        void deleteEnd(void);

        ListIter begin() const { return ListIter(__front); }
        ListIter end() const { return ListIter(__end); }

        std::size_t size() const { return __size; }

        friend std::ostream &operator<<(std::ostream &__os, MyForwardList<Type> &__forwardList)
        {
            __os << "Node Size = " << __forwardList.size() << '\n';

            std::for_each(__forwardList.begin(), __forwardList.end(), 
                          [&__os](const MyListItem<Type> & __listItem) { __os << __listItem.getValue() << '\t'; }
                        );

            return __os;
        }

        ~MyForwardList();
};

template <typename Type>
void MyForwardList<Type>::insertFront(Type __value)
{
    MyListItem<Type> *newNode = new MyListItem<Type>(__value, __front);

    if (__front == nullptr)
    {
        __front = newNode;
        __end = newNode;
    }
    else
    {
        __front = newNode;
    }

    __size++;
}

template <typename Type>
void MyForwardList<Type>::insertEnd(Type __value)
{
    MyListItem<Type> *newItem = new MyListItem<Type>(__value, nullptr);

    if (__front == nullptr)
    {
        __front = newItem;
        __end = newItem;
    }
    else
    {
        __end->setNext(newItem);
        __end = newItem;
    }

    __size++;
}

template <typename Type>
void MyForwardList<Type>::deleteFront(void)
{
    if (__front == nullptr) { return; }
    else if (__front == __end) 
    {
        delete __front;
        __front = __end = nullptr;
        --__size;

        return;
    }
    else
    {
        MyListItem<Type> * tempNodePtr = __front;

        __front = __front->next();
        
        delete tempNodePtr;

        --__size;
    }
}

template <typename Type>
void MyForwardList<Type>::deleteEnd(void)
{
    if (__front == nullptr) { return; }
    else if (__front == __end) 
    {
        delete __front;
        __front = __end = nullptr;
        --__size;

        return;
    }
    else
    {
        MyListItem<Type> * tempNodePtr = __front;

        while (tempNodePtr->next() != __end)
        {
            tempNodePtr = tempNodePtr->next();
        }

        delete __end;
        __end = tempNodePtr;
        __end->setNext(nullptr);
        --__size;
    }
}

template <typename Type>
MyForwardList<Type>::~MyForwardList()
{
    ListIter tempIter = begin();

    while (tempIter != end())
    {
        Item tempItemPtr = tempIter.itemPointer; 
        ++tempIter;
        delete tempItemPtr;
    }
}

#endif // _MY_FORWARD_LIST_H