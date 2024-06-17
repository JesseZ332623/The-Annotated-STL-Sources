#ifndef _MY_VECTOR_H_
#define _MY_VECTOR_H_

#include <initializer_list>
#include <memory>
#include <new>

#include "./simpleAlloc.h"

template <typename Type, typename Alloc = std::allocator<Type>>
class My_Vector
{
    public:
        using valueType  = Type;
        using pointer    = valueType *;
        using iterator   = valueType *;
        using constIterator = const valueType *;
        using reference  = valueType &;
        using sizeType   = std::size_t;
        using differenceType = std::ptrdiff_t;

    protected:

        using dataAllocator = Simple_Alloc<valueType, Alloc>;

        iterator start;
        iterator finish;
        iterator endOfStorage;

        void insertAux(iterator __pos, const Type & __n);

        void deallocate()
        {
            if (this->start) { dataAllocator::deallocate(start, endOfStorage - start); }
        }

        iterator allocate_and_fill(sizeType __n, const Type & __value)
        {
            iterator result = dataAllocator::allocate(__n);
            std::uninitialized_fill_n(result, __n, __value);

            return result;
        }
        
        void fillInitialize(sizeType __n, const Type & __value)
        {
            start        = allocate_and_fill(__n, __value);
            finish       = start + __n;
            endOfStorage = finish; 
        }

    public:
        iterator begin()        { return this->start; }
        iterator end()          { return this->finish; }
        constIterator begin() const    { return this->start; }
        constIterator end()   const    { return this->finish; }
        
        sizeType size()     const      { return sizeType(this->end() - this->begin()); }
        sizeType capacity() const      { return sizeType(endOfStorage - this->begin()); }

        bool empty() const { return (this->begin() == this->end()); }


        reference operator[](sizeType __n) { return *(begin() + __n); }

        My_Vector() : start(nullptr), finish(nullptr), endOfStorage(nullptr) {}
        My_Vector(sizeType __n, const Type & __value) { this->fillInitialize(__n, __value); }
        My_Vector(int __n, const Type & __value) { this->fillInitialize(__n, __value); }
        My_Vector(long int __n, const Type & __value) { this->fillInitialize(__n, __value); }
        explicit My_Vector(sizeType __n) { this->fillInitialize(__n, Type()); }

        reference front() { return (*begin()); }
        reference back()  { return *(end() - 1); }
        
        ~My_Vector() 
        {
            std::destroy(start, finish);
            this->deallocate();
        }
};  

#endif // _MY_VECTOR_H_