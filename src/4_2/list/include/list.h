#ifndef __LIST_H_
#define __LIST_H_

/**
 *  list 容器不仅是一个双向链表，还是一个环状双向链表，
 *  因此只需要一个指针，就能完整表现整个链表。 
*/

#include "./list_Iterator.h"
#include "../../simple_allocator/simpleAlloc.h"

#include <memory>
#include <algorithm>

template <typename Type, typename Alloc = std::allocator<Type>>
class MyList
{
    protected:
        using listNode       = ListNode<Type>;
        using nodeAllocator  = Simple_Alloc<listNode, Alloc>;
        using iterator  = typename ListIterator<Type, Type &, Type *>::iterator;
        using reference = Type &;
        using sizeType = std::size_t;

    public:
        using linkType = ListNode<Type> *;

    protected:
        linkType nodePointer;
        
    
    public:
        iterator begin() { return linkType((*nodePointer).next); }
        iterator end()   { return this->nodePointer; }

        bool empty() const { return (this->nodePointer->next == this->nodePointer); }

        sizeType size() const {
            return std::distance(this->begin(), this->end());
        }

        reference front() const { return *(this->begin()); }
        reference back()  const { return *(--this->end()); }
};

#endif // __LIST_H_