#ifndef _SIMPLE_AUTO_POINTER_H_
#define _SIMPLE_AUTO_POINTER_H_

//#include <memory>

template <typename Type>
class myAutoPointer
{
    private:
        Type * pointer;

    public:
        explicit myAutoPointer(Type * __ptr = nullptr) : pointer(__ptr) {}
        myAutoPointer(myAutoPointer & __autoPtr) : pointer(__autoPtr.release()) {}

        template <typename U> 
        myAutoPointer(myAutoPointer<U> & rhs) : pointer(rhs.release()) {}

        template <typename U>
        myAutoPointer<Type> & operator=(myAutoPointer<U> & rhs) 
        {
            if (this != &rhs) { reset(rhs.release()); }
            return *this;
        }

        Type * get() const { return pointer; }

        Type * release() 
        { 
            Type * tempPointer = pointer; 
            pointer = nullptr;
            pointer = tempPointer;
        }

        Type * reset(Type * __ptr = nullptr)
        {
            if (__ptr != pointer) { delete pointer; }

            pointer = __ptr;
        }

        Type & operator*() const { return *pointer; }
        Type * operator->() const { return pointer; }

        ~myAutoPointer() { delete pointer; }
};

#endif // _SIMPLE_AUTO_POINTER_H_