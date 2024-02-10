#ifndef _LIST_ITERATOR_H_
#define _LIST_ITERATOR_H_

//#include "./List_Item.h"
#include <iterator>
#include <exception>

template <typename Item>
class ListIterator
{
    private:
        Item * itemPointer;

    public:

        //template<typename T>
        //struct iterator_traits<ListIterator<T>> { using iterator_category = std::forward_iterator_tag; };
        //using iterator_category = std::forward_iterator_tag;

        ListIterator(Item * __itemPtr = nullptr) : itemPointer(__itemPtr) {}

        Item * get() const { return itemPointer; }

        Item & operator*() const 
        { 
            return *itemPointer; 
        }
        Item * operator->() const 
        { 
            return itemPointer; 
        }

        ListIterator & operator++() 
        { 
            itemPointer = itemPointer->next(); 
            //if (!(itemPointer->next())) { throw std::out_of_range("Iterator out of range! operator++()\n"); }
            return *this; 
        }
        ListIterator operator++(int) 
        { 
            ListIterator tempIter = *this; 
            ++(*this); 
            //if (!(itemPointer->next())) { throw std::out_of_range("Iterator out of range! operator++(int)\n"); }
            return tempIter; 
        }

        ListIterator operator+(std::size_t __val) 
        { 
            if (__val < 0) { throw std::invalid_argument("Parameter val cannot be negative for forward iterator. operator+(std::size_t __val)"); }

            for (std::size_t index = 0; index < __val; ++index) 
            { 
                itemPointer = itemPointer->next();
                //if (!(itemPointer->next())) { throw std::out_of_range("Iterator out of range! operator+(std::size_t __val)"); }
            }

            return *this;
        }

        bool operator==(const ListIterator & __iter) const { return (itemPointer == __iter.itemPointer); }
        bool operator!=(const ListIterator & __iter) const { return (itemPointer != __iter.itemPointer); }
};

#endif // _LIST_ITERATOR_H_