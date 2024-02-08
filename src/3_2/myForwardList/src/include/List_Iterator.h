#ifndef _LIST_ITERATOR_H_
#define _LIST_ITERATOR_H_

//#include "./List_Item.h"
#include <iterator>

template <typename Item>
class ListIterator
{
    public:
        Item * itemPointer;

        //template<typename T>
        //struct iterator_traits<ListIterator<T>> { using iterator_category = std::forward_iterator_tag; };
        //using iterator_category = std::forward_iterator_tag;

        ListIterator(Item * __itemPtr = nullptr) : itemPointer(__itemPtr) {}

        Item & operator*() const { return *itemPointer; }
        Item * operator->() const { return itemPointer; }

        ListIterator & operator++() { itemPointer = itemPointer->next(); return *this; }
        ListIterator operator++(int) { ListIterator tempIter = *this; ++(*this); return tempIter; }

        bool operator==(const ListIterator & __iter) const { return (itemPointer == __iter.itemPointer); }
        bool operator!=(const ListIterator & __iter) const { return (itemPointer != __iter.itemPointer); }
};

#endif // _LIST_ITERATOR_H_