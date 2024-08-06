#ifndef _RB_TREE_ITERATOR_H_
#define _RB_TREE_ITERATOR_H_

#include "./RB_Tree_Base_Iterator.h"

template <typename Type, typename Ref, typename Ptr>
struct RBTree_Iterator : public RBTree_Base_Iterator
{
    typedef Type value_type;
    typedef Ref  reference;
    typedef Ptr  pointer;

    typedef RBTree_Iterator<Type, Type *, Type &> iterator;
    typedef RBTree_Iterator<const Type, const Type *, const Type &> const_reference;
    typedef RBTree_Iterator<Type, Ref, Ptr> self;
    typedef RBTree_Node<Type> * link_type;

    RBTree_Iterator() { this->node = nullptr;  }
    RBTree_Iterator(link_type __node) { this->node = __node; }
    RBTree_Iterator(const RBTree_Iterator & iter) { this->node = iter.node; }

    reference operator*() const { return link_type(this->node)->value_field; }

    pointer  operator->() const { return &(this->operator*()); }

    self & operator++()  { this->increment(); return *this; }

    self operator++(int) 
    { 
        self tempIter = *this; 
        this->increment();
        return tempIter;
    }

    self & operator--()  { this->decrement(); return *this; }
    
    self operator--(int) 
    { 
        self tempIter = *this;
        this->decrement();
        return tempIter;
    }
};

#endif // _RB_TREE_ITERATOR_H_